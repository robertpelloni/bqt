#include "OmniThemeManager.h"
#include <QDebug>

OmniThemeManager* OmniThemeManager::instance() {
    static OmniThemeManager* manager = new OmniThemeManager();
    return manager;
}

OmniThemeManager::OmniThemeManager(QObject *parent) : QObject(parent), m_currentTheme(Cyberpunk) {}
OmniThemeManager::~OmniThemeManager() = default;

OmniThemeManager::Theme OmniThemeManager::currentTheme() const { return m_currentTheme; }

void OmniThemeManager::setDarkMode() { m_currentTheme = Dark; emit themeChanged(); }
void OmniThemeManager::setLightMode() { m_currentTheme = Light; emit themeChanged(); }
void OmniThemeManager::setCyberpunkMode() { m_currentTheme = Cyberpunk; emit themeChanged(); }
void OmniThemeManager::setLiquidGlassMode() { m_currentTheme = LiquidGlass; emit themeChanged(); }
void OmniThemeManager::setAetheriaMode() { m_currentTheme = Aetheria; emit themeChanged(); }

QColor OmniThemeManager::primary() const {
    switch(m_currentTheme) {
        case Cyberpunk: return QColor("#00F0FF"); // Neon Cyan
        case LiquidGlass: return QColor(255, 255, 255, 180);
        case Aetheria: return QColor("#D4AF37"); // Cosmic Gold
        default: return QColor("#0078D7");
    }
}

QColor OmniThemeManager::surface() const {
    switch(m_currentTheme) {
        case Cyberpunk: return QColor(10, 10, 15, 220); // Semi-transparent Vantablack
        case LiquidGlass: return QColor(255, 255, 255, 30); // Ultra-translucency
        case Aetheria: return QColor(26, 11, 46, 240); // Nebula Purple
        default: return QColor(30, 30, 30);
    }
}

QColor OmniThemeManager::text() const {
    return (m_currentTheme == Light) ? Qt::black : Qt::white;
}

QColor OmniThemeManager::accent() const {
    return (m_currentTheme == Cyberpunk) ? QColor("#FF003C") : QColor("#569CD6");
}
