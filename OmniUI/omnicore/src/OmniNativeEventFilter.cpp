#include "OmniNativeEventFilter.h"
#include "OmniInputManager.h"
#include <QDebug>
#include <QGuiApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

OmniNativeEventFilter::OmniNativeEventFilter(OmniInputManager* inputManager)
    : m_inputManager(inputManager),
      m_isRegistered(false)
{
}

OmniNativeEventFilter::~OmniNativeEventFilter() = default;

void OmniNativeEventFilter::registerRawInput() {
    if (m_isRegistered) return;

#ifdef Q_OS_WIN
    qDebug() << "OmniUI: Registering Win32 RAWINPUT devices for Multi-Cursor extraction...";
    
    RAWINPUTDEVICE Rid[2];

    // Keyboard
    Rid[0].usUsagePage = 0x01; 
    Rid[0].usUsage = 0x06; 
    Rid[0].dwFlags = RIDEV_INPUTSINK; // Capture even when in background (optional)
    Rid[0].hwndTarget = nullptr; // Or a specific HWND if available

    // Mouse
    Rid[1].usUsagePage = 0x01; 
    Rid[1].usUsage = 0x02; 
    Rid[1].dwFlags = 0;
    Rid[1].hwndTarget = nullptr;

    if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
        qCritical() << "OmniUI: Failed to register Win32 RAWINPUT devices! Error:" << GetLastError();
    } else {
        m_isRegistered = true;
        qDebug() << "OmniUI: Win32 RAWINPUT registered successfully.";
    }
#else
    qDebug() << "OmniUI: Raw input registration is only fully implemented for Windows (Win32) currently.";
#endif
}

bool OmniNativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
    Q_UNUSED(result);

#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
        MSG *msg = static_cast<MSG *>(message);

        if (msg->message == WM_INPUT) {
            UINT dwSize = 0;
            GetRawInputData((HRAWINPUT)msg->lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
            
            if (dwSize == 0) return false;

            QByteArray rawData;
            rawData.resize(dwSize);
            
            if (GetRawInputData((HRAWINPUT)msg->lParam, RID_INPUT, rawData.data(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
                return false;
            }

            RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.data());

            // Extract the unique hardware Handle (Device ID)
            QString deviceId = QString("hw-%1").arg(reinterpret_cast<quintptr>(raw->header.hDevice), 0, 16);

            if (raw->header.dwType == RIM_TYPEMOUSE) {
                m_inputManager->registerDevice(deviceId, "Hardware Mouse", "mouse");
                
                // In a full implementation, we accumulate raw->data.mouse.lLastX and lLastY
                // to move the specific cursor. For now, we just log that we caught it.
                // qDebug() << "OmniUI: Raw Mouse Input from" << deviceId;
            } 
            else if (raw->header.dwType == RIM_TYPEKEYBOARD) {
                m_inputManager->registerDevice(deviceId, "Hardware Keyboard", "keyboard");
                
                // Extract key data
                // USHORT vKey = raw->data.keyboard.VKey;
                // qDebug() << "OmniUI: Raw Keyboard Input from" << deviceId << "Key:" << vKey;
            }
        }
    }
#else
    Q_UNUSED(eventType);
    Q_UNUSED(message);
#endif

    return false; // Return false to let Qt continue processing the event natively
}
