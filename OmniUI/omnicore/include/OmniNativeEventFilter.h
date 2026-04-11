#ifndef OMNINATIVEEVENTFILTER_H
#define OMNINATIVEEVENTFILTER_H

#include <QAbstractNativeEventFilter>
#include <QObject>

class OmniInputManager;

class OmniNativeEventFilter : public QAbstractNativeEventFilter {
public:
    explicit OmniNativeEventFilter(OmniInputManager* inputManager);
    ~OmniNativeEventFilter() override;

    // Initializes the OS-specific raw input registration
    void registerRawInput();

    // The core interception hook for OS messages before Qt processes them
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

private:
    OmniInputManager* m_inputManager;
    bool m_isRegistered;
};

#endif // OMNINATIVEEVENTFILTER_H
