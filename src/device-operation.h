#ifndef DEVICEOPERATION_H
#define DEVICEOPERATION_H

#include <QObject>

class DeviceOperation : public QObject
{
    Q_OBJECT
public:
    explicit DeviceOperation(QObject *parent = nullptr);

    bool udiskFormat();
    bool udiskRepair();

Q_SIGNALS:

};

#endif // DEVICEOPERATION_H
