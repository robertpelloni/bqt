#ifndef OMNITHEMEMANAGER_H
#define OMNITHEMEMANAGER_H

#include <QObject>
#include <QColor>

class OmniThemeManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(Theme currentTheme READ currentTheme NOTIFY themeChanged)
    Q_PROPERTY(QColor primaryColor READ primaryColor NOTIFY themeChanged)
    Q_PROPERTY(QColor secondaryColor READ secondaryColor NOTIFY themeChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor surfaceColor READ surfaceColor NOTIFY themeChanged)
    Q_PROPERTY(QColor textColor READ textColor NOTIFY themeChanged)

public:
    enum Theme {
        Dark,
        Light,
        Cyberpunk,
        LiquidGlass,
        ChronosShift // The Custom AI Theme
    };
    Q_ENUM(Theme)

    static OmniThemeManager* instance();

    Theme currentTheme() const;
    QColor primaryColor() const;
    QColor secondaryColor() const;
    QColor backgroundColor() const;
    QColor surfaceColor() const;
    QColor textColor() const;
    QColor dangerColor() const;
    QColor successColor() const;

    Q_INVOKABLE void setDarkMode();
    Q_INVOKABLE void setLightMode();
    Q_INVOKABLE void setCyberpunkMode();
    Q_INVOKABLE void setLiquidGlassMode();
    Q_INVOKABLE void setChronosShiftMode();

signals:
    void themeChanged();

private:
    explicit OmniThemeManager(QObject *parent = nullptr);
    ~OmniThemeManager() override;

    Theme m_currentTheme;
    QColor m_primary;
    QColor m_secondary;
    QColor m_background;
    QColor m_surface;
    QColor m_text;
    QColor m_danger;
    QColor m_success;
};

#endif // OMNITHEMEMANAGER_H
