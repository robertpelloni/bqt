#ifndef OMNIAPP_H
#define OMNIAPP_H

#include <QApplication>
#include <QString>
#include <QList>
#include <memory>

class JuceWidget;
class OmniInputManager;
class OmniDeveloperOverlay;

class OmniApplication : public QApplication {
    Q_OBJECT
public:
    OmniApplication(int &argc, char **argv);
    ~OmniApplication();

    // Bootstraps JUCE without starting its modal loop
    void initializeJuce();

    // Loads the main UI script (transpiled JS from TS)
    bool loadMainSource(const QString& sourcePath);

    // Creates and manages a JuceWidget instance
    JuceWidget* createJuceWidget(QWidget* parent = nullptr);

    // Toggles the developer overlay visibility
    void setDeveloperOverlayEnabled(bool enabled);

    // Access to the multi-input manager
    OmniInputManager* inputManager() const;

private:
    bool m_juceInitialized;
    QList<JuceWidget*> m_managedWidgets;
    OmniDeveloperOverlay* m_developerOverlay;
};

#endif // OMNIAPP_H