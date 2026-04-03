#ifndef OMNITHEMEMANAGER_H
#define OMNITHEMEMANAGER_H

#include <QObject>
#include <QColor>

class OmniThemeManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor primaryColor READ primaryColor WRITE setPrimaryColor NOTIFY themeChanged)
    Q_PROPERTY(QColor secondaryColor READ secondaryColor WRITE setSecondaryColor NOTIFY themeChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor surfaceColor READ surfaceColor WRITE setSurfaceColor NOTIFY themeChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor dangerColor READ dangerColor WRITE setDangerColor NOTIFY themeChanged)
    Q_PROPERTY(QColor successColor READ successColor WRITE setSuccessColor NOTIFY themeChanged)

public:
    static OmniThemeManager* instance();

    QColor primaryColor() const;
    void setPrimaryColor(const QColor& color);

    QColor secondaryColor() const;
    void setSecondaryColor(const QColor& color);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& color);

    QColor surfaceColor() const;
    void setSurfaceColor(const QColor& color);

    QColor textColor() const;
    void setTextColor(const QColor& color);

    QColor dangerColor() const;
    void setDangerColor(const QColor& color);

    QColor successColor() const;
    void setSuccessColor(const QColor& color);

    Q_INVOKABLE void setDarkMode();
    Q_INVOKABLE void setLightMode();
    Q_INVOKABLE void setCyberpunkMode();

signals:
    void themeChanged();

private:
    explicit OmniThemeManager(QObject *parent = nullptr);
    ~OmniThemeManager() override;

    QColor m_primary;
    QColor m_secondary;
    QColor m_background;
    QColor m_surface;
    QColor m_text;
    QColor m_danger;
    QColor m_success;
};

#endif // OMNITHEMEMANAGER_H
