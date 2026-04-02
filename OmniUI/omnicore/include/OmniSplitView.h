#ifndef OMNISPLITVIEW_H
#define OMNISPLITVIEW_H

#include <QSplitter>
#include <QDebug>

class OmniSplitView : public QSplitter {
    Q_OBJECT
public:
    explicit OmniSplitView(QWidget *parent = nullptr);
    virtual ~OmniSplitView();
};

#endif // OMNISPLITVIEW_H
