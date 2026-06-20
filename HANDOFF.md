# Session Handoff & Memory State

## Current Context
We are systematically evolving BobQ towards an interoperable engine crossing C++, Go, and Rust.
The immediate goals completed in this session:
1. Rebranded core references from `bobui` and `qt` to `bqt`.
2. Progressed the Go port of `OmniAudioGraph` by adding `Start()` and `Stop()` logic mirroring C++.

## What the next LLM should know:
- **Massive Renaming Warning**: We ran a targeted script to switch `BobUI` / `bobui` to `BQt` / `bqt` inside the Go backend (`internal/` and `main.go`). A broader regex replacement across the entire repo was avoided due to the immense diff size causing code review failures and instability.
- The Go audio port (priority 1) in `internal/audio/` continues to expand. `OmniAudioGraph` now has start/stop/parallelism stubs.
- Version is now `v1.1.67`.

## Active Priorities:
1. Finish linking `OmniAudioGraph` and `OmniSynthesizer` in Go.
2. Maintain GTK parity.
