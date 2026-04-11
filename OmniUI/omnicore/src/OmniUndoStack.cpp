#include "OmniUndoStack.h"
#include <QDebug>

OmniUndoStack* OmniUndoStack::instance() {
    static OmniUndoStack* manager = new OmniUndoStack();
    return manager;
}

OmniUndoStack::OmniUndoStack(QObject *parent) : QObject(parent) {
    setObjectName("OmniUndoStack");
}

OmniUndoStack::~OmniUndoStack() = default;

void OmniUndoStack::pushCommand(const QString& userId, const QString& desc, std::function<void()> undo, std::function<void()> redo) {
    OmniUndoCommand cmd{userId, desc, undo, redo};
    m_commands.append(cmd);
    
    // Limits stack size to prevent RAM leaks in long multiplayer sessions
    if (m_commands.size() > 1000) m_commands.removeFirst();
    
    qDebug() << "OmniUndoStack: Recorded [" << desc << "] for User [" << userId << "]";
    emit stackChanged();
}

void OmniUndoStack::undo(const QString& userId) {
    // Search backwards for the last command belonging to THIS specific user
    for (int i = m_commands.size() - 1; i >= 0; --i) {
        if (m_commands[i].userId == userId) {
            OmniUndoCommand cmd = m_commands.takeAt(i);
            cmd.undoAction();
            m_redoBuffer.append(cmd);
            qDebug() << "OmniUndoStack: Undoing [" << cmd.description << "] for User [" << userId << "]";
            emit stackChanged();
            return;
        }
    }
}

void OmniUndoStack::redo(const QString& userId) {
    for (int i = m_redoBuffer.size() - 1; i >= 0; --i) {
        if (m_redoBuffer[i].userId == userId) {
            OmniUndoCommand cmd = m_redoBuffer.takeAt(i);
            cmd.redoAction();
            m_commands.append(cmd);
            qDebug() << "OmniUndoStack: Redoing [" << cmd.description << "] for User [" << userId << "]";
            emit stackChanged();
            return;
        }
    }
}
