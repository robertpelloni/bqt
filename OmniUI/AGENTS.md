# AGENT INSTRUCTIONS FOR OmniUI FRAMEWORK

You are developing the OmniUI framework, a massive Qt/QML/C++ hybrid simulation runtime designed to encapsulate everything from quantum physics and hardware emulation to biological tracking and universe generation.

**CRITICAL DIRECTIVES:**

1. **Module Scope**: There are 30+ highly developed modules inside `OmniUI/core/src` and `OmniUI/core/include`. They are registered in `OmniUI/core/include/OmniQmlRegister.h`. Do not create new modules. Your job is to debug, integrate, or enhance existing ones.
2. **Implementation Style**:
    - **C++**: The backend uses deep state-machines and physics loops (usually driven by `QTimer`). All properties exposed to the QML UI must use `Q_PROPERTY` with appropriate `NOTIFY` signals.
    - **QML**: The frontend uses `QtQuick 2.15` and `QtQuick.Controls 2.15`. Always build modern, dark-themed (Slate/Zinc palette) dashboards with extreme detail, tooltips, dynamic progress bars, and custom `<Canvas>` components.
3. **Data Logging**: All systems should maintain a `QVariantList` log exposed via `Q_PROPERTY` so the QML can build scrolling history views (ListView).
4. **Tool Use Strategy**: When tasked to modify a module (e.g. OmniKernel), aggressively use `grep` and `cat` to understand the header files first. Then view the `.cpp` file, then view the corresponding `assets/main.qml` file inside `OmniUI/apps/` or `OmniUI/examples/`.

**The Master Clock Problem (v42.0+ Focus)**
The primary goal moving forward is **integration**. The individual modules currently rely on their own internal `QTimer` instances for simulation ticks. You must refactor them to listen to a central `OmniSimulationEngine::instance()->tick()` signal, ensuring the entire framework operates on one unified timeline.

If you are asked to implement a new feature, always document it heavily in `OmniUI/Manual.md`.
