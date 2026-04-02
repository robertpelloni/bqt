# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits.
- [ ] **Git Sync:** Intelligently merge any pending feature branches from the remote if they emerge.

## Implementation Tasks (Phase 8 Focus)

### 1. Data & Persistence
- [ ] Implement `OmniDatabase.cpp`. Expose `Q_INVOKABLE void executeQuery(QString query)` and `QVariantList getResults()`. Use `QSqlDatabase`.
- [ ] Ensure SQLite linkage is added to CMake.

### 2. Networking (Native/WASM parity)
- [ ] Implement `OmniHttpClient.cpp`. Provide Promise-like or Signal-Slot architecture for async HTTP GET/POST requests.
- [ ] Implement `OmniWebSocket.cpp` for native real-time chat/collaboration routing.

### 3. Polish & Documentation
- [ ] Write exhaustive Unit Tests for `OmniCodeEditor` text insertion and backspacing logic.
- [ ] Ensure all properties are documented in `Manual.md`.
