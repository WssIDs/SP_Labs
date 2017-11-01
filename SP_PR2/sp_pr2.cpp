#include <Windows.h>
#include <tchar.h>

//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);


//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPTSTR g_lpszlistboxClassName = TEXT("Button");
LPTSTR g_lpszAplicationTitle = TEXT("Разработчик: Володько Виталий, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("Вариант 1");
LPTSTR g_lpszDestroyMessage = TEXT("Данный вывод выполнен в обработчике сообщения WM_DESTROY. Сообщение поступило от Windows в связи с разрушением окна.");
HINSTANCE g_hInst;

//  Стартовая функция 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;
	g_hInst = hInstance;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = Pr2_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(255, 0, 0));
	wc.hbrBackground = hbr;

	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hWnd = CreateWindowEx(NULL, wc.lpszClassName,
		g_lpszAplicationTitle,
		WS_OVERLAPPEDWINDOW,
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
		return FALSE;
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	static HWND hList;
	HDC hDC;

	switch (msg)
	{
	case WM_CREATE:
	{
		MessageBox(hWnd, TEXT("Выполняется обработка WM_CREATE"), g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

		hList = CreateWindowEx(NULL,TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD, 20, 210, 200, 100, hWnd, NULL, g_hInst, NULL);

		//// Добавляем в список несколько строк
		SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)"Первая тестовая запись");
		SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)"Вторая тестовая запись");
		SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)"Третяя тестовая запись");
	}
	return 0;
	
	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
			case LBN_SELCHANGE: // Клик по элементу списка Listbox
			{
				LPTSTR LBString = new TCHAR[1024];
				int selIndex = 0;
				
				// Получаем индекс текущей ячейки
				selIndex = SendMessage(hList, LB_GETCURSEL, wParam, NULL);

				// Получаем текст текущей ячейки
				selIndex = SendMessage(hList, LB_GETTEXT, selIndex, (LPARAM)LBString);
				MessageBox(hWnd, LBString, TEXT("Сообщение элемента управления"), MB_OK | MB_ICONINFORMATION);
				delete[]LBString;

				break;
			}
		}
	}
	return 0;

	case WM_LBUTTONDOWN:
	{
		HDC hdc = GetDC(hWnd);//получить контекст
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		RECT rect;
		
		GetClientRect(hWnd, &rect);		
		
		rect.left = x; 
		rect.top = y;
		rect.bottom = rect.top + 300;

		TCHAR lpszBuff[200];
		wsprintf(lpszBuff, TEXT("Обработка сообщения WM_LBUTTONDOWN, которое посылается в окно при щелчке левой кнопки мыши."), TEXT("Hello, O World!"));

		DrawText(hdc, lpszBuff, -1, &rect, DT_LEFT);
		ReleaseDC(hWnd, hdc);//Освободить контекст
	}
	return 0;

	case WM_PAINT:    // Вывод при обновлении окна
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна

		TCHAR lpszBuff[200];
		wsprintf(lpszBuff, TEXT("Вывод текста при обработке сообщения WM_PAINT. Это соообщение окно получает после того, как оно было закрыто другим окном и затем открыто."), TEXT("Hello, O World!"));
		TextOut(hDC, 20, 100, lpszBuff, lstrlen(lpszBuff));

		EndPaint(hWnd, &ps); // Завершение обновления окна
	}
	return 0;

	case WM_DESTROY:  // Завершение работы приложения
	{
		MessageBox(hWnd, g_lpszDestroyMessage, g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

		PostQuitMessage(0); // Посылка WM_QUIT приложению
	}
	return 0;

	default: // Вызов "Обработчика по умолчанию"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// Для ветвей с "break"
}