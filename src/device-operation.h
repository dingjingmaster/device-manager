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
    ~DeviceOperation();

    bool udiskFormat(QString type, QString labelName);
    bool udiskRepair();

private:
    UDisksObject* getObjectFromBlockDevice(UDisksClient *client, const gchar *bdevice);

Q_SIGNALS:

private:
    UDisksBlock*            mDiskBlock = NULL;
    UDisksManager*          mDiskManager = NULL;
    UDisksFilesystem*       mDiskFilesystem = NULL;

};

#endif // DEVICEOPERATION_H
