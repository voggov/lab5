// Lab5(1).cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Header.h"
#include "Header1.h"
#include "pch.h"
#include "pch.h"

typedef void (*funclass) ();

int main()
{
	printf("Static\n");

	AssyncFileCopyTaskStatic();
	printf("Static library func is cancel working\n");
	system("pause");
	//FindFileAndWriteFile("E:\\p_p\\");
	HMODULE hDynamicLib = LoadLibrary(L"D:/ПТУ/Операционки/Lab5/dinDll/Debug/dinDll.dll");
	if (!hDynamicLib) {
		printf("Error while load dynamic lib");
	}
	funclass AssyncFileCopyTaskDynamic = (funclass) GetProcAddress(hDynamicLib, "AssyncFileCopyTaskDynamic");
	if (!AssyncFileCopyTaskDynamic) {
		printf("Get proc address err");
	} //подгрузка функции в класс обертку
	printf("Dynamic\n");
	AssyncFileCopyTaskDynamic();
	printf("Dynamic lib fun is cancel working\n");
	FreeLibrary(hDynamicLib);
	//освобождение библиотеки
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
