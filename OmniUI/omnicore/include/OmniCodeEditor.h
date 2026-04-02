#ifndef OMNICODEEDITOR_H
#define OMNICODEEDITOR_H

#include <QQuickItem>
#include <QDebug>

class OmniCodeEditor : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniCodeEditor(QQuickItem *parent = nullptr);
    virtual ~OmniCodeEditor();
};

#endif // OMNICODEEDITOR_H
