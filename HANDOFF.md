# Session Handoff & Memory State

## Current Context
We are systematically evolving BobQ towards an interoperable engine crossing C++, Go, Rust, Java, and C#.
The immediate goals completed in this session:
1. Rebranded core references from `bobui` and `qt` to `bqt`.
2. Ported the `OmniAudioGraph`, `OmniGain`, and `OmniSynthesizer` components to Go, Rust, Java, and C# maintaining strict 1:1 cross-language feature parity.
3. Implemented native Go routines to handle BQt signal/slot emission seamlessly.
4. Mapped JUCE audio primitives into the Go `AudioGraph` via `JUCEAudioGraphAdapter`.
5. Fixed CI pipeline WebAssembly targets and Qt dependency installations.
6. Stabilized the Go Package Graph (`go mod tidy` / tests pass).
7. Implemented the unified BQt Event Loop (`internal/ui/event_loop.go`) to orchestrate tasks across sub-frameworks.

## What the next LLM should know:
- **Massive Renaming Warning**: We ran a targeted script to switch `BobUI` / `bobui` to `BQt` / `bqt` inside the Go backend (`internal/` and `main.go`) and specific CI/cmake scripts. A broader regex replacement across the entire C++ repo was avoided due to the immense diff size causing code review failures and instability.
- Version is now `v1.1.75`.
- Never commit binary files (like `.class` files or `/obj/`, `/bin/` folders in C#). Ensure robust `.gitignore` protections are applied when establishing new language ecosystems.

## Active Priorities:
1. Finish linking `OmniAudioGraph` and `OmniSynthesizer` natively in Go.
3. Finish bridging the Ultimate++ elements via `BobQUppComponentRegistry`.
