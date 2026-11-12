# Library Inventory

## Submodules
* `JUCE`: Audio and DSP framework (C++).
  * **Location:** `submodules/juce`
  * **Remote URL:** `https://github.com/juce-framework/JUCE`
  * **Branch / Version:** `master` (`d60b7a78030661574a615b586d9325ba7a7fa7dd` — JUCE 9.0.5)
  * **Integration:** Mapped via `BobQJuceHost`.
* `Ultimate++`: GUI framework (C++).
  * **Location:** `submodules/ultimatepp`
  * **Remote URL:** `https://github.com/ultimatepp/ultimatepp`
  * **Branch / Version:** `master` (`1a56dd3a4a02112563eb9e16f62f17bba8a0edd1`)
  * **Integration:** Mapped via `BobQUltimatePPHost` / `BobQUppComponentRegistry`.

## Go Dependencies
* `gioui.org` (v0.9.0): Primary Go UI rendering backend.
* `golang.org/x/image` (v0.41.0): Font/glyph and image rasterization support.
* `github.com/gorilla/websocket` (v1.5.3): Networking.
* `modernc.org/sqlite` (v1.48.1): Database management.

## Qt (Forked)
* `qtbase` components heavily modified and vendored as `BobUI6`/`BobQ`.
