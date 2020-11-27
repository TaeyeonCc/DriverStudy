#include "helper.h"

#define JIANGCAN_TAGE 'TSET'

void MemoryTest()
{
	PVOID ptr1 = ExAllocatePoolWithTag(PagedPool, 0x100, JIANGCAN_TAGE);
	PVOID ptr2 = ExAllocatePoolWithTag(NonPagedPool, 0x200, JIANGCAN_TAGE);
	if (ptr1 == NULL || ptr2 == NULL)
		return;
	RtlFillMemory(ptr1, 0x100, 0x90);
	RtlFillMemory(ptr2, 0x200, 0x80);

	RtlMoveMemory(ptr2, ptr1, 0x100);


	ExFreePool(ptr1);
	ExFreePoolWithTag(ptr2, JIANGCAN_TAGE);
}

KIRQL WPOFFx64() {
	KIRQL irql = KeRaiseIrqlToDpcLevel();
	UINT64 cr0 = __readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;
}


void WPONx64(KIRQL irql) {
	UINT64 cr0 = __readcr0();
	cr0 |= 0x10000;
	_enable();
	__writecr0(cr0);
	KeLowerIrql(irql);
}

void TestMemCopy() {
	KIRQL irql = WPOFFx64();
	PVOID HookCode = ExAllocatePool(NonPagedPool, 0x200);
	RtlFillMemory(HookCode, 0x200, 0x90);
	RtlMoveMemory(HookCode, NtOpenProcess, 0x3);//NtOpenProcess函数的前三个字节
	RtlMoveMemory(NtOpenProcess, HookCode, 0x3);
	WPONx64(irql);
	//PASSIVE_LEVEL 
}


BOOLEAN SafeCopyMemory(PVOID pDestination, PVOID pSourceAddress, SIZE_T SizeOfCopy)
{
	PMDL pMdl = NULL;
	PVOID pSafeAddress = NULL;
	if (!MmIsAddressValid(pDestination) || !MmIsAddressValid(pSourceAddress))
		return FALSE;
	pMdl = IoAllocateMdl(pDestination, (ULONG)SizeOfCopy, FALSE, FALSE, NULL);
	if (!pMdl)
		return FALSE;
	__try
	{
		MmProbeAndLockPages(pMdl, KernelMode, IoReadAccess);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		IoFreeMdl(pMdl);
		return FALSE;
	}
	pSafeAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
	if (!pSafeAddress)
		return FALSE;
	__try
	{
		RtlMoveMemory(pSafeAddress, pSourceAddress, SizeOfCopy);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		;
	}
	MmUnlockPages(pMdl);
	IoFreeMdl(pMdl);
	return TRUE;
}

void TestMemCopy1() {
	DbgBreakPoint();

	PVOID HookCode = ExAllocatePool(NonPagedPool, 0x200);
	RtlFillMemory(HookCode, 0x200, 0x90);
	RtlMoveMemory(HookCode, NtOpenProcess, 0x3);
	SafeCopyMemory(NtOpenProcess, HookCode, 0x3);
}