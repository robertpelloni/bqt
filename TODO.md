# TODO

## Immediate
- [ ] Add `go test ./internal/...` and `go build -buildvcs=false .` to Go CI.
- [x] Wire the `DemoSurface` into a real interactive Gio frame loop.
- [ ] Add richer runtime integration for `Popup`, `Dialog`, `Drawer`, `ToolTip`, `Menu`, `ScrollView`, and `ScrollBar`.
- [x] Add automated runtime validation for the live Gio frame loop.
- [x] Add richer synthetic frame-context coverage for shell/taskbar/start-menu interaction.
- [x] Implement interactive `ScrollBar` drag behavior and couple it to `ScrollView` state.
- [x] Improve scrollbar occupancy/overlay policy and two-axis behavior.
- [ ] Add richer scroll corner/theming behavior when both bars are visible.

## Near-Term QML/Quick Focus
- [x] Upgrade `WebView` JS bridge from compile-safe contract to executable runtime semantics.
- [x] Add touch/swipe behavior primitives.
- [ ] Add multi-touch/pinch/rotation primitives after the single-swipe baseline is exercised further.
- [ ] Re-audit declarative state/transition parity versus QML/Quick.
- [x] Add automated validation around WebView bridge queueing, correlation, and error routing.
- [x] Add automated tests around navigation/history callback behavior.
- [ ] Use the new BTK comparison doc to keep parity language grounded in verified runtime behavior rather than repo breadth.

## Quality
- [x] Add tests for Go kernel/data/net packages.
- [ ] Expand coverage into additional kernel services such as undo/clipboard/merge logic.
- [ ] Keep docs aligned with verified state.
- [ ] Revisit which abstractions should remain BobUI experiments versus long-term native framework candidates after each major parity milestone.
