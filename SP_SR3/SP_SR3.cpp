#include "sp_sr3.h"

//  ��������� ������� 
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

// ������� ���������
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			MessageBox(hWnd, TEXT("����������� ��������� WM_CREATE"), g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

			g_lpszFileMenu = GetSubMenu(g_lpszMainMenu, 0);
			g_lpszEditMenu = GetSubMenu(g_lpszMainMenu, 1);

			CreateMenuItem(g_lpszFileMenu, "&������� ��������", 0, IDM_FILE_CLOSEDOC, NULL, FALSE, MFT_STRING);
		}
		return 0;
		case WM_LBUTTONDOWN: // ������ ����� ������� ����
		{
			HDC hdc = GetDC(hWnd);//�������� ��������
			RECT rect;

			GetClientRect(hWnd, &rect);

			rect.left = LOWORD(lParam);
			rect.top = HIWORD(lParam);
			rect.bottom = rect.top + 300;

			TCHAR lpszBuff[200];
			wsprintf(lpszBuff, TEXT("��������� ��������� WM_LBUTTONDOWN, ������� ���������� � ���� ��� ������ ����� ������ ����."), TEXT("Hello, O World!"));

			DrawText(hdc, lpszBuff, -1, &rect, DT_LEFT);
			ReleaseDC(hWnd, hdc);//���������� ��������
		}
		return 0;
		case WM_RBUTTONDOWN: // ������ ������ ������� ����
		{
			DWORD xyPos = GetMessagePos();
			WORD xPos = LOWORD(xyPos),
				yPos = HIWORD(xyPos);

			HMENU hPopupMenu = CreatePopupMenu();

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;

			GetMenuItemInfo(g_lpszEditMenu, IDM_EDIT_SELECT, FALSE, &mii);
			CreateMenuItem(hPopupMenu, TEXT("��������"), 0, IDM_EDIT_SELECT, NULL, FALSE, MFT_STRING);
			SetMenuItemInfo(hPopupMenu, IDM_EDIT_SELECT, FALSE, &mii);

			GetMenuItemInfo(g_lpszEditMenu, IDM_EDIT_COPY, FALSE, &mii);
			CreateMenuItem(hPopupMenu, TEXT("����������"), 0, IDM_EDIT_COPY, NULL, FALSE, MFT_STRING);
			SetMenuItemInfo(hPopupMenu, IDM_EDIT_COPY, FALSE, &mii);

			TrackPopupMenu(hPopupMenu, TPM_CENTERALIGN | TPM_LEFTBUTTON | TPM_VCENTERALIGN, xPos, yPos, 0, hWnd, NULL);

			DestroyMenu(hPopupMenu);
		}
		return 0;
		case WM_COMMAND: // ��������� ����
		{
			TCHAR buff[200];
			wsprintf(buff, TEXT("%d"), LOWORD(wParam));

			switch (LOWORD(wParam))
			{
				case IDM_FILE_NEW:
				{
					MessageBox(hWnd, TEXT("������ IDM_FILE_NEW"), buff, MB_OK);

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
					MessageBox(hWnd, TEXT("������ IDM_FILE_OPEN"), buff, MB_OK);
				}
				break;
				case IDM_FILE_SAVE:
				{
					MessageBox(hWnd, TEXT("������ IDM_FILE_SAVE"), buff, MB_OK);
				}
				break;
				case IDM_FILE_CLOSEDOC:
				{
					MessageBox(hWnd, TEXT("������ IDM_FILE_CLOSEDOC"), buff, MB_OK);
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
					MessageBox(hWnd, TEXT("������ IDM_EDIT_SELECT"), buff, MB_OK);

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
					MessageBox(hWnd, TEXT("������ IDM_EDIT_COPY"), buff, MB_OK);
				}
				break;
				case IDM_EDIT_PASTE:
				{
					MessageBox(hWnd, TEXT("������ IDM_FILE_PASTE"), buff, MB_OK);
				}
				break;
				case IDM_HELP_ABOUT:
				{
					MessageBox(hWnd, TEXT("������ IDM_HELP_ABOUT"), buff, MB_OK);
				}
				break;
				default:
				{
					MessageBox(hWnd, TEXT("������� �� �����������"), buff, MB_OK);
				}
				break;
			}
		}
		return 0;
		case WM_MENUSELECT: // ��� ����� �������� ����
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
		case WM_PAINT: // ����� ��� ���������� ����
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ��� ���������� ����

			TCHAR lpszBuff[200];
			wsprintf(lpszBuff, TEXT("����� ������ ��� ��������� ��������� WM_PAINT. ��� ���������� ���� �������� ����� ����, ��� ��� ���� ������� ������ ����� � ����� �������."), TEXT("Hello, O World!"));
			TextOut(hDC, 20, 100, lpszBuff, lstrlen(lpszBuff));

			EndPaint(hWnd, &ps); // ���������� ���������� ����
		}
		return 0;
		case WM_CLOSE: // ����� �����������
		{
			DestroyWindow(hWnd);
		}
		return 0;
		case WM_DESTROY: // ���������� ������ ����������
		{
			MessageBox(hWnd, g_lpszDestroyMessage, g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

			PostQuitMessage(0); // ������� WM_QUIT ����������
		}
		return 0;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}


/* ����������� ������ ���� */
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
		MessageBox(NULL, TEXT("������ ����������� ������ ����!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

/* �������� ���� */
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
		MessageBox(NULL, TEXT("���� �� �������!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}


/* ���������� �������� � ���� */
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