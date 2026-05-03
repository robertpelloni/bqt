#ifndef OMNICOMBOBOX_H
#define OMNICOMBOBOX_H

#include <QQuickItem>
#include <QDebug>

class OmniComboBox : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniComboBox(QQuickItem *parent = nullptr);
    virtual ~OmniComboBox();
};

#endif // OMNICOMBOBOX_H
