#include "sp_pr6_1.h"

//  Стартовая функция 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;

	if (!Register(hInstance)) return FALSE;

	hWnd = Create(hInstance, nCmdShow);

	if (hWnd == NULL) return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CLOSE, km_OnClose);					// Перед разрушением
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, km_OnLButtonDown);		// Нажата левая клавиша мыши
		HANDLE_MSG(hWnd, WM_LBUTTONUP, km_OnLButtonUp);			// Опущена левая клавига мыши
		HANDLE_MSG(hWnd, WM_MOUSEMOVE, km_OnMouseMove);
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
	wc.lpfnWndProc = Pr2_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
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
	Stl = WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX;

	HWND hWnd = CreateWindowEx(NULL, g_lpszClassName,
		g_lpszAplicationTitle,
		Stl,
		200,
		200,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return NULL;
	}

	g_HDC = GetDC(hWnd); // Получение контекста

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/* ---------- Обработчики сообщений -------------------------------- */

// WM_LBUTTONDOWN
void  km_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (!fDoubleClick) //Двойной щелчек
	{
		g_fDraw = TRUE;
		MoveToEx(g_HDC, x, y, NULL);
		SetCapture(hWnd); //Устанавливает захват мыши в заданном окне
	}
}

//=== Обработчик сообщения WM_LBUTTONUP ==============================//
void km_OnLButtonUp(HWND hWnd, int x, int y, UINT keyFlags)
{
	g_fDraw = FALSE;
	ReleaseCapture(); //Освобождает захват мыши окном в текущем потоке и восстанавливает обычную обработку ввода данных от мыши
}

// WM_MOUSEMOVE
void  km_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	char S[100];//Буфер для формирования выводимой строки(100 байт)
	HDC DC = GetDC(hwnd);//Получаем контекст устройства графического вывода

	wsprintf(S, "WM_MOUSEMOVE ==> x = %d y = %d keyFlags = %d    ", x, y, keyFlags);

	TextOut(DC, g_nPosX, g_nPosY + 80, S, strlen(S));// Выводим строку текста

	ReleaseDC(hwnd, DC);//Освобождаем контекст

	if (g_fDraw) LineTo(g_HDC, x, y);

}

// WM_ONCLOSE
void km_OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

// WM_DESTROY
void km_OnDestroy(HWND hWnd)
{
	ReleaseDC(hWnd, g_HDC); // Освобождение глобального контекстаы
	PostQuitMessage(0); // Посылка WM_QUIT приложению
}
