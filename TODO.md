# TODO

## Immediate
- [ ] Triage the massive untracked file set created by the earlier blanket `qt` -> `bobui` rename.
- [ ] Determine which renamed C++ artifacts are canonical source vs generated noise.
- [ ] Add `go test ./internal/...` and `go build -buildvcs=false .` to Go CI.
- [ ] Integrate `Popup`, `Menu`, and `WebView` into a runnable Go demo path.

## Near-Term QML/Quick Focus
- [ ] Implement `Dialog`, `Drawer`, `ToolTip`, and `ScrollBar` parity.
- [ ] Extend `WebView` with navigation signals and a JS bridge contract.
- [ ] Re-audit declarative state/transition parity versus QML/Quick.

## Quality
- [ ] Add tests for Go kernel/data/net packages.
- [ ] Keep docs aligned with verified state.
