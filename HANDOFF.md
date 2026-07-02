# Session Handoff & Memory State

## Current Context
We are systematically evolving BQt towards an interoperable engine crossing C++, Go, Rust, Java, and C#.
The immediate goals completed in this session:
1. Rebranded core references from `BobUI` and `qt` to `BQt` across codebase and documentation. Clarified that BQt is a framework kernel and not an OS shell (which is the separate `bobfilez` project).
2. Extensively updated the Go `DemoSurface` dashboard to visibly expose all backend features including Audio Graph, Synthesizer, Event Loop, UndoStack, Clipboard Sync, TimeMachine, MeshNode, and SearchEngine via interactive buttons, popups, and informative tooltips in an organized grid layout.

## What the next LLM should know:
- **Massive Renaming Warning**: We ran a targeted script to switch `BobUI` / `bobui` to `BQt` / `bqt` inside the Go backend (`internal/` and `main.go`) and documentation files. A broader regex replacement across the entire C++ repo was avoided due to the immense diff size causing code review failures and instability.
- Version is now `v1.1.75`.
- Never commit binary files (like `.class` files or `/obj/`, `/bin/` folders in C#) or output executables (like the `bqt` binary). Ensure robust `.gitignore` protections are applied when establishing new language ecosystems.

## Active Priorities:
1. Continue methodical 1:1 porting of remaining C++ components to Go.
