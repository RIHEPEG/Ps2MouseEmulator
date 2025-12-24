#include "Driver.h"
#if defined(__has_include)
#  if __has_include("queue.tmh")
#    include "queue.tmh"
#  else
#    define WPP_INIT_TRACING(DriverObject, RegistryPath)
#    define WPP_CLEANUP(DriverObject)
#  endif
#elif defined(EVENT_TRACING)
#  include "queue.tmh"
#else
#  define WPP_INIT_TRACING(DriverObject, RegistryPath)
#  define WPP_CLEANUP(DriverObject)
#endif

NTSTATUS
Ps2MouseEmulatorQueueInitialize(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDFQUEUE queue;

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchSequential);
    queueConfig.EvtIoRead = Ps2MouseEmulatorEvtIoRead;
    queueConfig.EvtIoWrite = Ps2MouseEmulatorEvtIoWrite;

    status = WdfIoQueueCreate(Device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    return STATUS_SUCCESS;
}
