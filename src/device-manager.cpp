#include "device-manager.h"

DeviceManager* DeviceManager::gInstance = new DeviceManager;

const DeviceManager* DeviceManager::getInstance()
{
    return gInstance;
}

DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
    mGvolumeMonitor = g_volume_monitor_get();
    g_signal_connect(mGvolumeMonitor, "drive-disconnected", G_CALLBACK(drive_disconnected_callback), this);
}

DeviceManager::~DeviceManager()
{
    if (mGvolumeMonitor)        g_object_unref(mGvolumeMonitor);
}

void DeviceManager::drive_disconnected_callback(GVolumeMonitor* monitor, GDrive* drive, gpointer pThis)
{
    g_return_if_fail(drive);
    g_return_if_fail(pThis);

    DeviceManager*      dm = static_cast<DeviceManager*>(pThis);

    g_autofree gchar*   devName = g_drive_get_identifier(drive, G_DRIVE_IDENTIFIER_KIND_UNIX_DEVICE);
    if (nullptr != devName) {
        Q_EMIT dm->driveDisconnected(devName);
    }

    Q_UNUSED(monitor);
}
