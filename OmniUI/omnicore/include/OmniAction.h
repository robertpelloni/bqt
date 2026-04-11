#ifndef OMNIACTION_H
#define OMNIACTION_H

#include <QObject>
#include <QString>
#include <QKeySequence>

// Multi-User Action System (Qt6 QAction Parity)
// Actions in BobUI are not global; they belong to specific user focus trees.
class OmniAction : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QKeySequence shortcut READ shortcut WRITE setShortcut NOTIFY shortcutChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit OmniAction(QObject *parent = nullptr);
    virtual ~OmniAction();

    QString text() const;
    void setText(const QString& t);

    QKeySequence shortcut() const;
    void setShortcut(const QKeySequence& seq);

    bool enabled() const;
    void setEnabled(bool e);

    // Triggers the action specifically for a given deviceId
    Q_INVOKABLE void trigger(const QString& deviceId);

signals:
    void textChanged();
    void shortcutChanged();
    void enabledChanged();
    void triggered(const QString& deviceId);

private:
    QString m_text;
    QKeySequence m_shortcut;
    bool m_enabled;
};

#endif // OMNIACTION_H
