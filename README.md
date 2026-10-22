# BQt

BQt (formerly BobUI) is an experimental cross-platform UI framework kernel.
The repository is built on top of a **full Qt Base source tree** (that's what
`src/`, `mkspecs/`, `configure`, `qmake/`, `coin/` and `QtBase.slnx` are),
with two layers of original work on top of it:

1. **C++ "Omni" components** — a large set of custom widgets, layouts,
   rendering nodes and an audio/DSP stack (OmniButton, OmniDockArea,
   OmniAudioGraph, OmniSynthesizer, OmniSequencer, a JUCE host bridge —
   `BobQJuceHost`, …) under `BobQ/` and `OmniUI/`. `ANALYSIS.md` inventories
   them and tracks parity against the Go port.
2. **A Go port intended to become the primary runtime** — `main.go`,
   `go.mod`, and `internal/` (including `internal/ui/widgets/` and the
   unified event loop). Per `VISION.md`, the direction is a Go-first kernel
   with a unified event loop, signal/slot semantics mirroring Qt queued
   connections, and the `OmniAudioGraph` DSP graph, progressively replacing
   the C++ render bridges. JUCE and Ultimate++ are integrated as submodules
   (see `SUBMODULE_INVENTORY.md`).

Scope note: BQt is the framework kernel only. The OS shell that consumes it
is the separate `bobfilez` project.

## Building

The C++ tree is Qt Base, so it builds like Qt:

```bash
./configure   # or configure.bat on Windows
cmake --build .
```

The Go port builds independently from the repo root:

```bash
go build ./...
```

Neither path is packaged or released; expect a very large tree (~317 MB)
with build artifacts, submodule checkouts, and agent session files mixed in.

## Status

Alpha, under active agent-driven development (latest commits October 2026
are submodule-pointer fixes and agent "protocol" checkpoints). `VISION.md`
describes the end state — hot-swappable rendering frontends over a pure Go
kernel — as a goal, not the current reality: the C++ base is still the bulk
of the tree and the Go port is partial (see the parity gaps in
`ANALYSIS.md`). The README this replaces had been committed with unresolved
merge-conflict markers in it, which is a fair summary of the repo's current
level of polish.
