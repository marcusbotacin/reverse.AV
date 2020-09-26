#include <iostream>
#include <ntstatus.h>
#include <Windows.h>
#include <winternl.h>
#include <string>
#include <tchar.h>
#include <excpt.h>
#include <fstream>

struct _PROCESS_BASIC_INFORMATION_COPY 
{
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
} PROCESS_BASIC_INFORMATION_COPY;


struct _LDR_MODULE_COPY
{
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID BaseAddress;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    LIST_ENTRY HashTableEntry;
    ULONG TimeDateStamp;
} LDR_MODULE_COPY , *PLDR_MODULE_COPY;


struct _PEB_LDR_DATA_COPY
{ 
    ULONG Length;
    UCHAR Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID EntryInProgress;
} PEB_LDR_DATA_COPY , *PPEB_LDR_DATA_COPY;


typedef ULONG (WINAPI * ZwQueryInformationProcess)( HANDLE ProcessHandle,
                                                    ULONG  ProcessInformationClass,
                                                    PVOID  ProcessInformation,
                                                    ULONG  ProcessInformationLength,
                                                    PULONG ReturnLength );
