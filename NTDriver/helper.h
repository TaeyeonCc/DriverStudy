#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>

#define	DEVICE_NAME	L"\\Device\\KrnlHW64"
#define LINK_NAME	L"\\DosDevices\\KrnlHW64"
#define LINK_GLOBAL_NAME	L"\\DosDevices\\Global\\KrnlHW64"

#define IOCTL_IO_TEST	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SAY_HELLO	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

BOOLEAN ZwCopyFile(IN PUNICODE_STRING ustrDestFile, IN PUNICODE_STRING ustrSrcFile);