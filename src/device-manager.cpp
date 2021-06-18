#include "device-manager.h"

DeviceManager* DeviceManager::gInstance = new DeviceManager;

DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
//    g_signal_connect (mGvolumeMonitor, "drive-connected", G_CALLBACK (drive_connected_callback), this);
//    g_signal_connect (mGvolumeMonitor, "drive-disconnected", G_CALLBACK (drive_disconnected_callback), this);
//    g_signal_connect (mGvolumeMonitor, "volume-added", G_CALLBACK (volume_added_callback), this);
//    g_signal_connect (mGvolumeMonitor, "volume-removed", G_CALLBACK (volume_removed_callback), this);
//    g_signal_connect (mGvolumeMonitor, "mount-added", G_CALLBACK (mount_added_callback), this);
//    g_signal_connect (mGvolumeMonitor, "mount-removed", G_CALLBACK (mount_removed_callback), this);
}
