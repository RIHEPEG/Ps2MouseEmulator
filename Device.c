#include "Driver.h"
#include "device.tmh"

// Define GUID in a single C file to avoid multiple-definition errors
#include <initguid.h>
DEFINE_GUID(GUID_DEVINTERFACE_PS2_MOUSE_EMULATOR,
    0x12345678, 0xABCD, 0xEF01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01);

// If GUID_DEVINTERFACE_PS2_MOUSE_EMULATOR is defined elsewhere, this extern will match it.
extern const GUID GUID_DEVINTERFACE_PS2_MOUSE_EMULATOR;

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, Ps2MouseEmulatorCreateDevice)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDevicePrepareHardware)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceRemove)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceSurpriseRemoval)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceReleaseHardware)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceDxStateEntry)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceDxStateExit)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceWakeFromSx)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceWakeFromSxTrigger)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceArmWakeFromSx)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceDisarmWakeFromSx)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceEnableWakeAtBus)
#pragma alloc_text (PAGE, Ps2MouseEmulatorEvtDeviceDisableWakeAtBus)
#endif

NTSTATUS
Ps2MouseEmulatorCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    WDF_PNPPOWER_EVENT_CALLBACKS pnpPowerCallbacks;
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    PDEVICE_CONTEXT deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

    PAGED_CODE();

    WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);

    // Only set callbacks that exist in this WDF_PNPPOWER_EVENT_CALLBACKS version.
    pnpPowerCallbacks.EvtDevicePrepareHardware = Ps2MouseEmulatorEvtDevicePrepareHardware;

    WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(DeviceInit, &deviceAttributes, &device);

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "WdfDeviceCreate failed with status 0x%x", status);
        return status;
    }

    deviceContext = DeviceGetContext(device);
    deviceContext->PrivateDeviceData = 0;

    status = WdfDeviceCreateDeviceInterface(
        device,
        &GUID_DEVINTERFACE_PS2_MOUSE_EMULATOR,
        NULL // ReferenceString
    );

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "WdfDeviceCreateDeviceInterface failed with status 0x%x", status);
        return status;
    }

    status = Ps2MouseEmulatorQueueInitialize(device);

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "Ps2MouseEmulatorQueueInitialize failed with status 0x%x", status);
        return status;
    }

    return STATUS_SUCCESS;
}

// Остальные функции (PrepareHardware, Remove, SurpriseRemoval и т.д.) реализуются аналогичным образом

// Добавление определения обработчика PrepareHardware, чтобы устранить ошибку "identifier not defined"
NTSTATUS
Ps2MouseEmulatorEvtDevicePrepareHardware(
    WDFDEVICE Device,
    WDFCMRESLIST ResourcesRaw,
    WDFCMRESLIST ResourcesTranslated
)
{
    PAGED_CODE();

    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(ResourcesRaw);
    UNREFERENCED_PARAMETER(ResourcesTranslated);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "Ps2MouseEmulatorEvtDevicePrepareHardware called");

    // TODO: Инициализация оборудования PS/2 мыши (если требуется)

    return STATUS_SUCCESS;
}

// Minimal stubs for other callbacks declared in Device.h to satisfy references.
VOID
Ps2MouseEmulatorEvtDeviceSurpriseRemoval(
    WDFDEVICE Device
)
{
    UNREFERENCED_PARAMETER(Device);
    TraceEvents(TRACE_LEVEL_WARNING, TRACE_DEVICE, "Ps2MouseEmulatorEvtDeviceSurpriseRemoval called");
}

NTSTATUS
Ps2MouseEmulatorEvtDeviceReleaseHardware(
    WDFDEVICE Device,
    WDFCMRESLIST ResourcesTranslated
)
{
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(ResourcesTranslated);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "Ps2MouseEmulatorEvtDeviceReleaseHardware called");
    return STATUS_SUCCESS;
}

NTSTATUS
Ps2MouseEmulatorEvtDeviceSelfManagedIoInit(
    WDFDEVICE Device
)
{
    UNREFERENCED_PARAMETER(Device);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "Ps2MouseEmulatorEvtDeviceSelfManagedIoInit called");
    return STATUS_SUCCESS;
}

VOID
Ps2MouseEmulatorEvtDeviceSelfManagedIoCleanup(
    WDFDEVICE Device
)
{
    UNREFERENCED_PARAMETER(Device);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "Ps2MouseEmulatorEvtDeviceSelfManagedIoCleanup called");
}

VOID
Ps2MouseEmulatorEvtIoDefault(
    WDFQUEUE Queue,
    WDFREQUEST Request
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Request);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "Ps2MouseEmulatorEvtIoDefault called");
    WdfRequestComplete(Request, STATUS_SUCCESS);
}

VOID
Ps2MouseEmulatorEvtIoDeviceControl(
    WDFQUEUE Queue,
    WDFREQUEST Request,
    size_t OutputBufferLength,
    size_t InputBufferLength,
    ULONG IoControlCode
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(IoControlCode);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "Ps2MouseEmulatorEvtIoDeviceControl called");
    WdfRequestComplete(Request, STATUS_SUCCESS);
}

VOID
Ps2MouseEmulatorEvtIoInternalDeviceControl(
    WDFQUEUE Queue,
    WDFREQUEST Request,
    size_t OutputBufferLength,
    size_t InputBufferLength,
    ULONG IoControlCode
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(IoControlCode);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "Ps2MouseEmulatorEvtIoInternalDeviceControl called");
    WdfRequestComplete(Request, STATUS_SUCCESS);
}

// Driver-level callbacks

VOID
Ps2MouseEmulatorEvtDriverContextCleanup(
    WDFOBJECT DriverObject
)
{
    UNREFERENCED_PARAMETER(DriverObject);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "Ps2MouseEmulatorEvtDriverContextCleanup called");
}

VOID
Ps2MouseEmulatorEvtDriverUnload(
    _In_ WDFDRIVER Driver
)
{
    UNREFERENCED_PARAMETER(Driver);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "Ps2MouseEmulatorEvtDriverUnload called");
}