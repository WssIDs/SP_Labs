#include "sp_sr5.h"

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
		HANDLE_MSG(hWnd, WM_COMMAND, km_OnCommand);				// ��������� ����
		HANDLE_MSG(hWnd, WM_CLOSE, km_OnClose);					// ����� �����������
		HANDLE_MSG(hWnd, WM_CREATE, km_OnCreate);				// ����� ��� ���������� ����
		HANDLE_MSG(hWnd, WM_PAINT, km_OnPaint);					// ����� ��� ���������� ����
		HANDLE_MSG(hWnd, WM_DESTROY, km_OnDestroy);				// ���������� ������ ����������
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
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

	HWND hWnd = CreateWindowEx(NULL, g_lpszClassName,
		g_lpszAplicationTitle,
		Stl,
		200,
		200,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)),
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

/* ---------- ����������� ��������� -------------------------------- */

// WM_CREATE
BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct)
{
	return TRUE;
}
// WM_ONPAINT
void km_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ��� ���������� ����

	EndPaint(hWnd, &ps); // ���������� ���������� ����
}

// WM_ONCLOSE
void km_OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

// WM_DESTROY
void km_OnDestroy(HWND hWnd)
{
	PostQuitMessage(0); // ������� WM_QUIT ����������
}


// WM_COMMAND
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	TCHAR buff[200];
	wsprintf(buff, TEXT("%d"), id);

	switch (id)
	{
		case IDM_FILE_NEW:
		{
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ModDlgProc);
		}
		break;
		case IDM_FILE_EXIT:
		{
			DestroyWindow(hWnd);
		}
		break;
		default:
		{
			MessageBox(hWnd, TEXT("������� �� �����������"), buff, MB_OK);
		}
		break;
	}
}


BOOL CALLBACK ModDlgProc(HWND hDlg, UINT mes, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	switch (mes)
	{
		case WM_INITDIALOG:
		{
			if (!(g_hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"Edit", "��������", WS_CHILD | WS_BORDER | WS_VISIBLE, 20, 50, 160, 40, hDlg, (HMENU)(IDC_EDIT1), (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE), NULL))) return FALSE;
			if (!(g_hListBox = CreateWindowEx(WS_EX_CLIENTEDGE,"ListBox", "������", WS_CHILD | WS_BORDER | WS_VISIBLE, 200, 50, 160, 180, hDlg, (HMENU)(IDC_LISTBOX), (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE), NULL))) return FALSE;
		}
		return TRUE;
		case WM_COMMAND:
		{
			int id = LOWORD(wParam);
			int idNotification = HIWORD(wParam);
			static TCHAR pszTextBuff[500];

			switch (id)
			{
				case IDCANCEL: EndDialog(hDlg,IDCANCEL); break;

				case IDC_BTN_SAVE:
				{ 
					int  cch; cch = SendMessage(g_hEdit, WM_GETTEXT, 500, (LPARAM)pszTextBuff); 
					
					if (cch == 0)
					{
						MessageBox(hDlg, "������� �����", "������ Edit", MB_OK);
					}
					else
					{
						TCHAR Buff1[500] = { 0 };
						SYSTEMTIME st; GetSystemTime(&st); wsprintf(Buff1, "����� : %d � %d ��� %d ���\n", st.wHour, st.wMinute, st.wSecond); lstrcat(Buff1, __TEXT("����� � ������: ")); lstrcat(Buff1, pszTextBuff); MessageBox(hDlg, Buff1, "���������� ������", MB_OK);
					}
				}
				break;
				case IDC_BTN_ADD:
				{ 
					int ind;
					ind = SendMessage(g_hListBox, LB_ADDSTRING, 0, (LPARAM)pszTextBuff); if (ind == LB_ERR) MessageBox(hDlg, "������ � ������", "", MB_OK);
				} 
				break;
			}
		}
		break;
		case WM_PAINT:
		{
			TCHAR szHello[MAX_LOADSTRING];
			LoadString((HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE), IDS_HELLO, szHello, MAX_LOADSTRING);
			PAINTSTRUCT ps;
			hdc = BeginPaint(hDlg, &ps); 
			// TODO: Add any drawing code here...
			RECT rt; GetClientRect(hDlg, &rt);
			DrawText(hdc, szHello, strlen(szHello),&rt, DT_CENTER);
			EndPaint(hDlg, &ps); break;
		}
		case WM_CLOSE:
		{
			EndDialog(hDlg, IDCANCEL);
		}
		return TRUE;
	}
	return FALSE;
}


