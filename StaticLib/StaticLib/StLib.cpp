#include "pch.h"
#include <cstdio>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <locale>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include "Header1.h"

//client global var
using namespace std;

HANDLE hMailSlot;
//char szBuf1[512];
DWORD cbWritten;
//LPCWSTR lpszMailSlotName = L"\\\\.\\mailslot\\mailslot";
/// <summary>
/// //////////////////////////////////////////////////////////
/// </summary>

BOOL fReturnCode;
DWORD cbMessages;
DWORD cbMsgNumber;
HANDLE hMailslot;
HANDLE ringers[5];
LPCWSTR lpszMailSlotName = L"\\\\.\\mailslot\\mailslot";
char szBuf[512];
DWORD cbRead;
int initialCount = 0;
int lock = 0;

LARGE_INTEGER ringer(int h, int m);
void startClient();

VOID CALLBACK TimerAPCProc(
	LPVOID lpArgToCompletionRoutine,
	DWORD dwTimerLowValue,
	DWORD dwTimerHighValue
);
//void swap(char** buf1, char** buf2)
//{
//	char* temp = *buf1;
//	*buf1 = *buf2;
//	*buf2 = temp;
//}
extern "C" __declspec(dllexport) void AssyncFileCopyTaskStatic()
{
	hMailslot = CreateMailslot(lpszMailSlotName, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (hMailslot == INVALID_HANDLE_VALUE)
	{
		printf("Error MailSlot Creating!\n");
		_getch();
		return ;
	}
	printf("This is server! VT ver:2.0.15\n");
	startClient();
	while (1)
	{
		fReturnCode = GetMailslotInfo(hMailslot, NULL, &cbMessages, &cbMsgNumber, NULL);
		if (!fReturnCode)
		{
			printf("Get MailSlotInfo Error!\n");
			_getch();
			return;
		}

		if (cbMsgNumber != 0)
		{
			if (ReadFile(hMailslot, szBuf, 512, &cbRead, NULL))
			{
				printf("Received %d bytes: <%s>\n", cbRead, szBuf);
#pragma region setTimer
				int hour = 0, minute = 0;
				LARGE_INTEGER lnTime;
				sscanf_s(szBuf, "%d %d", &hour, &minute);

				lnTime = ringer(hour, minute);
				ringers[initialCount] = CreateWaitableTimer(NULL, TRUE, L"SPO_Timer");
				if (!SetWaitableTimer(ringers[initialCount], &lnTime, 0, TimerAPCProc, NULL, FALSE))
				{
					printf("Error at set timer\n");
					return;
				}
				initialCount++;
#pragma endregion 
			}
			else
				printf("Error Data Transfer!\n");
		}
		if (initialCount != lock) {
			while (1)
			{
				static int nCount = 0;
				// Set thread to alertable state
				SleepEx(INFINITE, TRUE);
				break;
			}
			lock++;
		}
		break;
	}
	CloseHandle(hMailslot);
}

void startClient() {
	hMailSlot = CreateFile(lpszMailSlotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile Error!\n");
		_getch();
		return ;
	}
	while (1) {
		//scanf_s("%s %s", &szBuf1);
		printf("Enter time to set ringer: ");
		gets_s(szBuf);
		//strcpy_s(szBuf1, "Test message from console");
		if (!WriteFile(hMailSlot, szBuf, strlen(szBuf) + 1, &cbWritten, NULL))
			printf("Error Data Transfer!\n");
		else {
			printf("Transferred %d bytes: <%s>\n", cbWritten, szBuf);
			break;
		}
	}
	//CloseHandle(hMailSlot);
	return ;
}

VOID CALLBACK TimerAPCProc(
	LPVOID lpArgToCompletionRoutine,
	DWORD dwTimerLowValue,
	DWORD dwTimerHighValue
)
{
	MessageBeep(10);
	printf("good morning!");
	return;
}
LARGE_INTEGER ringer(int h, int m) {
	struct tm* u = new tm;
	const time_t timer = time(NULL);
	localtime_s(u, &timer);
	const int nTimerUnitsPerSecond = 10000000;
	LARGE_INTEGER ring;
	long time;

	time = (h - u->tm_hour) * 60 * 60
		+ (m - u->tm_min) * 60;
	ring.QuadPart = -(time * nTimerUnitsPerSecond);

	return ring;
}


//void returnLastFoundFolder(char* temp)
//{
//	int iStart = 0;
//	int iEnd = strlen(folders);
//
//	iStart = strrchr(folders, '|') - folders;
//	char* c = &folders[iStart + 1];
//
//	strcpy(temp, c);
//	folders[iStart] = 0;
//}
//
//void recursive_find_files(char* path, OVERLAPPED ovr_write, HANDLE file = INVALID_HANDLE_VALUE)
//{
//	if (!openFile) {
//		file = CreateFile(L"file.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);
//		if (file == INVALID_HANDLE_VALUE)
//			return;
//		else {
//			openFile = true;
//			sprintf_s(folders, "|%s", path);
//		}
//	}
//	char temp[MAX_PATH] = { '\0' };
//	char pathh[MAX_PATH] = { '\0' };
//	char dir[MAX_PATH] = { '\0' };
//	char fileName[MAX_PATH] = { '\0' };
//
//	HANDLE handle;
//
//	if (strlen(folders) > 1)
//	{
//		returnLastFoundFolder(temp);
//		sprintf_s(pathh, "%s\\*.*", temp);
//		WIN32_FIND_DATA fd;
//		handle = FindFirstFile((LPCWSTR)pathh, &fd);
//		if (handle != INVALID_HANDLE_VALUE)
//		{
//			do
//			{
//				if (strcmp(".", (const char*)fd.cFileName) != 0 && strcmp("..", (const char*)fd.cFileName) != 0)
//				{
//					if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
//					{
//						printf("dir \t%s\\%s\n", temp, fd.cFileName);
//						sprintf_s(dir, "|%s\\%s", temp, fd.cFileName);
//						strcat(folders, dir);
//					}
//					else
//					{
//						count++;
//						printf("file \t%s\\%s\n", temp, fd.cFileName);
//						sprintf_s(fileName, "%d %s\\%s\r\n", count, temp, fd.cFileName);
//						strcat(buf1, fileName);
//					}
//				}
//			} while (FindNextFile(handle, &fd));
//
//			FindClose(handle);
//
//			if (strlen(buf1) > 0)
//			{
//				DWORD dwWritten;
//				int size;
//				WaitForSingleObject(ovr_write.hEvent, INFINITE);
//
//				ovr_write.Offset = sizeTextBuf;
//
//				strcpy(buf2, buf1);
//				buf1[0] = 0;
//
//				size = strlen(buf2);
//				sizeTextBuf += size;
//
//				WriteFile(file, buf2, size, &dwWritten, &ovr_write);
//
//			}
//
//			recursive_find_files(temp, ovr_write, file);
//		}
//	}
//
//	if (openFile)
//	{
//		CloseHandle(file);
//		openFile = false;
//	}
//}
//
//extern "C" __declspec(dllexport) int FindFileAndWriteFile(char* dirName) {
//
//	HANDLE file;
//	setlocale(0, "rus");
//	char* path = { dirName };
//	OVERLAPPED ovr_write = { 0 };
//	ovr_write.hEvent = CreateEvent(NULL, false, true, NULL);
//	ovr_write.Offset = 0;
//	recursive_find_files(path, ovr_write);
//	printf("\n %d\n", count);
//	getchar();
//	return 0;
//
//}


