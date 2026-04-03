#ifndef OMNITHEMEMANAGER_H
#define OMNITHEMEMANAGER_H

#include <QObject>
#include <QColor>

class OmniThemeManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(Theme currentTheme READ currentTheme NOTIFY themeChanged)

public:
    enum Theme {
        Dark,
        Light,
        Cyberpunk,
        LiquidGlass,
        Aetheria // The Antigravity Original
    };
    Q_ENUM(Theme)

    static OmniThemeManager* instance();

    Theme currentTheme() const;

    Q_INVOKABLE void setDarkMode();
    Q_INVOKABLE void setLightMode();
    Q_INVOKABLE void setCyberpunkMode();
    Q_INVOKABLE void setLiquidGlassMode();
    Q_INVOKABLE void setAetheriaMode();

    // Thematic Color Accessors
    QColor primary() const;
    QColor surface() const;
    QColor text() const;
    QColor accent() const;

signals:
    void themeChanged();

private:
    explicit OmniThemeManager(QObject *parent = nullptr);
    ~OmniThemeManager() override;

    Theme m_currentTheme;
};

#endif // OMNITHEMEMANAGER_H
