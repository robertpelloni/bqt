#ifndef OMNISPLITVIEW_H
#define OMNISPLITVIEW_H

#include <QQuickItem>
#include <QDebug>

class OmniSplitView : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniSplitView(QQuickItem *parent = nullptr);
    virtual ~OmniSplitView();
};

#endif // OMNISPLITVIEW_H
