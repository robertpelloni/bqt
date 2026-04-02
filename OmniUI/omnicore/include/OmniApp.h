#ifndef OMNIAPP_H
#define OMNIAPP_H

#include <QApplication>
#include <QString>
#include <QList>
#include <memory>

// Forward declarations
class JuceWidget;
class OmniInputManager;
class OmniDeveloperOverlay;
class QQmlApplicationEngine;
class OmniNativeEventFilter;

class OmniApplication : public QApplication {
    Q_OBJECT
public:
    OmniApplication(int &argc, char **argv);
    ~OmniApplication();

    // Bootstraps JUCE without starting its modal loop
    void initializeJuce();

    // Loads the main UI script (transpiled JS from TS or QML directly)
    bool loadMainSource(const QString& sourcePath);

    // Creates and manages a JuceWidget instance (C++ side)
    JuceWidget* createJuceWidget(QWidget* parent = nullptr);

    // Toggles the developer overlay visibility
    void setDeveloperOverlayEnabled(bool enabled);

    // Access to the multi-input manager
    OmniInputManager* inputManager() const;

protected:
    // Core OS level event routing interception for multi-cursor support
    bool notify(QObject *receiver, QEvent *e) override;

private:
    bool m_juceInitialized;
    QList<JuceWidget*> m_managedWidgets;
    OmniDeveloperOverlay* m_developerOverlay;
    QQmlApplicationEngine* m_qmlEngine;
    OmniNativeEventFilter* m_nativeFilter;
};

#endif // OMNIAPP_H
