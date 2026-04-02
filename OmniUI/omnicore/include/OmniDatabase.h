#ifndef OMNIDATABASE_H
#define OMNIDATABASE_H

#include <QObject>
#include <QDebug>

class OmniDatabase : public QObject {
    Q_OBJECT
public:
    explicit OmniDatabase(QQuickItem *parent = nullptr);
    virtual ~OmniDatabase();
};

#endif // OMNIDATABASE_H
