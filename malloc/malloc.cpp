// malloc.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <process.h>


#define	MEM_SIZE	50			// memory size
#define COUNT		400000		// times to allocate memory
#define LOOP		50

void __stdcall newThread(void * pParam);
void __stdcall mallocThread(void * pParam);

int main()
{
    std::cout << "Performance between NEW and MALLOC:\n";

	HANDLE hThread[2];
	unsigned threadID;

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))newThread, NULL, 0, &threadID);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))mallocThread, NULL, 0, &threadID);

	int rval = WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	return 0;
}

void __stdcall newThread(void * pParam)
{
	int	n, *ptr;
	DWORD	i, tick1, tick2, sum = 0;

	for (n = 0; n < LOOP; n++)
	{
		tick1 = GetTickCount();
		for (i = 0; i < COUNT; i++)
		{
			ptr = new int[MEM_SIZE*(n + 1)];
			if (ptr)
			{
				delete ptr;
			}
			else
				std::cout << "new fail...";
		}

		tick2 = GetTickCount();
		sum += (tick2 - tick1);
		printf("%d: NEW size %d bytes took %d ms\n", n, MEM_SIZE*(n + 1)*sizeof(int), (tick2 - tick1));
	}

	std::cout << "newThread took " << sum << " ms\n";
}

void __stdcall mallocThread(void * pParam)
{
	int	n, *ptr;
	DWORD	i, tick1, tick2, sum = 0;

	for (n = 0; n < LOOP; n++)
	{
		tick1 = GetTickCount();
		for (i = 0; i < COUNT; i++)
		{
			ptr = (int*)malloc(sizeof(int)*MEM_SIZE*(n + 1));
//			ptr = (int*)calloc(MEM_SIZE*(n + 1), sizeof(int));
			if (ptr)
			{
				delete ptr;
			}
			else
				std::cout << "malloc fail...";
		}

		tick2 = GetTickCount();
		sum += (tick2 - tick1);
		printf("%d: MALLOC size %d bytes took %d ms\n", n, MEM_SIZE*(n + 1) * sizeof(int), (tick2 - tick1));
	}

	std::cout << "mallocThread took " << sum << " ms\n";
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的秘訣: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
