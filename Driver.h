#pragma once

#include <ntddk.h>
#include <wdf.h>
#include "Device.h"

#ifdef __cplusplus
extern "C" {
#endif

// Declarations for driver entry/unload and helper functions

NTSTATUS
DriverEntry(
    _In_ DRIVER_OBJECT* DriverObject,
    _In_ UNICODE_STRING* RegistryPath
    );

VOID
OnUnload(
    _In_ DRIVER_OBJECT* DriverObject
    );

// Open a WDF I/O target to communicate with a USB mouse (returns WDFIOTARGET or NULL on failure)
WDFIOTARGET OpenUsbMouse(_In_ WDFDEVICE Device);

// Close and delete the previously opened WDF I/O target
VOID CloseUsbMouse(_In_opt_ WDFIOTARGET IoTarget);

// EvtDeviceAdd callback for KMDF
NTSTATUS
EvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

#ifdef __cplusplus
}
#endif
