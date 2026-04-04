# TODO

## Immediate
- [ ] Add `go test ./internal/...` and `go build -buildvcs=false .` to Go CI.
- [ ] Wire the `DemoSurface` into a real interactive Gio frame loop.
- [ ] Add richer runtime integration for `Popup`, `Dialog`, `Drawer`, `ToolTip`, `Menu`, `ScrollView`, and `ScrollBar`.
- [ ] Implement interactive `ScrollBar` drag behavior and couple it to `ScrollView` state.

## Near-Term QML/Quick Focus
- [ ] Upgrade `WebView` JS bridge from compile-safe contract to executable runtime semantics.
- [ ] Add touch/swipe behavior primitives.
- [ ] Re-audit declarative state/transition parity versus QML/Quick.
- [ ] Use the new BTK comparison doc to keep parity language grounded in verified runtime behavior rather than repo breadth.

## Quality
- [ ] Add tests for Go kernel/data/net packages.
- [ ] Keep docs aligned with verified state.
- [ ] Revisit which abstractions should remain BobUI experiments versus long-term native framework candidates after each major parity milestone.
