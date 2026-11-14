# Run BQt at BelowNormal priority and tee logs to logs/bqt-last-run.log.
$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')
if (-not (Test-Path bqt.exe)) { & (Join-Path $PSScriptRoot 'build.ps1') }
New-Item -ItemType Directory -Force logs | Out-Null
$p = Start-Process .\bqt.exe -PassThru -RedirectStandardOutput logs\bqt-last-run.log -RedirectStandardError logs\bqt-last-run.err.log
try { $p.PriorityClass = 'BelowNormal' } catch {}
Write-Host "[bqt] PID $($p.Id) (BelowNormal). Logs: logs/bqt-last-run*.log - stop with: Stop-Process -Id $($p.Id)"
