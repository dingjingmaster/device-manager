#ifndef DEVICEOPERATION_H
#define DEVICEOPERATION_H

#include <QObject>
#include <gio/gio.h>
#include <udisks/udisks.h>

class DeviceOperation : public QObject
{
    Q_OBJECT
public:
    explicit DeviceOperation(GDrive* drive, QObject *parent = nullptr);
    explicit DeviceOperation(GVolume* volume, QObject *parent = nullptr);
    ~DeviceOperation();

public Q_SLOTS:
    void udiskFormat(QString type, QString labelName);
    void udiskRepair();
    void udiskFormatCancel();
    void udiskRepairCancel();

    QString udiskSize();
    QString udiskUUID();
    QString udiskLabel();

private:
    UDisksObject* getObjectFromBlockDevice(UDisksClient *client, const gchar *bdevice);

Q_SIGNALS:
    void repairFinished(bool);
    void formatFinished(bool);

private:
    UDisksBlock*            mDiskBlock = NULL;
    UDisksManager*          mDiskManager = NULL;
    UDisksFilesystem*       mDiskFilesystem = NULL;

    GCancellable            mRepairCancel;
    GCancellable            mFormatCancel;
};

#endif // DEVICEOPERATION_H
