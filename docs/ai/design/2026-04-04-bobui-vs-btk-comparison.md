# BobUI vs BTK Comparative Analysis

## Date
2026-04-04

## Scope
This document compares the active Go-based `bobui` framework track in this repository against the current `BTK` repository at `https://github.com/robertpelloni/btk`.

The goal is not hype or repo vanity. The goal is to determine:
- where `bobui` is genuinely stronger
- where `BTK` is genuinely stronger
- which ideas should stay experimental in `bobui`
- which ideas are better realized in `BTK`
- how the two projects should relate strategically

## Sources Inspected
### BobUI
- Local repo: `C:/Users/hyper/workspace/bobui`
- Branch: `main`
- Commit: `f96d69f084edd3397a57d67f19f9b9771fe7c43b`
- Verified commands:
  - `go test ./internal/...`
  - `go build -buildvcs=false .`

### BTK
- Repo: `https://github.com/robertpelloni/btk`
- Branch: `master`
- Commit: `a9d1206d79beb4fb0c0cd84adb4ceab2bbfb142e`
- Primary materials inspected:
  - `README.md`
  - `CMakeLists.txt`
  - `.gitmodules`
  - `docs/ai/...`
  - `src/core/kernel/btkinputowner.h`
  - `src/core/kernel/btkfocustoken.h`
  - `src/gui/kernel/btkinputarbitrator.h`

## Executive Summary
BTK is currently the stronger native framework substrate by a wide margin. It already has the shape, scale, packaging, and module breadth of a serious C++ application framework derived from CopperSpice/Qt.

BobUI is currently the stronger experimental framework narrative. It is clearer about:
- framework-vs-shell boundaries
- ownership and collaboration semantics
- permission and synchronization language
- lightweight modern host/page bridge ideas
- parity work as a documented, staged process rather than only a broad implementation claim

The correct synthesis is:
- use **BTK** as the heavy native framework path
- use **BobUI** as the fast idea incubator and requirements sketchpad
- port only the best validated abstractions from BobUI into BTK

## Scale Snapshot
### BTK
Approximate active `src/` source footprint inspected:
- ~12,854 source files
- ~3,908,222 lines

### BobUI
Approximate active `internal/` Go source footprint inspected:
- 99 `.go` files
- ~4,523 lines

## Interpretation
This is not a close match in implementation depth. BTK is already a very large framework tree. BobUI's active, verified Go path is much smaller and should be treated as a compact experimental framework layer.

## Architecture Comparison
### BTK
Observed source areas:
- `src/core`
- `src/gui`
- `src/network`
- `src/sql`
- `src/svg`
- `src/opengl`
- `src/vulkan`
- `src/webkit`
- `src/xml`
- `src/xmlpatterns`
- `src/declarative`
- `src/plugins`
- `src/tools`

### BobUI
Observed Go source areas:
- `internal/audio`
- `internal/data`
- `internal/kernel`
- `internal/net`
- `internal/state`
- `internal/ui`
- `internal/vm`

## Interpretation
- BTK already resembles a broad native application framework.
- BobUI resembles a framework kernel and experimental systems lab.
- BobUI should not pretend to outrank BTK in mature runtime breadth today.
- BTK should not ignore BobUI's stronger future-facing abstractions.

## Build and Migration Strategy
### BTK strengths
BTK already implements a staged branding and compatibility model:
- preserves CopperSpice compatibility exports
- adds `BTK` package/config files
- exports alias targets such as `BtkCore` and `BTK::Core`
- avoids a destructive one-shot rename

This is the right strategy for a large native framework.

### BobUI strengths
BobUI's Go path is easier to iterate and verify quickly:
- simpler build loop
- smaller implementation surface
- clearer compile-safe baseline workflow

### Finding
BTK is stronger in packaging, migration discipline, and native framework build strategy.

## Multi-User and Ownership Semantics
### BobUI strengths
BobUI has been explicit about:
- multi-user interaction
- device identity
- permission locking
- user/device mapping
- sync/reconciliation language
- framework-vs-shell separation

Relevant files include:
- `internal/kernel/input_manager.go`
- `internal/kernel/permission_manager.go`
- `internal/kernel/user_manager.go`
- `internal/data/history_sync.go`

### BTK strengths
BTK has started translating these ideas into stronger framework-native C++ APIs:
- `BtkInputOwner`
- `BtkFocusToken`
- `BtkInputArbitrator`

These types encode:
- ownership identity
- session identity
- device grouping
- capability flags
- focus scope
- modality policy
- routing decisions

### Finding
BobUI contributed stronger framing. BTK now has the stronger typed substrate.

## Web Runtime Comparison
### BobUI strengths
BobUI's current Go `WebView` provides a clean modern bridge contract:
- `OnNavigate`
- `OnLoad`
- `OnTitleChanged`
- `OnHistoryChanged`
- `OnScriptMessage`
- `EvalJS(...)`
- `PostMessage(...)`
- `Request(...)`
- `RegisterHandler(...)`
- `BridgeContractJSON()`

This is not a full browser engine, but it is a useful contract model inspired by modern WebEngineQuick-style embedding.

### BTK strengths
BTK already has a real legacy web stack under `src/webkit`, which is far broader than BobUI's current WebView baseline.

### Gap
BTK's own docs correctly acknowledge that legacy WebKit is not the same as a verified modern WebEngineQuick-class runtime.

### Finding
- BTK has greater historical web depth.
- BobUI currently has the cleaner modern bridge abstraction.
- BTK should adopt the bridge concepts, but pair them with a newer engine strategy than legacy WebKit alone.

## Widget and Runtime Surface
### BTK strengths
BTK inherits a broad native framework surface and remains much closer to a real Qt-class framework in scope.

### BobUI strengths
BobUI's current Go widget work is useful as a parity baseline and experimental layer:
- `Popup`
- `Dialog`
- `Drawer`
- `ToolTip`
- `Menu`
- `ScrollView`
- `ScrollBar`
- `WebView`
- `DemoSurface`

### Finding
BobUI's widget set is useful as a compile-safe requirements sketch, not as proof of runtime parity.

## Documentation and Strategic Clarity
BTK already includes its own AI planning documents explicitly analyzing BobUI and treating it as a reference input rather than a peer implementation baseline. This is a good sign.

BTK's internal docs independently reached several conclusions that match this analysis:
- BobUI is strategically valuable for ownership and collaboration ideas.
- BobUI should not be copied wholesale.
- BTK should absorb the strongest concepts into a more rigorous C++ substrate.

## Adoption Matrix
| Capability / Idea | BobUI Status | BTK Status | Recommendation |
|---|---|---|---|
| Framework vs shell separation | Clear and explicit | Less central but compatible | Preserve in both; keep shell concerns out of framework repos |
| Multi-user ownership framing | Strong | Becoming strong | Keep BobUI as idea lab; implement rigorously in BTK |
| Typed focus/input arbitration | Early/simple in Go | Stronger typed C++ scaffolding | Prefer BTK as the long-term home |
| Web bridge abstraction | Strong concept | Not yet modernized | Port BobUI-style bridge ideas into BTK |
| Traditional framework breadth | Weak/partial | Strong | Use BTK as native framework mainline |
| Rapid experimentation | Strong | Slower due to scale | Continue using BobUI as a fast prototyping lane |
| Verified parity discipline | Improving | Better overall substrate discipline | Keep tightening both repos around testable claims |

## Main Risks
### BobUI risks
- code/docs drift
- parity language outrunning runtime truth
- too many surfaces added before deeper runtime integration
- noisy repository state caused by unrelated large trees

### BTK risks
- legacy architecture gravity
- modernization gaps around Quick/WebEngine-like runtime behavior
- high integration cost for radical new ownership semantics
- migration complexity while preserving compatibility

## Strategic Recommendation
### Role of BTK
BTK should be treated as the primary long-term native framework track because it already has:
- serious C++ depth
- broad modularity
- packaging and aliasing strategy
- stronger framework-grade API surfaces

### Role of BobUI
BobUI should be treated as:
- a fast experimental framework track
- a collaboration/ownership semantics incubator
- a host/page bridge design lab
- a parity-audit and requirements reference

### Transfer rules
BobUI ideas should be promoted into BTK only when they are:
1. clearly scoped
2. internally coherent
3. build-verified or at least behaviorally validated
4. expressed as framework concepts rather than shell/product UX

## Concrete Next Steps for BobUI
1. Keep the Go baseline honest and compile-verified.
2. Implement runtime-integrated `ScrollBar` and `ScrollView` behavior next.
3. Add touch/swipe primitives.
4. Evolve the `WebView` bridge from contract to executable runtime semantics.
5. Continue documenting parity as verified stages rather than broad completion claims.

## Concrete Next Steps for BTK
1. Push `BtkInputOwner`, `BtkFocusToken`, and `BtkInputArbitrator` deeper into real event routing.
2. Add owner-scoped modal and popup policies.
3. Define a modern web embedding plan beyond legacy WebKit.
4. Use BobUI's bridge contract as input for BTK's embedded web API design.
5. Continue staged BTK branding without reckless repository-wide rename damage.

## Bottom Line
BTK and BobUI should not be treated as duplicate efforts fighting for the same role.

The healthier interpretation is:
- **BTK** = implementation-heavy native framework mainline
- **BobUI** = smaller, faster experimental framework and abstraction incubator

That division lets BobUI stay inventive without pretending to be more complete than it is, and lets BTK evolve beyond its CopperSpice legacy without losing the benefits of its mature substrate.
