# BobQ / OmniUI Submodule Dashboard

This document lists all active submodules, their remote URLs, tracking branches, and pinned commit versions in the BQt (`submodules/bobui`) repository.

## Active Submodules

| Submodule Path | Remote URL | Branch | Commit SHA | Notes |
|---|---|---|---|---|
| `submodules/juce` | `https://github.com/juce-framework/JUCE` | `master` | `d60b7a78030661574a615b586d9325ba7a7fa7dd` | JUCE 9.0.5 — Audio / DSP / Component UI framework utilized via `BobQJuceHost` |
| `submodules/ultimatepp` | `https://github.com/ultimatepp/ultimatepp` | `master` | `1a56dd3a4a02112563eb9e16f62f17bba8a0edd1` | Rapid Application Development C++ toolkit utilized via `BobQUppComponentRegistry` |

## Architecture Directories

- `BobQ/` & `OmniUI/`: The core C++ Native Engine (Qt/Meta-Object driven) and Omni component suites.
- `internal/`: The pure Go rewrite, unified event loop, audio DSP graph, and widget bindings engine (`main.go`).
- `docs/`: Universal AI instructions and architectural deep dives.
- `scripts/`: Ecosystem automation scripts.
