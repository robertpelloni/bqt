#include "OmniAction.h"
#include <QDebug>

OmniAction::OmniAction(QObject *parent) : QObject(parent), m_enabled(true) {
    setObjectName("OmniAction");
}

OmniAction::~OmniAction() = default;

QString OmniAction::text() const { return m_text; }
void OmniAction::setText(const QString& t) { m_text = t; emit textChanged(); }

QKeySequence OmniAction::shortcut() const { return m_shortcut; }
void OmniAction::setShortcut(const QKeySequence& seq) { m_shortcut = seq; emit shortcutChanged(); }

bool OmniAction::enabled() const { return m_enabled; }
void OmniAction::setEnabled(bool e) { m_enabled = e; emit enabledChanged(); }

void OmniAction::trigger(const QString& deviceId) {
    if (!m_enabled) return;
    qDebug() << "OmniAction:" << m_text << "triggered by Device:" << deviceId;
    emit triggered(deviceId);
}
