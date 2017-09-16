#include <Windows.h>
#include <tchar.h>
#include "resource.h"

//-- Prototypes -------------------
LRESULT CALLBACK SimWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ModAboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ModLoadDlgProc(HWND, UINT, WPARAM, LPARAM);

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
					DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, ModLoadDlgProc, 0);
					//MessageBox(hWnd, TEXT("Нажата IDM_FILE_OPEN"), buff, MB_OK);
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
					DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ModAboutDlgProc, 0);

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




BOOL CALLBACK ModAboutDlgProc(HWND hDlg, UINT mes, WPARAM wParam, LPARAM lParam)
{
	switch (mes)
	{
	case WM_INITDIALOG:
	{
		TCHAR buff[200];
		GetDlgItemText(hDlg, IDC_STATIC1, buff, 200);
		//MessageBox(hDlg, buff, NULL, MB_OK);
		SYSTEMTIME st;
		GetLocalTime(&st);
		TCHAR buff1[200];
		wsprintf(buff1, TEXT("Текущаяя дата: %d.%d.%d г. и время %d:%d:%d \n "), st.wDay, st.wMonth, st.wYear,st.wHour,st.wMinute,st.wSecond);
		lstrcat(buff1, buff);
		SetDlgItemText(hDlg, IDC_STATIC1, buff1);

	}return TRUE;
	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDOK:
			EndDialog(hDlg, IDOK);
		}


	}return TRUE;

	}

	return FALSE;
}

BOOL CALLBACK ModLoadDlgProc(HWND hDlg, UINT mes, WPARAM wParam, LPARAM lParam)
{
	switch (mes)
	{
	case WM_INITDIALOG:
	{
	}return TRUE;
	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDOK:
			EndDialog(hDlg, IDOK);
			break;

		case IDLOAD:
			MessageBox(hDlg, TEXT("Нажата клавиша IDLOAD"), "IDLOAD", MB_OK);
			break;
		}

	}return TRUE;
	
	case WM_CLOSE:
		EndDialog(hDlg, IDCANCEL);
		break;
		



	}

	return FALSE;
}
