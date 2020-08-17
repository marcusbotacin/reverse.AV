// Marcus Botacin - Library Loading - 2020

#include<stdio.h>
#include<Windows.h>

// Guessing a prototype
typedef int (__cdecl *MYPROC)(void); 

int main(int argc, char *argv[])
{
    // Load it
	HMODULE h = LoadLibraryA(argv[1]);
    // check if it works
	if(!h)
	{
		printf("Fail\n");
	}else{
		printf("%x\n",h);
        // function to be invoked
		MYPROC addr = (MYPROC)GetProcAddress(h,"engine_GlobalGetParameter");
		printf("%x\n",addr);
        // invoke it
		int ret = (addr)();
        // return value
		printf("%x\n",ret);
	}
	return 0;
}
