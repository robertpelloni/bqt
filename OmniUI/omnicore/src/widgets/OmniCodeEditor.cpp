#include "OmniCodeEditor.h"

OmniCodeEditor::OmniCodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    setObjectName("OmniCodeEditor");
    // qDebug() << "Initialized OmniCodeEditor";
}

OmniCodeEditor::~OmniCodeEditor() = default;
