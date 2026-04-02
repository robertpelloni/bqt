#ifndef OMNITEXTFIELD_H
#define OMNITEXTFIELD_H

#include <QLineEdit>
#include <QDebug>

class OmniTextField : public QLineEdit {
    Q_OBJECT
public:
    explicit OmniTextField(QWidget *parent = nullptr);
    virtual ~OmniTextField();
};

#endif // OMNITEXTFIELD_H
