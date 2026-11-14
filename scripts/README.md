# Scripts

Canonical entry points (use these; root `build.bat`/`start.bat` remain as thin legacy wrappers):

- `build.ps1` — Go build to `bqt.exe` at BelowNormal priority; auto-uses `D:\gocache-bqt` when C: is low.
- `run.ps1` — build if needed, run at BelowNormal, logs to `logs/bqt-last-run*.log`.

Legacy helpers kept (not deleted): `check_release.py`, `generate_dashboard.py`, `omni_neural_bridge.py`.
Root Qt-era `configure.bat`/`configure` belong to the vendored qtbase fork; do not use them for the Go runtime.
