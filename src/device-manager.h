#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <gio/gio.h>
#include <gio/gunixmounts.h>

#include <QObject>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    const DeviceManager* getInstance();

private:
    explicit DeviceManager(QObject* parent = nullptr);
    ~DeviceManager();
    static void drive_disconnected_callback(GVolumeMonitor* monitor, GDrive* drive, gpointer pThis);

Q_SIGNALS:
    void driveDisconnected(QString drive);


private:
    static DeviceManager*               gInstance;
    GVolumeMonitor*                     mGvolumeMonitor = nullptr;
};

#endif // DEVICEMANAGER_H
