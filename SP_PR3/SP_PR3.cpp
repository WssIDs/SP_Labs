#include "sp_pr3.h"

//  ��������� ������� 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;
	g_hInst = hInstance;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = TEXT("VMainWindowClass");
	wc.lpfnWndProc = Pr2_WndProc;
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
		MessageBox(NULL, TEXT("������ ����������� ������ ����!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX;

	HMENU lpszMenu;
	lpszMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	hWnd = CreateWindowEx(NULL, wc.lpszClassName,
		g_lpszAplicationTitle,
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
		MessageBox(NULL, TEXT("���� �� �������!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
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

// ������� ���������
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	static HWND hList;
	HDC hDC;

	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			HDC hdc = GetDC(hWnd);//�������� ��������
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			RECT rect;

			GetClientRect(hWnd, &rect);

			rect.left = x;
			rect.top = y;
			rect.bottom = rect.top + 300;

			TCHAR lpszBuff[200];
			wsprintf(lpszBuff, TEXT("��������� ��������� WM_LBUTTONDOWN, ������� ���������� � ���� ��� ������ ����� ������ ����."), TEXT("Hello, O World!"));

			DrawText(hdc, lpszBuff, -1, &rect, DT_LEFT);
			ReleaseDC(hWnd, hdc);//���������� ��������
		}
		return 0;

			// ��������� ����
		case WM_COMMAND:
		{
			int id = LOWORD(wParam);
			int idNotification = HIWORD(wParam);

			TCHAR buff[200];
			wsprintf(buff, TEXT("%d"), id);

			switch (id)
			{
				case IDC_LIST:
				{
					switch (idNotification)
					{
						case LBN_SELCHANGE: // ���� �� �������� ������ Listbox
						{
							LPTSTR LBString = new TCHAR[1024];
							int selIndex = 0;

							// �������� ������ ������� ������
							selIndex = SendMessage(hList, LB_GETCURSEL, wParam, NULL);

							// �������� ����� ������� ������
							selIndex = SendMessage(hList, LB_GETTEXT, selIndex, (LPARAM)LBString);
							MessageBox(hWnd, LBString, TEXT("��������� �������� ����������"), MB_OK | MB_ICONINFORMATION);
							delete[]LBString;
						}
						break;
					}
				}
				break;

				case IDM_FILE_NEW:
				{
					MessageBox(hWnd, TEXT("������ IDM_FILE_NEW"), buff, MB_OK);
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
				case IDM_FILE_EXIT:
				{
					DestroyWindow(hWnd);
				}
				break;
				case IDM_EDIT_SELECT:
				{
					MessageBox(hWnd, TEXT("������ IDM_EDIT_SELECT"), buff, MB_OK);
				}
				break;
				case IDM_EDIT_COPY:
				{
					MessageBox(hWnd, TEXT("������ IDM_EDIT_COPY"), buff, MB_OK);
				}
				break;
				case IDM_EDIT_PASTE:
				{
					MessageBox(hWnd, TEXT("������ IDM_FILE_NEW"), buff, MB_OK);
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

			HGDIOBJ original = NULL;;

			original = SelectObject(hdc1, GetStockObject(DC_PEN));
			SetDCBrushColor(hdc1, RGB(0, 255, 0));
			SetDCPenColor(hdc1, RGB(0, 0, 255));

			//SelectObject(hdc1, GetStockObject(DC_BRUSH));
			Rectangle(hdc1, rc.left, rc.bottom - 40, 300, rc.bottom);
			SelectObject(hdc1, original);

			TextOut(hdc1, rc.left + 10, rc.bottom - 30, Buf, lstrlen(Buf));
			ReleaseDC(hWnd, hdc1);
		}
		return 0;

		case WM_CREATE:
		{
			MessageBox(hWnd, TEXT("����������� ��������� WM_CREATE"), g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

			hList = CreateWindowEx(NULL, TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD, 20, 210, 200, 100, hWnd, (HMENU)IDC_LIST, g_hInst, NULL);

			//// ��������� � ������ ��������� �����
			SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)"������ �������� ������");
			SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)"������ �������� ������");
			SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)"������ �������� ������");
		}
		return 0;

		case WM_PAINT:    // ����� ��� ���������� ����
		{
			PAINTSTRUCT ps;
			hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ��� ���������� ����

			TCHAR lpszBuff[200];
			wsprintf(lpszBuff, TEXT("����� ������ ��� ��������� ��������� WM_PAINT. ��� ���������� ���� �������� ����� ����, ��� ��� ���� ������� ������ ����� � ����� �������."), TEXT("Hello, O World!"));
			TextOut(hDC, 20, 100, lpszBuff, lstrlen(lpszBuff));

			EndPaint(hWnd, &ps); // ���������� ���������� ����
		}
		return 0;

		case WM_CLOSE:  // ����� �����������
		{
			DestroyWindow(hWnd);
		}
		return 0;

		case WM_DESTROY:  // ���������� ������ ����������
		{
			MessageBox(hWnd, g_lpszDestroyMessage, g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

			PostQuitMessage(0); // ������� WM_QUIT ����������
		}
		return 0;

		default: // ����� "����������� �� ���������"
			return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// ��� ������ � "break"
}