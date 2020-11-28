#include "helper.h"


VOID UnicodeToChar(PUNICODE_STRING src, char* dst)
{
	ANSI_STRING string;
	RtlUnicodeStringToAnsiString(&string, src, TRUE);
	strcpy(dst, string.Buffer);
	RtlFreeAnsiString(&string);
}

VOID WcharToChar(PWCHAR src, PCHAR dst)
{
	UNICODE_STRING uString = { 0 };
	ANSI_STRING aString = { 0 };
	RtlInitUnicodeString(&uString, src);

	RtlUnicodeStringToAnsiString(&uString, &aString, TRUE);
	strcpy(dst, aString.Buffer);
	RtlFreeAnsiString(&aString);
}

VOID CharToWchar(PCHAR src, PWCHAR dst)
{
	ANSI_STRING aString;
	UNICODE_STRING uString;
	RtlInitAnsiString(&aString, src);
	RtlAnsiStringToUnicodeString(&uString, &aString, TRUE);
	wcscpy(dst, uString.Buffer);
	RtlFreeUnicodeString(&uString);
}

void StingTestZ()
{
	CHAR chString1[] = "hello";
	ANSI_STRING ansiSting1;
	RtlInitAnsiString(&ansiSting1, chString1);
	DbgPrint("jjcc %Z\n", ansiSting1);
	chString1[0] = 'H';
	DbgPrint("jjcc %Z\n", ansiSting1);

#define BUFFER_SIZE 1024

	UNICODE_STRING uniString1 = {0};
	WCHAR* wzString = L"Hello World!";
	uniString1.MaximumLength = BUFFER_SIZE;
	uniString1.Buffer = ExAllocatePool(PagedPool, BUFFER_SIZE);
	uniString1.Length = 2 * wcslen(wzString);

	RtlCopyMemory(uniString1.Buffer, wzString, uniString1.Length);

	KdPrint(("jjcc UnicodeString %wZ\n", uniString1));

	ExFreePool(uniString1.Buffer);
	uniString1.Buffer = NULL;
	uniString1.MaximumLength = uniString1.Length = 0;

	//RtlCopyUnicodeString()
	//RtlFreeUnicodeString()

	//RtlEqualUnicodeString()  CaseInSensitive
	//RtlUpcaseUnicodeString() AllocateDestinationString

	//RtlUnicodeStringToInteger() ULONG Base
	//RtlIntegerToUnicodeString()
	//RtlUnicodeStringToAnsiString()
	//RtlAnsiStringToUnicodeString()
	DbgBreakPoint();

	WCHAR* strSrc = L"Hello World";
	UNICODE_STRING uniString = { 0 };
	RtlInitUnicodeString(&uniString, strSrc);

	char dstBuffer[64] = { 0 };
	UnicodeToChar(&uniString, dstBuffer);

	return;
}