#ifndef OMNIUNDOSTACK_H
#define OMNIUNDOSTACK_H

#include <QObject>
#include <QString>
#include <QList>
#include <functional>

struct OmniUndoCommand {
    QString userId;
    QString description;
    std::function<void()> undoAction;
    std::function<void()> redoAction;
};

// High-performance Multi-User Undo Kernel
class OmniUndoStack : public QObject {
    Q_OBJECT
public:
    static OmniUndoStack* instance();

    // Pushes a new action onto the stack, tagged with the performing user
    void pushCommand(const QString& userId, const QString& desc, std::function<void()> undo, std::function<void()> redo);

    // Undoes the last action performed specifically by this user
    Q_INVOKABLE void undo(const QString& userId);

    // Redoes the last undone action specifically for this user
    Q_INVOKABLE void redo(const QString& userId);

signals:
    void stackChanged();

private:
    explicit OmniUndoStack(QObject *parent = nullptr);
    ~OmniUndoStack() override;

    QList<OmniUndoCommand> m_commands;
    QList<OmniUndoCommand> m_redoBuffer;
};

#endif // OMNIUNDOSTACK_H
