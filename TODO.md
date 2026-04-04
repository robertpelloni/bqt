# TODO

## Immediate
- [x] Break major Go package cycles.
- [x] Make `go test ./internal/...` pass.
- [x] Make `go build -buildvcs=false .` succeed.
- [ ] Update CI to run the verified Go build/test commands.

## Next Engineering Targets
- [ ] Replace placeholder Go `Engine.Run()` loop with a real Gio event/render loop.
- [ ] Restore real interactive window drag/ownership in Go using current Gio APIs.
- [ ] Validate the Go mesh stack end-to-end with actual peer instances.
- [ ] Decide which C++ components remain canonical until the Go renderer is feature-complete.

## Scope Corrections
- [x] Document that the shell/OS belongs to `bobfilez`.
- [ ] Remove or relocate shell-specific claims from remaining project docs where needed.

## Quality
- [ ] Add tests for `internal/kernel`, `internal/net`, and `internal/data` packages.
- [ ] Audit and normalize unrealistic historical changelog/version entries over time.
