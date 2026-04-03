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

QColor OmniThemeManager::primaryColor() const { return m_primary; }
void OmniThemeManager::setPrimaryColor(const QColor& color) {
    if (m_primary != color) { m_primary = color; emit themeChanged(); }
}

QColor OmniThemeManager::secondaryColor() const { return m_secondary; }
void OmniThemeManager::setSecondaryColor(const QColor& color) {
    if (m_secondary != color) { m_secondary = color; emit themeChanged(); }
}

QColor OmniThemeManager::backgroundColor() const { return m_background; }
void OmniThemeManager::setBackgroundColor(const QColor& color) {
    if (m_background != color) { m_background = color; emit themeChanged(); }
}

QColor OmniThemeManager::surfaceColor() const { return m_surface; }
void OmniThemeManager::setSurfaceColor(const QColor& color) {
    if (m_surface != color) { m_surface = color; emit themeChanged(); }
}

QColor OmniThemeManager::textColor() const { return m_text; }
void OmniThemeManager::setTextColor(const QColor& color) {
    if (m_text != color) { m_text = color; emit themeChanged(); }
}

QColor OmniThemeManager::dangerColor() const { return m_danger; }
void OmniThemeManager::setDangerColor(const QColor& color) {
    if (m_danger != color) { m_danger = color; emit themeChanged(); }
}

QColor OmniThemeManager::successColor() const { return m_success; }
void OmniThemeManager::setSuccessColor(const QColor& color) {
    if (m_success != color) { m_success = color; emit themeChanged(); }
}

void OmniThemeManager::setDarkMode() {
    m_primary = QColor("#0078D7"); // Windows Blue
    m_secondary = QColor("#005A9E");
    m_background = QColor("#111111"); // Deep Dark
    m_surface = QColor("#1E1E1E"); // Elevated Surface
    m_text = QColor("#FFFFFF");
    m_danger = QColor("#D32F2F"); // Material Red
    m_success = QColor("#388E3C"); // Material Green
    emit themeChanged();
    qDebug() << "OmniThemeManager: Activated Dark Mode";
}

void OmniThemeManager::setLightMode() {
    m_primary = QColor("#0078D7");
    m_secondary = QColor("#005A9E");
    m_background = QColor("#F3F3F3"); // Off White
    m_surface = QColor("#FFFFFF"); // Pure White
    m_text = QColor("#000000");
    m_danger = QColor("#D32F2F");
    m_success = QColor("#388E3C");
    emit themeChanged();
    qDebug() << "OmniThemeManager: Activated Light Mode";
}

void OmniThemeManager::setCyberpunkMode() {
    m_primary = QColor("#FCEE09"); // Cyberpunk Yellow
    m_secondary = QColor("#00F0FF"); // Neon Cyan
    m_background = QColor("#0B0B0C"); // Vantablack
    m_surface = QColor("#1A1A1D"); // Gunmetal
    m_text = QColor("#FF003C"); // Neon Pink/Red
    m_danger = QColor("#FF0000");
    m_success = QColor("#00FF00");
    emit themeChanged();
    qDebug() << "OmniThemeManager: Activated Cyberpunk Mode";
}
