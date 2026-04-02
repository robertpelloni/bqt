#ifndef OMNICOMBOBOX_H
#define OMNICOMBOBOX_H

#include <QComboBox>
#include <QDebug>

class OmniComboBox : public QComboBox {
    Q_OBJECT
public:
    explicit OmniComboBox(QWidget *parent = nullptr);
    virtual ~OmniComboBox();
};

#endif // OMNICOMBOBOX_H
