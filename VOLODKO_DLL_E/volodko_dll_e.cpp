
#include "volodko_dll_e.h"

//  Стартовая функция 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;

	if (!Register(hInstance)) return FALSE;

	hWnd = Create(hInstance, nCmdShow);

	if (hWnd == NULL) return FALSE;

	HACCEL hAccel;
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_COMMAND, km_OnCommand);				// Обработка меню
		HANDLE_MSG(hWnd, WM_MENUSELECT, km_OnMenuSelect);		// При смене элемента меню
		HANDLE_MSG(hWnd, WM_CLOSE, km_OnClose);					// Перед разрушением
		HANDLE_MSG(hWnd, WM_CREATE, km_OnCreate);				// Вывод при обновлении окна
		HANDLE_MSG(hWnd, WM_PAINT, km_OnPaint);					// Вывод при обновлении окна
		HANDLE_MSG(hWnd, WM_DESTROY, km_OnDestroy);				// Завершение работы приложения
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


/* Регистрация класса окна */
BOOL Register(HINSTANCE hInst)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

/* Создание окна */
HWND Create(HINSTANCE hInstance, int nCmdShow)
{
	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW;

	g_lpszMainMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	HWND hWnd = CreateWindowEx(NULL, g_lpszClassName,
		g_lpszAplicationTitle,
		Stl,
		200,
		200,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		g_lpszMainMenu,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/* ---------- Обработчики сообщений -------------------------------- */

// WM_CREATE
BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct)
{
	HINSTANCE hLib = LoadLibrary(TEXT("../DEBUG/VOLODKO.dll"));
	if (hLib == NULL)
	{
		MessageBox(NULL, TEXT("Ошибка загрузки библиотеки"), NULL, MB_OK);
		return FALSE;
	}
	else
	{
		pg_nFnCallsCount = (int *)GetProcAddress(hLib, "g_nFnCallsCount");
		pg_nDllCallsCount = (int *)GetProcAddress(hLib, "g_nDllCallsCount");
		Fun11 = (LPFN_FUN11)GetProcAddress(hLib, "Fun11");

		Fun12 = (LPFN_FUN12)GetProcAddress(hLib,MAKEINTRESOURCEA(2));
		//Fun12 = (LPFN_FUN12)GetProcAddress(hLib, "Fun12");
		Fun13 = (LPFN_FUN13)GetProcAddress(hLib, "Fun13");

		if (!Fun11 || !Fun12 || !Fun13 || !pg_nFnCallsCount || !pg_nDllCallsCount)
		{// handle the error
			FreeLibrary(hLib);
			MessageBox(NULL, TEXT("Ошибка загрузки функции/переменной из библиотеки"), NULL, MB_OK);
		}
	}

	return TRUE;
}

// WM_ONPAINT
void km_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна

	EndPaint(hWnd, &ps); // Завершение обновления окна
}

// WM_ONCLOSE
void km_OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

// WM_MENUSELECT
void km_OnMenuSelect(HWND hWnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
}

// WM_DESTROY
void km_OnDestroy(HWND hWnd)
{
	PostQuitMessage(0); // Посылка WM_QUIT приложению
}

#include "stdio.h"

// WM_COMMAND
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	TCHAR buff[200];
	wsprintf(buff, TEXT("%d"), id);

	switch (id)
	{
		case IDM_FILE_EXIT:
		{
			DestroyWindow(hWnd);
		}
		break;
		case IDM_LIB_FUNC11:
		{
			(*pg_nFnCallsCount)++;

			int a = 4;
			int b = 3;

			int c = Fun11(a, b);

			sprintf_s(buff, TEXT("a = %d, b = %d\nРезультат: %d\nВызовов библиотеки: %d, Вызовов функций: %d"), a, b, c, *pg_nDllCallsCount, *pg_nFnCallsCount);

			MessageBox(hWnd, buff, TEXT("Сообщение из библиотеки DLL"), MB_OK);
		}
		break;
		case IDM_LIB_FUNC12:
		{
			(*pg_nFnCallsCount)++;

			int a = 4;
			int b = 3;
			int c = 6;
			float d = Fun12(a, b, c);

			sprintf_s(buff, TEXT("a = %d, b = %d, c = %d\nРезультат: %.3f\nВызовов библиотеки: %d, Вызовов функций: %d"), a, b, c, d, *pg_nDllCallsCount, *pg_nFnCallsCount);

			MessageBox(hWnd, buff, TEXT("Сообщение из библиотеки DLL"), MB_OK);
		}
		break;
		case IDM_LIB_FUNC13:
		{
			(*pg_nFnCallsCount)++;

			double a = 4.0;
			double b;

			Fun13(a, &b);

			sprintf_s(buff, TEXT("a = %.3f, b = %.3f\nВызовов библиотеки: %d, Вызовов функций: %d "), a, b, *pg_nDllCallsCount, *pg_nFnCallsCount);

			MessageBox(hWnd, buff, TEXT("Сообщение из библиотеки DLL"), MB_OK);
		}
		break;

		default:
		{
			MessageBox(hWnd, TEXT("Команда не реализована"), buff, MB_OK);
		}
		break;
	}
}
