#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream> 
using namespace std;
typedef BOOL(*function)();
void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	HINSTANCE hLib = LoadLibraryA("../dll/Debug/dll.dll");
	if (hLib == NULL)
		printf("Error loading library\n");
	else
	{
		function SetHook = (function)GetProcAddress(hLib, "?SetHook@@YAHXZ");
		function DeleteHook = (function)GetProcAddress(hLib, "?DeleteHook@@YAHXZ");
		if (SetHook == NULL || DeleteHook == NULL)
		{
			printf("Error dll import.\n");
			FreeLibrary(hLib);
		}
		else
		{
			if (SetHook())
			{
				printf("Proc hooked.\nPress any Key to unhook.\n");
				_getch();
				if (DeleteHook())
					printf("Unhooked.\n");
				else
					printf("Error unsetting.\n");
			}
			else
				printf("Error while setting hook.\n");
		}
	}
	printf("Press any key to exit program.\n");
	_getch();
}
