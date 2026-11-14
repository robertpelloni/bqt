# BQt unified build (Go-first). Non-destructive: never deletes prior outputs.
# Uses D: Go cache when C: free space is low (default cache lives on C:).
$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')
if ((Get-PSDrive C).Free -lt 4GB) {
  $env:GOCACHE = 'D:\gocache-bqt'; $env:GOTMPDIR = 'D:\gotmp-bqt'
  New-Item -ItemType Directory -Force $env:GOCACHE, $env:GOTMPDIR | Out-Null
  Write-Host "[bqt] C: low on space - Go cache on D:"
}
$p = Start-Process go -ArgumentList 'build','-buildvcs=false','-o','bqt.exe','.' -NoNewWindow -PassThru
try { $p.PriorityClass = 'BelowNormal' } catch {}
$p.WaitForExit()
if ($p.ExitCode -ne 0) { throw "go build failed ($($p.ExitCode))" }
'success' | Set-Content .build_success
Write-Host "[bqt] Built bqt.exe"
