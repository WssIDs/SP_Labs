#include <Windows.h>
#include <tchar.h>
#include "resource.h"

//-- Prototypes -------------------
LRESULT CALLBACK SimWndProc(HWND, UINT, WPARAM, LPARAM);


//-- Global Variables ------------ 
//HMENU g_lpszMenu;

//  Стартовая функция 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = TEXT("VMainWindowClass");
	wc.lpfnWndProc = SimWndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
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

	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX;

	HMENU lpszMenu;
	lpszMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	hWnd = CreateWindowEx(NULL, wc.lpszClassName,
		TEXT("Володько Виталий Иванович"),
		Stl,
		200,
		200,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		lpszMenu,
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
LRESULT CALLBACK SimWndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{

	HDC hDC;

	switch (msg)
	{
		// Обработка меню
		case WM_COMMAND:
		{
			int id = LOWORD(wParam);
			int idNotification = HIWORD(wParam);

			TCHAR buff[200];
			wsprintf(buff, TEXT("%d"), id);

			switch (id)
			{
				case IDM_FILE_NEW:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_FILE_NEW"), buff, MB_OK);
				}
					break;
				case IDM_FILE_OPEN:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_FILE_OPEN"), buff, MB_OK);
				}
					break;
				case IDM_FILE_SAVE:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_FILE_SAVE"), buff, MB_OK);
				}
					break;
				case IDM_FILE_EXIT:
				{
					DestroyWindow(hWnd);
				}
					break;
				case IDM_EDIT_SELECT:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_EDIT_SELECT"), buff, MB_OK);
				}
					break;
				case IDM_EDIT_COPY:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_EDIT_COPY"), buff, MB_OK);
				}
					break;
				case IDM_EDIT_PASTE:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_FILE_NEW"), buff, MB_OK);
				}
					break;
				case IDM_HELP_ABOUT:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_HELP_ABOUT"), buff, MB_OK);
				}
					break;
				default:
				{
					MessageBox(hWnd, TEXT("Команда не реализована"), buff, MB_OK);
				}
					break;
			}
		}
		return 0;

		case WM_MENUSELECT:
		{
			int idNotification = HIWORD(wParam);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdc1;
			LPTSTR lpszMsgSpace = TEXT("____________________________");
			TCHAR Buf[300];
			HINSTANCE hInst;
			hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
			int size;
			size = LoadString(hInst, LOWORD(wParam), Buf, 300);
			hdc1 = GetDC(hWnd);
			TextOut(hdc1, rc.left + 10, rc.bottom - 30,
				lpszMsgSpace, lstrlen(lpszMsgSpace));
			TextOut(hdc1, rc.left + 10, rc.bottom - 30, Buf, lstrlen(Buf));
			ReleaseDC(hWnd, hdc1);
		}
		break;

		case WM_CREATE:
		{
			HMENU hMainMenu = GetMenu(hWnd);
			// Create a popup menu ready for use
			//HMENU popup_menu = CreatePopupMenu();
		}
		return 0;

		case WM_PAINT:    // Вывод при обновлении окна
		{
			PAINTSTRUCT ps;
			hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна
			EndPaint(hWnd, &ps); // Завершение обновления окна
		}
		return 0;

		case WM_CLOSE:  // Перед разрушением
		{
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:  // Завершение работы приложения
		PostQuitMessage(0); // Посылка WM_QUIT приложению
		break;

	default: // Вызов "Обработчика по умолчанию"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// Для ветвей с "break"
}