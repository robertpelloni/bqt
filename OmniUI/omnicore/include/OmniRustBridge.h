#ifndef OMNIRUSTBRIDGE_H
#define OMNIRUSTBRIDGE_H

#include <QString>
#include <QDebug>

// This class acts as the C++ API wrapper around the CXX-generated Rust bindings.
// If the Rust toolchain is missing during compilation, it provides safe C++ fallbacks.

class OmniRustBridge {
public:
    static bool registerDevice(const QString& id, const QString& name, const QString& type);
    static bool updateCursor(const QString& id, double x, double y);
    static int getDeviceCount();
};

#endif // OMNIRUSTBRIDGE_H
