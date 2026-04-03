#include "OmniThemeManager.h"
#include <QDebug>

OmniThemeManager* OmniThemeManager::instance() {
    static OmniThemeManager* manager = new OmniThemeManager();
    return manager;
}

OmniThemeManager::OmniThemeManager(QObject *parent) : QObject(parent) {
    setObjectName("OmniThemeManager");
    setDarkMode(); // Default
}

OmniThemeManager::~OmniThemeManager() = default;

OmniThemeManager::Theme OmniThemeManager::currentTheme() const { return m_currentTheme; }
QColor OmniThemeManager::primaryColor() const { return m_primary; }
QColor OmniThemeManager::secondaryColor() const { return m_secondary; }
QColor OmniThemeManager::backgroundColor() const { return m_background; }
QColor OmniThemeManager::surfaceColor() const { return m_surface; }
QColor OmniThemeManager::textColor() const { return m_text; }
QColor OmniThemeManager::dangerColor() const { return m_danger; }
QColor OmniThemeManager::successColor() const { return m_success; }

void OmniThemeManager::setDarkMode() {
    m_currentTheme = Dark;
    m_primary = QColor("#0078D7");
    m_secondary = QColor("#005A9E");
    m_background = QColor("#111111");
    m_surface = QColor("#1E1E1E");
    m_text = QColor("#FFFFFF");
    m_danger = QColor("#D32F2F");
    m_success = QColor("#388E3C");
    emit themeChanged();
}

void OmniThemeManager::setLightMode() {
    m_currentTheme = Light;
    m_primary = QColor("#0078D7");
    m_secondary = QColor("#005A9E");
    m_background = QColor("#F3F3F3");
    m_surface = QColor("#FFFFFF");
    m_text = QColor("#000000");
    m_danger = QColor("#D32F2F");
    m_success = QColor("#388E3C");
    emit themeChanged();
}

void OmniThemeManager::setCyberpunkMode() {
    m_currentTheme = Cyberpunk;
    m_primary = QColor("#FCEE09"); // Cyberpunk Yellow
    m_secondary = QColor("#00F0FF"); // Neon Cyan
    m_background = QColor(11, 11, 12, 230); // Deep transparent Vantablack
    m_surface = QColor(26, 26, 29, 180); // Transparent Gunmetal
    m_text = QColor("#FF003C"); // Neon Pink/Red
    m_danger = QColor("#FF0000");
    m_success = QColor("#00FF00");
    emit themeChanged();
}

void OmniThemeManager::setLiquidGlassMode() {
    m_currentTheme = LiquidGlass;
    m_primary = QColor(255, 255, 255, 180); // Frosted White
    m_secondary = QColor(200, 200, 255, 100); // Light blue tint
    m_background = QColor(10, 20, 30, 80); // Highly transparent dark water
    m_surface = QColor(255, 255, 255, 30); // Barely visible glass pane
    m_text = QColor("#E0F7FA"); // Pale Cyan text
    m_danger = QColor(255, 50, 50, 180);
    m_success = QColor(50, 255, 150, 180);
    emit themeChanged();
}

void OmniThemeManager::setChronosShiftMode() {
    m_currentTheme = ChronosShift;
    m_primary = QColor("#FFD700"); // Master Clock Gold
    m_secondary = QColor("#B8860B"); // Dark Goldenrod
    m_background = QColor("#120024"); // Deep Cosmic Indigo
    m_surface = QColor("#2D004D"); // Elevated Purple
    m_text = QColor("#FFF8DC"); // Light Goldenrod text
    m_danger = QColor("#FF4500"); // Orange Red
    m_success = QColor("#32CD32"); // Lime Green
    emit themeChanged();
}
