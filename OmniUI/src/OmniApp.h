#ifndef OMNIAPP_H
#define OMNIAPP_H

#include <QApplication>
#include <QString>

class OmniApplication : public QApplication {
    Q_OBJECT
public:
    OmniApplication(int &argc, char **argv);
    ~OmniApplication();

    // Bootstraps JUCE without starting its modal loop
    void initializeJuce();

    // Loads the main UI script (transpiled JS from TS)
    bool loadMainSource(const QString& sourcePath);

private:
    bool m_juceInitialized;
};

#endif // OMNIAPP_H