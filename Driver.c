#include "Driver.h"
#include <ntddk.h>

// Forward declare device creation implemented in Device.c
NTSTATUS Ps2MouseEmulatorCreateDevice(_Inout_ PWDFDEVICE_INIT DeviceInit);

// Open a WDF I/O target to communicate with a USB mouse
WDFIOTARGET
OpenUsbMouse(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS status;
    WDFIOTARGET ioTarget = NULL;
    WDF_IO_TARGET_OPEN_PARAMS openParams;
    UNICODE_STRING targetName;

    // TODO: Obtain correct device symbolic link or interface string dynamically.
    RtlInitUnicodeString(&targetName, L"\\??\\HID#VID_XXXX&PID_YYYY#ZZZ#{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}");

    status = WdfIoTargetCreate(Device, WDF_NO_OBJECT_ATTRIBUTES, &ioTarget);
    if (!NT_SUCCESS(status)) {
        KdPrint(("WdfIoTargetCreate failed: 0x%08X\n", status));
        return NULL;
    }

    WDF_IO_TARGET_OPEN_PARAMS_INIT_OPEN_BY_NAME(&openParams, &targetName, GENERIC_READ | GENERIC_WRITE);

    status = WdfIoTargetOpen(ioTarget, &openParams);
    if (!NT_SUCCESS(status)) {
        KdPrint(("WdfIoTargetOpen failed: 0x%08X\n", status));
        WdfObjectDelete(ioTarget);
        return NULL;
    }

    return ioTarget;
}

// Close and delete the previously opened WDF I/O target
VOID
CloseUsbMouse(
    _In_opt_ WDFIOTARGET IoTarget
)
{
    if (IoTarget == NULL) {
        return;
    }

    (void)WdfIoTargetClose(IoTarget);
    WdfObjectDelete(IoTarget);
}

// Event: DeviceAdd (wraps device creation implemented in Device.c)
NTSTATUS
Ps2MouseEmulatorEvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    UNREFERENCED_PARAMETER(Driver);

    return Ps2MouseEmulatorCreateDevice(DeviceInit);
}

// DriverEntry
NTSTATUS
DriverEntry(
    _In_ DRIVER_OBJECT* DriverObject,
    _In_ UNICODE_STRING* RegistryPath
)
{
    NTSTATUS status;
    WDF_DRIVER_CONFIG config;
    WDFDRIVER driver;

    WDF_DRIVER_CONFIG_INIT(&config, Ps2MouseEmulatorEvtDeviceAdd);

    status = WdfDriverCreate(DriverObject,
                             RegistryPath,
                             WDF_NO_OBJECT_ATTRIBUTES,
                             &config,
                             &driver);
    if (!NT_SUCCESS(status)) {
        KdPrint(("WdfDriverCreate failed: 0x%08X\n", status));
        return status;
    }

    KdPrint(("PS2 Mouse Emulator Driver loaded\n"));

    return STATUS_SUCCESS;
}

// OnUnload - framework drivers typically use EvtDriverContextCleanup; keep a simple function for legacy use.
VOID
OnUnload(
    _In_ DRIVER_OBJECT* DriverObject
)
{
    UNREFERENCED_PARAMETER(DriverObject);
    KdPrint(("PS2 Mouse Emulator Driver unloaded\n"));
}

// Deprecated stub kept for compatibility
NTSTATUS
EvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    UNREFERENCED_PARAMETER(Driver);
    UNREFERENCED_PARAMETER(DeviceInit);
    return STATUS_SUCCESS;
}

// Minimal I/O callbacks used by queue
VOID
Ps2MouseEmulatorEvtIoRead(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Length);

    KdPrint(("Reading data from device...\n"));
    WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 0);
}

VOID
Ps2MouseEmulatorEvtIoWrite(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Length);

    KdPrint(("Writing data to device...\n"));
    WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 0);
}
