#include "helper.h"


BOOLEAN ZwCopyFile(IN PUNICODE_STRING ustrDestFile, IN PUNICODE_STRING ustrSrcFile)
{
	//\??\C:\1.txt

	HANDLE hSrcFile, hDestFile = -1;
	PVOID buffer = NULL;
	ULONG length = 0;
	LARGE_INTEGER offset = { 0 };
	IO_STATUS_BLOCK Io_Status_Block = { 0 };
	OBJECT_ATTRIBUTES obj_attr;
	NTSTATUS status;
	BOOLEAN bRet = FALSE;

	do 
	{
		InitializeObjectAttributes(&obj_attr, ustrSrcFile,
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

		status = ZwCreateFile(&hSrcFile, GENERIC_READ, &obj_attr, &Io_Status_Block, NULL,
			FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_NON_DIRECTORY_FILE
			| FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);


		if (!NT_SUCCESS(status))
		{
			bRet = FALSE;
			break;
		}

		InitializeObjectAttributes(&obj_attr, ustrDestFile,
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

		status = ZwCreateFile(&hDestFile, GENERIC_WRITE, &obj_attr, &Io_Status_Block, NULL,
			FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE
			| FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

		if (!NT_SUCCESS(status))
		{
			bRet = FALSE;
			break;
		}

		buffer = ExAllocatePool(NonPagedPool, 1024 * 4);
		if (buffer == NULL)
		{
			bRet = FALSE;
			break;
		}

		RtlFillMemory(buffer, 1024 * 4, 0);

		while (TRUE)
		{
			length = 1024 * 4;
			status = ZwReadFile(hSrcFile, NULL, NULL, NULL,
				&Io_Status_Block, buffer, length, &offset, NULL);
			if (!NT_SUCCESS(status))
			{
				if (status == STATUS_END_OF_FILE)
				{
					bRet = TRUE;
					break;
				}
				bRet = FALSE;
				break;
			}
			length = (ULONG)Io_Status_Block.Information;

			status = ZwWriteFile(hDestFile, NULL, NULL, NULL,
				&Io_Status_Block, buffer, length, &offset, NULL);

			if (!NT_SUCCESS(status))
			{
				bRet = FALSE;
				break;
			}
			offset.QuadPart += length;
		}


	} while (FALSE);

	if (hSrcFile)
		ZwClose(hSrcFile);
	if (hDestFile)
		ZwClose(hDestFile);
	if (buffer != NULL)
		ExFreePool(buffer);

	return bRet;

}