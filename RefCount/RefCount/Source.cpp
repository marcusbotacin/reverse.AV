/* List loaded libs and their reference counter
 * Adapted from: https://stackoverflow.com/questions/3553231/how-to-check-dlls-reference-count-how-to-know-where-the-dll-was-loaded
 * Marcus Botacin - UFPR - 2020
 */

// All definitions
#include "defs.h"

using namespace std;

// exception filter, just keep things running
int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
   if (code == EXCEPTION_ACCESS_VIOLATION) {
      return EXCEPTION_EXECUTE_HANDLER;
   }
   return EXCEPTION_CONTINUE_SEARCH;
}

// lib inspection function
void print_libs()
{
	_PROCESS_BASIC_INFORMATION_COPY     stProcessBasicInformation   = { 0 };
    _PEB_LDR_DATA_COPY                  peb_ldr_data                = { 0 };
    _LDR_MODULE_COPY                    peb_ldr_module              = { 0 };
    PEB                                 peb                         = { 0 };
    USHORT                              loadCount                   = 0;
    
	// First thing is to get PEB address
	// This will be retrieved from theh ZwQueryInfo function
	// But this is not exported, so need to locate it
    HMODULE hModule = LoadLibraryA( (const char*)"NTDLL.dll" );
	HANDLE hProcess = GetCurrentProcess();
    ZwQueryInformationProcess ZwQueryInformationProcessPtr = (ZwQueryInformationProcess)GetProcAddress( hModule, "ZwQueryInformationProcess");

	// try to get PEB here
    if(ZwQueryInformationProcessPtr){
        ZwQueryInformationProcessPtr(hProcess, 0, &stProcessBasicInformation, sizeof(stProcessBasicInformation), 0);
    }

	// having the PEB, can read it and put on the struct to be parsed
    DWORD dwSize = 0;
    bool bStatus;
    // Get list of loaded DLLs from PE pointer
    bStatus = ReadProcessMemory(hProcess, stProcessBasicInformation.PebBaseAddress, &peb, sizeof(peb), &dwSize);
	// read the address pointed by the list in another struct
    bStatus = ReadProcessMemory(hProcess, peb.Ldr, &peb_ldr_data, sizeof(peb_ldr_data), &dwSize);

	// Get first element on the list
    void *readAddr = (void*) peb_ldr_data.InLoadOrderModuleList.Flink;
	// save value so we know when to stop (circular list)
	void *stop = readAddr;
	// print separator
	cout << "---------------" << endl;
    // Go through each modules one by one in their load order.
    while( ReadProcessMemory(hProcess, readAddr, &peb_ldr_module, sizeof(peb_ldr_module), &dwSize) )
    {
		// try to read, thing might go bad
        __try{
                // Get the reference count of the DLL
                loadCount = (signed short)peb_ldr_module.LoadCount;
				// print ref coutner first
				cout << peb_ldr_module.LoadCount << " | ";
				// module name later
                wcout << peb_ldr_module.BaseDllName.Buffer << endl;
            }_except(filter(GetExceptionCode(), GetExceptionInformation())){
				// if you are here, something failed
				// compute next element in the list
                readAddr = (void *) peb_ldr_module.InLoadOrderModuleList.Flink;
				// check if want to stop or not
				if (readAddr == stop)
				{
					break;
				}
				// if ok, proceed
                continue;
        }
		// same as above
        readAddr = (void *) peb_ldr_module.InLoadOrderModuleList.Flink;
		if (readAddr == stop)
			break;
    }
}

int main()
{
	// keep dumping to track real-time injections
	while(true)
	{
		// check libs
		print_libs();
		// check again in 3 seconds
		Sleep(3000);
	}
    return 0;
}