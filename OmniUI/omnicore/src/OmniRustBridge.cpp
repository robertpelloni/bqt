#include "OmniRustBridge.h"

// If corrosion/cxx is successfully configured via CMake, OMNI_RUST_ENABLED will be defined
#ifdef OMNI_RUST_ENABLED
// #include "omnicore/rust/src/lib.rs.h" // The auto-generated CXX header
#endif

bool OmniRustBridge::registerDevice(const QString& id, const QString& name, const QString& type) {
#ifdef OMNI_RUST_ENABLED
    // return register_device_safe(id.toStdString(), name.toStdString(), type.toStdString());
    return true; // Stubbed for CXX integration
#else
    // Fallback if Rust is not compiled
    Q_UNUSED(id); Q_UNUSED(name); Q_UNUSED(type);
    return true;
#endif
}

bool OmniRustBridge::updateCursor(const QString& id, double x, double y) {
#ifdef OMNI_RUST_ENABLED
    // return update_cursor_safe(id.toStdString(), x, y);
    return true;
#else
    Q_UNUSED(id); Q_UNUSED(x); Q_UNUSED(y);
    return true;
#endif
}

int OmniRustBridge::getDeviceCount() {
#ifdef OMNI_RUST_ENABLED
    // return get_device_count();
    return 0;
#else
    return 0;
#endif
}
