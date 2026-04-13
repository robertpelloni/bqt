#include "OmniStyleSheet.h"
#include <QStringList>
#include <QDebug>

OmniStyleSheet::OmniStyleSheet() = default;
OmniStyleSheet::~OmniStyleSheet() = default;

void OmniStyleSheet::parse(const QString& styleString) {
    m_properties.clear();
    if (styleString.isEmpty()) return;

    // Simple parser for "-omni-background-color: #FF0000; -omni-border-radius: 5px;"
    QStringList declarations = styleString.split(';', BobUI::SkipEmptyParts);
    for (const QString& decl : declarations) {
        int colonIdx = decl.indexOf(':');
        if (colonIdx > 0) {
            QString key = decl.left(colonIdx).trimmed().toLower();
            QString value = decl.mid(colonIdx + 1).trimmed();
            m_properties[key] = value;
        }
    }
}

bool OmniStyleSheet::hasProperty(const QString& key) const {
    return m_properties.contains(key);
}

QString OmniStyleSheet::getString(const QString& key, const QString& fallback) const {
    return m_properties.value(key, fallback);
}

QColor OmniStyleSheet::getColor(const QString& key, const QColor& fallback) const {
    if (!m_properties.contains(key)) return fallback;
    QString val = m_properties[key];
    if (QColor::isValidColorName(val)) {
        return QColor(val);
    }
    return fallback;
}

int OmniStyleSheet::getInt(const QString& key, int fallback) const {
    if (!m_properties.contains(key)) return fallback;
    QString val = m_properties[key];
    val.remove("px"); // naive unit strip
    bool ok;
    int parsed = val.toInt(&ok);
    return ok ? parsed : fallback;
}

double OmniStyleSheet::getDouble(const QString& key, double fallback) const {
    if (!m_properties.contains(key)) return fallback;
    QString val = m_properties[key];
    val.remove("px");
    val.remove("em");
    bool ok;
    double parsed = val.toDouble(&ok);
    return ok ? parsed : fallback;
}

QFont OmniStyleSheet::getFont(const QFont& baseFont) const {
    QFont f = baseFont;
    if (hasProperty("-omni-font-family")) {
        f.setFamily(getString("-omni-font-family"));
    }
    if (hasProperty("-omni-font-size")) {
        f.setPixelSize(getInt("-omni-font-size", f.pixelSize()));
    }
    if (hasProperty("-omni-font-weight")) {
        QString w = getString("-omni-font-weight");
        if (w == "bold") f.setBold(true);
        else f.setWeight(w.toInt());
    }
    if (hasProperty("-omni-font-style") && getString("-omni-font-style") == "italic") {
        f.setItalic(true);
    }
    return f;
}
