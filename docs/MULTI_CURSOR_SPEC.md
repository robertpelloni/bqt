# Multi-Cursor & Multi-Input Specification: BobUI OS

## 1. Objective
Enable a true multi-user, collaborative operating system experience where multiple independent cursors and input streams (keyboard/gamepad) coexist on the same workspace, whether local or remote.

## 2. Core Concepts

### 2.1 Input Device Identity
Every input event (mouse, touch, key, tablet) must be tagged with a unique `DeviceId`.
- **Qt 6 Foundation**: Leverage `QInputDevice` and `QPointingDevice`.
- **Extension**: Ensure every `QInputEvent` carries a valid `QInputDevice` pointer that correctly identifies the physical or virtual source.

### 2.2 Independent Focus Trees
Standard UI frameworks assume a single "active" focus. BobUI must support per-device (or per-user) focus.
- **Implementation**: `OmniApplication` will maintain a mapping of `DeviceId -> FocusedWidget`.
- **Event Dispatch**: `QCoreApplication::notify` will be extended to route events based on the `DeviceId` rather than a global focus.

### 2.3 Visual Representation (The Cursor Overlay)
Since the system cursor is usually singular, BobUI will render "Virtual Cursors".
- **OmniCursorOverlay**: A high-priority rendering layer (using Dear ImGui or a top-level QQuickItem) that draws cursors for every registered `QPointingDevice`.
- **Aesthetics**: Cursors will be color-coded and labeled per user/device.

## 3. Implementation Roadmap

### Phase 1: HID Enumeration & Raw Input
- **Windows**: Use `RegisterRawInputDevices` to capture independent mouse streams.
- **Linux (Wayland/X11)**: Use `libinput` directly or a custom compositor protocol to distinguish seats/devices.

### Phase 2: Event Routing Extension
- Modify `QGuiApplicationPrivate::processMouseEvent` to avoid clobbering the global mouse state.
- Introduce `OmniInputEvent` as a wrapper or extension to standard events.

### Phase 3: Collaboration API
- Expose the multi-input state to TypeScript via the `Omni` namespace.
- `Omni.onDeviceConnected((device) => { ... })`
- `Omni.getCursorPosition(deviceId)`

## 4. Hardware/Software Synergy
The goal is to allow a single PC with 4 mice and 4 keyboards to act as 4 independent workstations on one or more monitors, seamlessly transitioning into an online multiplayer/collaborative environment.
