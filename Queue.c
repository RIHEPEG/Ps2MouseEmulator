#include "Driver.h"
#include "queue.tmh"

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