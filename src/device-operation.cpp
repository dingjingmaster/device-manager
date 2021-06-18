#include "device-operation.h"

#include <sys/stat.h>

static void formatCB (GObject* sourceObject, GAsyncResult* res, gpointer udata);
static void repairCB (GObject* sourceObject, GAsyncResult* res, gpointer udata);


DeviceOperation::DeviceOperation(GDrive *drive, QObject *parent) : QObject(parent)
{
    g_return_if_fail(drive);

    g_autofree char* devName = g_drive_get_identifier(drive, G_DRIVE_IDENTIFIER_KIND_UNIX_DEVICE);
    g_return_if_fail(devName);

    UDisksClient* client = udisks_client_new_sync(NULL, NULL);
    g_return_if_fail(client);

    UDisksObject* udiskObj = getObjectFromBlockDevice(client, devName);
    g_return_if_fail(udiskObj);

    mDiskBlock = udisks_object_get_block(udiskObj);
    mDiskManager = udisks_object_get_manager(udiskObj);
    mDiskFilesystem = udisks_object_get_filesystem(udiskObj);

    g_clear_object(&client);
    g_clear_object(&udiskObj);
}

DeviceOperation::~DeviceOperation()
{
    g_clear_object(&mDiskBlock);
    g_clear_object(&mDiskManager);
    g_clear_object(&mDiskFilesystem);
}

bool DeviceOperation::udiskFormat(QString type, QString labelName)
{
    bool ret = false;

    GVariantBuilder optionsBuilder;
    g_variant_builder_init(&optionsBuilder, G_VARIANT_TYPE_VARDICT);

    g_variant_builder_add (&optionsBuilder, "{sv}", "label", g_variant_new_string (labelName.toUtf8().constData()));
    g_variant_builder_add (&optionsBuilder, "{sv}", "take-ownership", g_variant_new_boolean (TRUE));
    g_variant_builder_add (&optionsBuilder, "{sv}", "update-partition-type", g_variant_new_boolean (TRUE));

    udisks_block_call_format(mDiskBlock, type.toLower().toUtf8().constData(), g_variant_builder_end(&optionsBuilder), NULL, formatCB, &ret);

    return ret;
}

bool DeviceOperation::udiskRepair()
{
    bool ret = false;

    GVariantBuilder optionsBuilder;
    g_variant_builder_init(&optionsBuilder, G_VARIANT_TYPE_VARDICT);

    udisks_filesystem_call_repair(mDiskFilesystem, g_variant_builder_end(&optionsBuilder), NULL, repairCB, &ret);

    return ret;
}

UDisksObject *DeviceOperation::getObjectFromBlockDevice(UDisksClient *client, const gchar *bdevice)
{
    struct stat statbuf;
    UDisksBlock* block = NULL;
    UDisksObject* object = NULL;
    UDisksObject* cryptoBackingObject = NULL;
    g_autofree const gchar* cryptoBackingDevice = NULL;

    g_return_val_if_fail(stat(bdevice, &statbuf) == 0, object);

    block = udisks_client_get_block_for_dev (client, statbuf.st_rdev);
    g_return_val_if_fail(block != NULL, object);

    object = UDISKS_OBJECT (g_dbus_interface_dup_object (G_DBUS_INTERFACE (block)));
    g_object_unref (block);

    cryptoBackingDevice = udisks_block_get_crypto_backing_device ((udisks_object_peek_block (object)));
    cryptoBackingObject = udisks_client_get_object (client, cryptoBackingDevice);
    if (cryptoBackingObject != NULL) {
        g_object_unref (object);
        object = cryptoBackingObject;
    }

    return object;
}

static void formatCB (GObject* sourceObject, GAsyncResult* res, gpointer udata)
{
    bool ret = false;
    GError* error = NULL;

    if (!udisks_block_call_format_finish (UDISKS_BLOCK(sourceObject), res, &error)) {
        if (NULL != error && NULL != strstr(error->message, "wipefs:")) {
            g_clear_error(&error);
        }
        ret = false;
    } else {
        ret = true;
    }

    if (udata) {
        bool* uret = static_cast<bool*>(udata);
        *uret = ret;
    }
}
static void repairCB (GObject* sourceObject, GAsyncResult* res, gpointer udata)
{
    GError* error = NULL;
    gboolean outRet = FALSE;

    if (!udisks_filesystem_call_repair_finish(UDISKS_FILESYSTEM(sourceObject), &outRet, res, &error)) {
        g_clear_error(&error);
        outRet = false;
    }

    if (udata) {
        bool* uret = static_cast<bool*>(udata);
        *uret = outRet;
    }
}
