#include <ntddk.h>

NTSTATUS DriverUnload(PDRIVER_OBJECT pDriver)
{

	KdPrint(("jjcc DriverUnload\n"));
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg)
{
	KdPrint(("jjcc DriverEntry"));
	pDriver->DriverUnload = DriverUnload;

	return STATUS_SUCCESS;
}