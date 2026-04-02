#ifndef OMNIBUTTON_H
#define OMNIBUTTON_H

#include <QPushButton>
#include <QDebug>

class OmniButton : public QPushButton {
    Q_OBJECT
public:
    explicit OmniButton(QWidget *parent = nullptr);
    virtual ~OmniButton();
};

#endif // OMNIBUTTON_H
