#ifndef OMNICODEEDITOR_H
#define OMNICODEEDITOR_H

#include <QPlainTextEdit>
#include <QDebug>

class OmniCodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit OmniCodeEditor(QWidget *parent = nullptr);
    virtual ~OmniCodeEditor();
};

#endif // OMNICODEEDITOR_H
