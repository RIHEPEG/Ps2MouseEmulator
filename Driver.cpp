#include "Driver.h"
#include <ntddk.h>

// Forward declare device creation implemented in Device.c
NTSTATUS Ps2MouseEmulatorCreateDevice(_Inout_ PWDFDEVICE_INIT DeviceInit);

// Open a WDF I/O target to communicate with a USB mouse
WDFIOTARGET OpenUsbMouse(_In_ WDFDEVICE Device)
{
    NTSTATUS status;
    WDFIOTARGET ioTarget = NULL;
    WDF_IO_TARGET_OPEN_PARAMS openParams;
    UNICODE_STRING targetName;

    // TODO: Obtain correct device symbolic link or interface string dynamically.
    // Using a placeholder here is unsafe; replace this with Device Interface enumeration.
    RtlInitUnicodeString(&targetName, L"\\??\\HID#VID_XXXX&PID_YYYY#ZZZ#{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}");

    // Create I/O target for the provided device
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
VOID CloseUsbMouse(_In_opt_ WDFIOTARGET IoTarget)
{
    if (IoTarget == NULL) {
        return;
    }

    // Close target (ignore errors)
    (void)WdfIoTargetClose(IoTarget);

    // Delete WDF object (will be cleaned up properly)
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

// Точка входа драйвера
NTSTATUS DriverEntry(_In_ DRIVER_OBJECT* DriverObject, _In_ UNICODE_STRING* RegistryPath)
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

// Функция выгрузки драйвера
void OnUnload(_In_ DRIVER_OBJECT* DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    // Освобождение ресурсов при выгрузке драйвера
    KdPrint(("PS2 Mouse Emulator Driver unloaded\n"));
}

// Функция создания и инициализации устройства
// (kept as a stub if needed elsewhere; actual device creation happens in Ps2MouseEmulatorCreateDevice)
NTSTATUS EvtDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit)
{
    UNREFERENCED_PARAMETER(Driver);
    UNREFERENCED_PARAMETER(DeviceInit);

    // Deprecated: use Ps2MouseEmulatorEvtDeviceAdd as WDF callback
    return STATUS_SUCCESS;
}

// Обработчик чтения данных устройством (name matches Queue.c expectation)
VOID Ps2MouseEmulatorEvtIoRead(_In_ WDFQUEUE Queue, _In_ WDFREQUEST Request, _In_ size_t Length)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Length);

    KdPrint(("Reading data from device...\n"));

    // Complete request with no data for now
    WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 0);
}

// Обработчик записи данных в устройство (name matches Queue.c expectation)
VOID Ps2MouseEmulatorEvtIoWrite(_In_ WDFQUEUE Queue, _In_ WDFREQUEST Request, _In_ size_t Length)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Length);

    KdPrint(("Writing data to device...\n"));

    // Complete request with no data written for now
    WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 0);
}