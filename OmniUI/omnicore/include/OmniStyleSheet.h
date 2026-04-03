#ifndef OMNISTYLESHEET_H
#define OMNISTYLESHEET_H

#include <QString>
#include <QColor>
#include <QMap>
#include <QFont>

// A JavaFX/CSS-inspired inline styling engine for OmniUI Widgets
class OmniStyleSheet {
public:
    OmniStyleSheet();
    ~OmniStyleSheet();

    void parse(const QString& styleString);

    bool hasProperty(const QString& key) const;
    QString getString(const QString& key, const QString& fallback = "") const;
    QColor getColor(const QString& key, const QColor& fallback = Qt::transparent) const;
    int getInt(const QString& key, int fallback = 0) const;
    double getDouble(const QString& key, double fallback = 0.0) const;

    // Generates a QFont based on parsed -omni-font-family, -omni-font-size, etc.
    QFont getFont(const QFont& baseFont) const;

private:
    QMap<QString, QString> m_properties;
};

#endif // OMNISTYLESHEET_H
