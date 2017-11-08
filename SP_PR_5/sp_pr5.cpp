#include "sp_pr5.h"

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


	g_hwndDlg = (HWND)0;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (g_hwndDlg == 0 || !IsDialogMessage(g_hwndDlg, &msg))
		{
			if (!TranslateAccelerator(hWnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
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
		HANDLE_MSG(hWnd, WM_MENUSELECT, km_OnMenuSelect);		// ��� ����� �������� ����
		HANDLE_MSG(hWnd, WM_CLOSE, km_OnClose);					// ����� �����������
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, km_OnLButtonDown);		// ������ ����� ������� ����
		HANDLE_MSG(hWnd, WM_RBUTTONDOWN, km_OnRButtonDown);		// ������ ������ ������� ����
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

/* ---------- ����������� ��������� -------------------------------- */

// WM_CREATE
BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct)
{
	MessageBox(hWnd, TEXT("����������� ��������� WM_CREATE"), g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

	g_lpszFileMenu = GetSubMenu(g_lpszMainMenu, 0);
	g_lpszViewMenu = GetSubMenu(g_lpszMainMenu, 1);


	RECT rc;
	GetClientRect(hWnd, &rc);

	hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VSCROLL | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL, rc.left, rc.top, rc.right, rc.bottom-50, hWnd, (HMENU)IDC_EDIT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	//SetFocus(hEdit);

	return TRUE;
}

// WM_LBUTTONDOWN
void  km_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
}

// WM_RBUTTONDOWN
void  km_OnRButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
}

// WM_ONPAINT
void km_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ��� ���������� ����
	RECT rc;
	GetClientRect(hWnd, &rc);

	SetWindowPos(hEdit, HWND_TOP, rc.left, rc.top, rc.right, rc.bottom-50, NULL);
	EndPaint(hWnd, &ps); // ���������� ���������� ����
}

// WM_ONCLOSE
void km_OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

// WM_MENUSELECT
void km_OnMenuSelect(HWND hWnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	HDC hDC;
	TCHAR Buf[300];
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	int size = LoadString(hInst, item, Buf, 300);
	hDC = GetDC(hWnd);

	RECT rcFill = rc;
	rcFill.top = rcFill.bottom - 30;
	rcFill.right = rcFill.left + 300;
	FillRect(hDC, &rcFill, CreateSolidBrush(RGB(255, 0, 0)));

	TextOut(hDC, rc.left + 10, rc.bottom - 30, Buf, lstrlen(Buf));
	ReleaseDC(hWnd, hDC);
}

// WM_DESTROY
void km_OnDestroy(HWND hWnd)
{
	MessageBox(hWnd, g_lpszDestroyMessage, g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

	PostQuitMessage(0); // ������� WM_QUIT ����������
}


// WM_COMMAND
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	TCHAR buff[200];
	
	DWORD dwNumbOfBytes = MAX_BYTES;
	HANDLE hFile = NULL;

	wsprintf(buff, TEXT("%d"), id);

	switch (id)
	{
		case IDC_EDIT:			// ���������� ���� Edit
		{

		}
		break;
		case IDM_FILE_OPEN:
		{
			OPENFILENAME ofn;   // ��������� ��� common dialog box

			//������������ OPENFILENAME
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;  // hwnd � ���������� ��������������
			ofn.lpstrFile = lpszFileSpec;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(lpszFileSpec);
			// ������������ ������� ����� �������� �������
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1; // ������ ��� �������� ������� �������
			ofn.lpstrFileTitle = NULL; // ��� ���������
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL; // � �������� ���������� ������� �������
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// ����������� ����������� ���� 
			BOOL fRet = GetOpenFileName(&ofn);
			if (fRet == FALSE) break;//������ � ������ 

			hFile = CreateFile(lpszFileSpec, GENERIC_READ, 0,
				NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(hWnd, TEXT("���� �� ������"), TEXT("������"), MB_OK | MB_ICONERROR);
				break;
			}

			ReadFile(hFile, Buffer, 100, &dwNumbOfBytes, NULL);

			if (hFile) CloseHandle(hFile);

		}
		break;
		case IDM_FILE_EXIT:
		{
			DestroyWindow(hWnd);
		}
		break;
		case IDM_VIEW_TEXT:		// ������� ��������-> �����
		{
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, ModLoadDlgProc);
		}
		break;
		case IDM_VIEW_BMP:		// ������� ��������-> �������
		{
			//MessageBox(hWnd, TEXT("������ IDM_VIEW_BMP"), buff, MB_OK);
			g_hwndDlg = CreateDialog((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG3), hWnd, (DLGPROC)LoadBmpDlgProc);
		}
		break;
		case IDM_HELP_ABOUT:
		{
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ModAboutDlgProc);
		}
		break;
		default:
		{
			MessageBox(hWnd, TEXT("������� �� �����������"), buff, MB_OK);
		}
		break;
	}
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
			wsprintf(buff1, TEXT("�������� ����: %d.%d.%d �. � ����� %d:%d:%d \n "), st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
			lstrcat(buff1, buff);
			SetDlgItemText(hDlg, IDC_STATIC1, buff1);
		}
		return TRUE;
		case WM_COMMAND:
		{
			int id = LOWORD(wParam);
			
			switch (id)
			{
				case IDOK:
				EndDialog(hDlg, IDOK);
			}
		}
		break;
		case WM_CLOSE:
		{
			EndDialog(hDlg, IDCANCEL);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK ModLoadDlgProc(HWND hDlg, UINT mes, WPARAM wParam, LPARAM lParam)
{
	switch (mes)
	{
		case WM_INITDIALOG:
		{
			SetDlgItemText(hDlg, IDC_EDIT1, "");
			SetDlgItemText(hDlg, IDC_EDIT1, Buffer);
		}
		return TRUE;
		case WM_COMMAND:
		{
			int id = LOWORD(wParam);
			
			switch (id)
			{
				case IDOK:
				EndDialog(hDlg, IDOK);
				break;

				case IDLOAD:
				{
					SetDlgItemText(hDlg, IDC_EDIT1, "");
					wsprintf(Buffer, TEXT("��� 2. ��������� �����"));
					SetDlgItemText(hDlg, IDC_EDIT1, Buffer);
				}
				break;
			}

		}
		return TRUE;

		case WM_CLOSE:
		EndDialog(hDlg, IDCANCEL);
		break;
	}
	return FALSE;
}


// ��������� ����������� ���� (BMP)
BOOL CALLBACK LoadBmpDlgProc(HWND hDlg, UINT mes, WPARAM wParam, LPARAM lParam)
{
	switch (mes)
	{ // ������������� ���������� ������
	case WM_INITDIALOG:
	{

	} return TRUE;
	case WM_COMMAND:
	{
		switch (wParam)
		{ // ��� ���� ������� ����������  ������ ������������ ����������� ����
		  // ������ � � ����������� �WM_DESTROY�
		  // ��������� �� ������ "OK"
		case IDOK:
			//{ ���������� ����������� ������ ...}
			// ��������� �� �Cancel� � ���������� ��� ����������
		case IDCANCEL:
		{ // ���������� ���������� ������
			DestroyWindow(hDlg);
			return TRUE;
		}
		}
	}
	}
	return FALSE;
}

