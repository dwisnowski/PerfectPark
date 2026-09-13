#Requires -Version 5.1
$ErrorActionPreference = "Stop"

function Test-Python {
    param([string]$Command)
    try {
        & $Command -c "import sys; raise SystemExit(0 if sys.version_info >= (3, 8) else 1)" 2>$null
        return ($LASTEXITCODE -eq 0)
    } catch {
        return $false
    }
}

function Find-Python {
    foreach ($name in @("python", "python3")) {
        $cmd = Get-Command $name -ErrorAction SilentlyContinue
        if ($null -eq $cmd) { continue }
        # Skip Windows Store execution-alias stubs that are not a real install.
        if ($cmd.Source -like "*\WindowsApps\*") { continue }
        if (Test-Python -Command $cmd.Source) {
            return $cmd.Source
        }
    }

    $root = Join-Path $env:LOCALAPPDATA "Programs\Python"
    if (Test-Path $root) {
        $candidates = Get-ChildItem -Path $root -Directory -Filter "Python3*" |
            ForEach-Object { Join-Path $_.FullName "python.exe" } |
            Where-Object { Test-Path $_ }
        foreach ($exe in $candidates) {
            if (Test-Python -Command $exe) {
                return $exe
            }
        }
    }

    return $null
}

function Add-PythonToPath {
    $root = Join-Path $env:LOCALAPPDATA "Programs\Python"
    if (-not (Test-Path $root)) { return }
    Get-ChildItem -Path $root -Directory -Filter "Python3*" | ForEach-Object {
        $dir = $_.FullName
        $scripts = Join-Path $dir "Scripts"
        if ($env:Path -notlike "*$dir*") {
            $env:Path = "$dir;$scripts;$env:Path"
        }
    }
}

$python = Find-Python
if (-not $python) {
    $winget = Get-Command winget -ErrorAction SilentlyContinue
    if (-not $winget) {
        Write-Error @"
Python 3.8+ is required, but neither Python nor winget was found.
Install Python from https://www.python.org/downloads/windows/ (enable Add python.exe to PATH), then re-run: make on-windows-install
"@
    }

    Write-Host "Python not found; installing Python 3.12 via winget..."
    & winget install -e --id Python.Python.3.12 --scope user `
        --accept-package-agreements --accept-source-agreements
    if ($LASTEXITCODE -ne 0 -and $LASTEXITCODE -ne -1978335189) {
        # -1978335189 = already installed / no newer version (winget)
        Write-Error "winget failed to install Python (exit $LASTEXITCODE)."
    }

    Add-PythonToPath
    $python = Find-Python
}

if (-not $python) {
    Write-Error @"
Python was installed but is not available in this shell yet.
Open a new terminal and re-run: make on-windows-install
"@
}

Write-Host "Using $python"
& $python -m pip install -U platformio
if ($LASTEXITCODE -ne 0) {
    Write-Error "pip failed to install PlatformIO (exit $LASTEXITCODE)."
}

Write-Host "PlatformIO install complete. Ensure pio is on PATH (open a new terminal if needed)."
