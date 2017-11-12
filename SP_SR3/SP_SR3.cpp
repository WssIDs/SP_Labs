#include "sp_sr3.h"

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
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			MessageBox(hWnd, TEXT("Выполняется обработка WM_CREATE"), g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

			g_lpszFileMenu = GetSubMenu(g_lpszMainMenu, 0);
			g_lpszEditMenu = GetSubMenu(g_lpszMainMenu, 1);

			CreateMenuItem(g_lpszFileMenu, "&Закрыть документ", 0, IDM_FILE_CLOSEDOC, NULL, FALSE, MFT_STRING);
		}
		return 0;
		case WM_LBUTTONDOWN: // Нажата левая клавиша мыши
		{
			HDC hdc = GetDC(hWnd);//получить контекст
			RECT rect;

			GetClientRect(hWnd, &rect);

			rect.left = LOWORD(lParam);
			rect.top = HIWORD(lParam);
			rect.bottom = rect.top + 300;

			TCHAR lpszBuff[200];
			wsprintf(lpszBuff, TEXT("Обработка сообщения WM_LBUTTONDOWN, которое посылается в окно при щелчке левой кнопки мыши."), TEXT("Hello, O World!"));

			DrawText(hdc, lpszBuff, -1, &rect, DT_LEFT);
			ReleaseDC(hWnd, hdc);//Освободить контекст
		}
		return 0;
		case WM_RBUTTONDOWN: // Нажата правая клавиша мыши
		{
			DWORD xyPos = GetMessagePos();
			WORD xPos = LOWORD(xyPos),
				yPos = HIWORD(xyPos);

			HMENU hPopupMenu = CreatePopupMenu();

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;

			GetMenuItemInfo(g_lpszEditMenu, IDM_EDIT_SELECT, FALSE, &mii);
			CreateMenuItem(hPopupMenu, TEXT("Выделить"), 0, IDM_EDIT_SELECT, NULL, FALSE, MFT_STRING);
			SetMenuItemInfo(hPopupMenu, IDM_EDIT_SELECT, FALSE, &mii);

			GetMenuItemInfo(g_lpszEditMenu, IDM_EDIT_COPY, FALSE, &mii);
			CreateMenuItem(hPopupMenu, TEXT("Копировать"), 0, IDM_EDIT_COPY, NULL, FALSE, MFT_STRING);
			SetMenuItemInfo(hPopupMenu, IDM_EDIT_COPY, FALSE, &mii);

			TrackPopupMenu(hPopupMenu, TPM_CENTERALIGN | TPM_LEFTBUTTON | TPM_VCENTERALIGN, xPos, yPos, 0, hWnd, NULL);

			DestroyMenu(hPopupMenu);
		}
		return 0;
		case WM_COMMAND: // Обработка меню
		{
			TCHAR buff[200];
			wsprintf(buff, TEXT("%d"), LOWORD(wParam));

			switch (LOWORD(wParam))
			{
				case IDM_FILE_NEW:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_FILE_NEW"), buff, MB_OK);

					if (g_lpszEditMenu)
					{
						MENUITEMINFO mii;

						mii.cbSize = sizeof(MENUITEMINFO);
						mii.fMask = MIIM_STATE;
						mii.fState = MFS_ENABLED;

						SetMenuItemInfo(g_lpszEditMenu, IDM_EDIT_SELECT, FALSE, &mii);
					}
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
				case IDM_FILE_CLOSEDOC:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_FILE_CLOSEDOC"), buff, MB_OK);
					if (g_lpszEditMenu)
					{
						MENUITEMINFO mii;
						mii.cbSize = sizeof(MENUITEMINFO);
						mii.fMask = MIIM_STATE;
						mii.fState = MFS_GRAYED;

						SetMenuItemInfo(g_lpszEditMenu, IDM_EDIT_COPY, FALSE, &mii);
						SetMenuItemInfo(g_lpszEditMenu, IDM_EDIT_SELECT, FALSE, &mii);
					}
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

					if (g_lpszEditMenu)
					{
						MENUITEMINFO mii;

						mii.cbSize = sizeof(MENUITEMINFO);
						mii.fMask = MIIM_STATE;
						mii.fState = MFS_ENABLED;

						SetMenuItemInfo(g_lpszEditMenu, IDM_EDIT_COPY, FALSE, &mii);
					}
				}
				break;
				case IDM_EDIT_COPY:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_EDIT_COPY"), buff, MB_OK);
				}
				break;
				case IDM_EDIT_PASTE:
				{
					MessageBox(hWnd, TEXT("Нажата IDM_FILE_PASTE"), buff, MB_OK);
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
		case WM_MENUSELECT: // При смене элеиента меню
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hDC;
			TCHAR Buf[300];
			HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
			int size = LoadString(hInst, LOWORD(wParam), Buf, 300);
			hDC = GetDC(hWnd);

			RECT rcFill = rc;
			rcFill.top = rcFill.bottom - 30;
			rcFill.right = rcFill.left + 300;
			FillRect(hDC, &rcFill, CreateSolidBrush(RGB(255, 0, 0)));

			TextOut(hDC, rc.left + 10, rc.bottom - 30, Buf, lstrlen(Buf));
			ReleaseDC(hWnd, hDC);
		}
		return 0;
		case WM_PAINT: // Вывод при обновлении окна
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна

			TCHAR lpszBuff[200];
			wsprintf(lpszBuff, TEXT("Вывод текста при обработке сообщения WM_PAINT. Это соообщение окно получает после того, как оно было закрыто другим окном и затем открыто."), TEXT("Hello, O World!"));
			TextOut(hDC, 20, 100, lpszBuff, lstrlen(lpszBuff));

			EndPaint(hWnd, &ps); // Завершение обновления окна
		}
		return 0;
		case WM_CLOSE: // Перед разрушением
		{
			DestroyWindow(hWnd);
		}
		return 0;
		case WM_DESTROY: // Завершение работы приложения
		{
			MessageBox(hWnd, g_lpszDestroyMessage, g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

			PostQuitMessage(0); // Посылка WM_QUIT приложению
		}
		return 0;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
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
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));
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

	return TRUE;
}

/* Создание окна */
HWND Create(HINSTANCE hInstance, int nCmdShow)
{
	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX;

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


/* Добавление элемента в меню */
BOOL CreateMenuItem(HMENU hMenu, char *str, UINT uIns, UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mii.fType = fType;
	mii.fState = MFS_ENABLED;
	mii.dwTypeData = str;
	mii.cch = sizeof(str);
	mii.wID = uCom;
	mii.hSubMenu = hSubMenu;
	return InsertMenuItem(hMenu, uIns, flag, &mii);
}