#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <gio/gunixmounts.h>

#include <QObject>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject* parent = nullptr);

Q_SIGNALS:


private:
    static DeviceManager*               gInstance;
    GVolumeMonitor*                     mGvolumeMonitor = nullptr;
};

#endif // DEVICEMANAGER_H
