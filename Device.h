#pragma once

#include <wdm.h>
#include <wdf.h>
#include <usb.h>
#include <usbdlib.h>
#include <wdfusb.h>

// GUID is defined in a single C file to avoid multiple-definition link errors
extern const GUID GUID_DEVINTERFACE_PS2_MOUSE_EMULATOR;

// Структура контекста устройства
typedef struct _DEVICE_CONTEXT
{
    WDFUSBDEVICE UsbDevice;
    ULONG PrivateDeviceData;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;

// Макрос для безопасного получения контекста устройства
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

// Внешние объявления функций драйвера
EXTERN_C_START

// Основные callback-функции драйвера
DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD Ps2MouseEmulatorEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP Ps2MouseEmulatorEvtDriverContextCleanup;
EVT_WDF_DRIVER_UNLOAD Ps2MouseEmulatorEvtDriverUnload;

// Callback-функции устройства
EVT_WDF_DEVICE_PREPARE_HARDWARE Ps2MouseEmulatorEvtDevicePrepareHardware;
EVT_WDF_DEVICE_SURPRISE_REMOVAL Ps2MouseEmulatorEvtDeviceSurpriseRemoval;
EVT_WDF_DEVICE_RELEASE_HARDWARE Ps2MouseEmulatorEvtDeviceReleaseHardware;
EVT_WDF_DEVICE_SELF_MANAGED_IO_INIT Ps2MouseEmulatorEvtDeviceSelfManagedIoInit;
EVT_WDF_DEVICE_SELF_MANAGED_IO_CLEANUP Ps2MouseEmulatorEvtDeviceSelfManagedIoCleanup;

// Обработчики ввода-вывода
EVT_WDF_IO_QUEUE_IO_DEFAULT Ps2MouseEmulatorEvtIoDefault;
EVT_WDF_IO_QUEUE_IO_READ Ps2MouseEmulatorEvtIoRead;
EVT_WDF_IO_QUEUE_IO_WRITE Ps2MouseEmulatorEvtIoWrite;
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL Ps2MouseEmulatorEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL Ps2MouseEmulatorEvtIoInternalDeviceControl;

// Инициализация очередей ввода-вывода
NTSTATUS Ps2MouseEmulatorQueueInitialize(_In_ WDFDEVICE Device);

EXTERN_C_END
