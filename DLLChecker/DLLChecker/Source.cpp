/* Enumerating the DLLs loaded on my own process
 * Used to identify runtime injections
 * Marcus Botacin - UFPR - 2020
 */

#include<stdio.h>
#include<Windows.h>
#include<Psapi.h>

// the enumeration function
void print_libs()
{
	HMODULE mods[1024];
	DWORD bytes;
	// handle to my own process
	HANDLE me = GetCurrentProcess();
	// enumerate modules (DLLs)
	EnumProcessModules(me,mods,sizeof(mods),&bytes);
	// separator
	printf("-----------------\n");
	// for each module
	for(int i=0;i< (bytes/sizeof(HMODULE));i++)
	{
		// Just print the module name
		char modname[MAX_PATH];
		GetModuleFileNameA(mods[i], modname, sizeof(modname)/ sizeof(char));
		printf("%s\n",modname);
	}
}

int main()
{
	// keep printing over time
	while(1)
	{
		// identify functions
		print_libs();
		// wait some time to check again
		Sleep(3000);
	}
	return 0;
}