#include "sp_pr2-2.h"


void for_delay(int param)
{
	double x, y, z; x = 2.0; y = 3.0;
	for (int i = 0; i<param; i++) { z = x*y - 1; z = z + 1; }
}


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
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_COMMAND, km_OnCommand);				// ��������� ����
		HANDLE_MSG(hWnd, WM_MENUSELECT, km_OnMenuSelect);		// ��� ����� �������� ����
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
	// ������� �������
	g_lpProcess[0].ProcImage = TEXT("");
	wsprintf(g_lpProcess[0].CmdParam, TEXT("%s"), g_lpProcess[0].ProcImage);

	g_lpProcess[0].ProcId = GetCurrentProcessId();
	g_lpProcess[0].ProcHandle = GetCurrentProcess();
	g_lpProcess[0].ThreadHandle = GetCurrentThread();
	g_lpProcess[0].ThreadId = GetCurrentThreadId();
	
	// �������
	g_lpProcess[1].ProcImage = TEXT("Notepad");
	wsprintf(g_lpProcess[1].CmdParam, TEXT("%s"), g_lpProcess[1].ProcImage);

	g_lpProcess[2].ProcImage = TEXT("Notepad");
	wsprintf(g_lpProcess[2].CmdParam, TEXT("%s %s"), g_lpProcess[2].ProcImage, TEXT("sp_pr2-1.cpp"));

	// �����������
	g_lpProcess[3].ProcImage = TEXT("Calc");
	wsprintf(g_lpProcess[3].CmdParam, TEXT("%s"), g_lpProcess[3].ProcImage);



	ThrParam1.hWnd = hWnd;
	ThrParam2.hWnd = hWnd;


	g_lpszThread1Menu = GetSubMenu(g_lpszMainMenu, 1);

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
	for (int i = 0; i < ARRAYSIZE(g_lpThread); i++)
	{
		if (TerminateThread(g_lpThread[i].ThreadHandle, 0))
		{
			if (CloseHandle(g_lpThread[i].ThreadHandle))
			{
				g_lpThread[i].ThreadHandle = NULL;
			}
		}
	}

	DestroyWindow(hWnd);
}

// WM_MENUSELECT
void km_OnMenuSelect(HWND hWnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
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
	DWORD exitCode = 0;

	switch (id)
	{
		case IDM_FILE_EXIT:
		{
			DestroyWindow(hWnd);
		}
		break;
		case IDM_THREAD1_CREATE: // ������� �����
		{
			g_lpThread[0].ThreadHandle = CreateThread(NULL, 0L,ThreadFunc1, &ThrParam1, 0L, &g_lpThread[0].ThreadId);

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_GRAYED;

			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATE, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATEWAIT, FALSE, &mii);

			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_SUSPEND, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_RESUME, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DESTROY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_UPPRIORITY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DOWNPRIORITY, FALSE, &mii);

		}
		break;
		case IDM_THREAD1_CREATEWAIT: // ������� ������ �����
		{
			g_lpThread[0].ThreadHandle = CreateThread(NULL, 0L, ThreadFuncWait, &ThrParam1, 0L, &g_lpThread[0].ThreadId);

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_GRAYED;

			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATE, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATEWAIT, FALSE, &mii);

			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_SUSPEND, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_RESUME, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DESTROY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_UPPRIORITY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DOWNPRIORITY, FALSE, &mii);
		}
		break;
		case IDM_THREAD1_SUSPEND: // �������������
		{
			SuspendThread(g_lpThread[0].ThreadHandle);
		}
		break;
		case IDM_THREAD1_RESUME: // ����������
		{
			ResumeThread(g_lpThread[0].ThreadHandle);
		}
		break;
		case IDM_THREAD1_DESTROY: // ����������
		{
			if (TerminateThread(g_lpThread[0].ThreadHandle, 0))
			{
				if (CloseHandle(g_lpThread[0].ThreadHandle))
				{
					g_lpThread[0].ThreadHandle = NULL;

					MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
					mii.fMask = MIIM_STATE;
					mii.fState = MFS_ENABLED;

					SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATE, FALSE, &mii);
					SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATEWAIT, FALSE, &mii);

					mii.fState = MFS_GRAYED;
					SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_SUSPEND, FALSE, &mii);
					SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_RESUME, FALSE, &mii);
					SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DESTROY, FALSE, &mii);
					SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_UPPRIORITY, FALSE, &mii);
					SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DOWNPRIORITY, FALSE, &mii);
				}
			}
		}
		break;
		case IDM_THREAD1_UPPRIORITY:
		{
			SetThreadPriority(g_lpThread[0].ThreadHandle, THREAD_PRIORITY_HIGHEST);
		}
		break;
		case IDM_THREAD1_DOWNPRIORITY:
		{
			SetThreadPriority(g_lpThread[0].ThreadHandle, THREAD_PRIORITY_LOWEST);
		}
		break;
		case IDM_THREAD2_CREATE:
		{
			g_lpThread[1].ThreadHandle = CreateThread(NULL, 0L, ThreadFunc1, &ThrParam2, 0L, &g_lpThread[1].ThreadId);
		}
		break;
		case IDM_THREAD2_DESTROY:
		{
			if (TerminateThread(g_lpThread[1].ThreadHandle, 0))
			{
				if (CloseHandle(g_lpThread[1].ThreadHandle))
				{
					g_lpThread[1].ThreadHandle = NULL;
				}
			}
		}
		break;
		case IDM_THREADINFO_THREAD1:
		{
			DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, ThreadDlgProc, 0);
		}
		break;
		case IDM_PROCESS_NOTEPAD:
		{
			PROCESS_INFORMATION pi;
			STARTUPINFO sti;
			ZeroMemory(&sti, sizeof(STARTUPINFO));
			sti.cb = sizeof(STARTUPINFO);

			if (CreateProcess(NULL, g_lpProcess[1].CmdParam, NULL, NULL, 0, 0, NULL, NULL, &sti, &pi))
			{
				g_lpProcess[1].ProcHandle = &pi.hProcess;
				g_lpProcess[1].ProcId = pi.dwProcessId;
				g_lpProcess[1].ThreadHandle = &pi.hThread;
				g_lpProcess[1].ThreadId = pi.dwThreadId;
			}
			else
			{
				MessageBox(NULL, "������� ������� �� ������", "����������", MB_OK);
			}
		}
		break;
		case IDM_PROCESS_NOTEPADTEXT:
		{
			PROCESS_INFORMATION pi;
			STARTUPINFO sti;
			ZeroMemory(&sti, sizeof(STARTUPINFO));
			sti.cb = sizeof(STARTUPINFO);

			if (CreateProcess(NULL, g_lpProcess[2].CmdParam, NULL, NULL, 0, 0, NULL, NULL, &sti, &pi))
			{
				g_lpProcess[2].ProcHandle = &pi.hProcess;
				g_lpProcess[2].ProcId = pi.dwProcessId;
				g_lpProcess[2].ThreadHandle = &pi.hThread;
				g_lpProcess[2].ThreadId = pi.dwThreadId;
			}
			else
			{
				MessageBox(NULL, "������� ������� �� ������", "����������", MB_OK);
			}
		}
		break;
		case IDM_PROCESS_CALC:
		{
			PROCESS_INFORMATION pi;
			STARTUPINFO sti;
			ZeroMemory(&sti, sizeof(STARTUPINFO));
			sti.cb = sizeof(STARTUPINFO);

			if (CreateProcess(NULL, g_lpProcess[3].CmdParam, NULL, NULL, 0, 0, NULL, NULL, &sti, &pi))
			{
				g_lpProcess[3].ProcHandle = &pi.hProcess;
				g_lpProcess[3].ProcId = pi.dwProcessId;
				g_lpProcess[3].ThreadHandle = &pi.hThread;
				g_lpProcess[3].ThreadId = pi.dwThreadId;
			}
			else
			{
				MessageBox(NULL, "������� ����������� �� ������", "����������", MB_OK);
			}
		}
		break;
		case IDM_PROCESS_CLOSECALC:
		{
			MessageBox(NULL, "Hello", "����������", MB_OK);
			TerminateProcess(g_lpProcess[3].ProcHandle, NO_ERROR);
		}
		break;
		case IDM_PROCESSINFO_CURRENT:
		{
			DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProcessDlgProc,0);
		}
		break;
		case IDM_PROCESSINFO_NOTEPAD:
		{
			DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProcessDlgProc,1);
		}
		break;
		case IDM_PROCESSINFO_NOTEPADTEXT:
		{
			DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProcessDlgProc, 2);
		}
		break;
		case IDM_PROCESSINFO_CALC:
		{
			DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProcessDlgProc, 3);
		}
		break;

		default:
		{
			MessageBox(hWnd, TEXT("������� �� �����������"), buff, MB_OK);
		}
		break;
	}
}

BOOL CALLBACK ProcessDlgProc(HWND hDlg, UINT mes, WPARAM wParam, LPARAM lParam)
{
	TCHAR buff[200];
	wsprintf(buff, TEXT("%d"), lParam);

	switch (mes)
	{
		case WM_INITDIALOG:
		{
			DWORD dwExitCode;

			SetDlgItemText(hDlg, IDC_PROCNAME, g_lpProcess[lParam].ProcImage);

			TCHAR procHandleText[128];
			wsprintf(procHandleText, TEXT("0x%08X"), g_lpProcess[lParam].ProcHandle);
			SetDlgItemText(hDlg, IDC_PROCHANDLE, procHandleText);

			TCHAR procIdText[128];
			wsprintf(procIdText, TEXT("%d"), g_lpProcess[lParam].ProcId);
			SetDlgItemText(hDlg, IDC_PROCID, procIdText);

			TCHAR threadHandleText[128];
			wsprintf(threadHandleText, TEXT("0x%08X"), g_lpProcess[lParam].ThreadHandle);
			SetDlgItemText(hDlg, IDC_THREADHANDLE, threadHandleText);

			TCHAR threadIdText[128];
			wsprintf(threadIdText, TEXT("%d"), g_lpProcess[lParam].ThreadId);
			SetDlgItemText(hDlg, IDC_THREADID, threadIdText);


			if(!GetExitCodeProcess(g_lpProcess[lParam].ProcHandle, &dwExitCode))
			{

			}
			else
			{
				if (dwExitCode == STILL_ACTIVE)
				{
					SetDlgItemText(hDlg, IDC_PROCESSSTATUS, TEXT("�������"));
				}
				else
				{
					TCHAR processStatus[128];
					wsprintf(processStatus, TEXT("%d"), dwExitCode);
					SetDlgItemText(hDlg, IDC_PROCESSSTATUS, processStatus);
				}
			}
		}
		return TRUE;
		case WM_COMMAND:
		{
		}
		return TRUE;
		case WM_CLOSE:
		{
			EndDialog(hDlg, IDCANCEL);
		}
		break;
	}
	return FALSE;
}


BOOL CALLBACK ThreadDlgProc(HWND hDlg, UINT mes, WPARAM wParam, LPARAM lParam)
{
	TCHAR buff[200];
	wsprintf(buff, TEXT("%d"), lParam);
	DWORD dwExitCode;

	switch (mes)
	{
	case WM_INITDIALOG:
	{
		TCHAR buff[200];
		wsprintf(buff, TEXT("%d"), g_lpThread[lParam].ThreadId);

		SetDlgItemText(hDlg, IDC_THREADID, buff);

		int Priority = GetThreadPriority(g_lpThread[lParam].ThreadHandle);

		TCHAR priorityText[128];
		wsprintf(priorityText, TEXT("%d"), Priority+8);
		SetDlgItemText(hDlg, IDC_THREADPRIORITY, priorityText);

		TCHAR threadHandle[128];
		wsprintf(threadHandle, TEXT("0x%.8X"), g_lpThread[lParam].ThreadHandle);
		SetDlgItemText(hDlg, IDC_THREADHANDLE, threadHandle);

		if (!GetExitCodeThread(g_lpThread[lParam].ThreadHandle, &dwExitCode))
		{

		}
		else
		{
			if (dwExitCode == STILL_ACTIVE)
			{
				SetDlgItemText(hDlg, IDC_THREADSTATE, TEXT("�������"));
			}
			else
			{
				TCHAR threadState[128];
				wsprintf(threadState, TEXT("%s"), dwExitCode);
				SetDlgItemText(hDlg, IDC_THREADSTATE, threadState);
			}
		}
	}
	return TRUE;
	case WM_COMMAND:
	{
	}
	return TRUE;
	case WM_CLOSE:
	{
		EndDialog(hDlg, IDCANCEL);
	}
	break;
	}
	return FALSE;
}


// �������, ����������� � ������ ��������� ������(������� ������)
DWORD WINAPI ThreadFunc1(PVOID pvParam)
{
	THREAD_PARAM * ThrParam; // ��������� ���������� ��� �������� ����������� ���������
	ThrParam = (THREAD_PARAM *)pvParam;

#define MAXCHARCOUNT 100

	TCHAR CreepingLine[MAXCHARCOUNT];	// ����� ��� �������� ������� ������
	TCHAR buf[MAXCHARCOUNT] = { 0 };		// ������� ����� ��� ������������ ������ ������
	int   iBeginningIndex;			// ������ ������ ��������� ������������������ ��������
	int   StringLength = 0;			// ����� ������
	RECT  rc;
	HDC   hDC;
	int cRun = 0;	// ������� ��������� ������
	int N = 5;		// ���������� ��������� � �����

	// ������������ ������ ������� ������
	wsprintf(CreepingLine,
			TEXT("60331,�������� �.�.,����� � %d, ������������� - 0x%.8X. ###"),
			ThrParam->Num, g_lpThread[ThrParam->Num].ThreadId);
	// ������ ������
	StringLength = iBeginningIndex = lstrlen(CreepingLine);
	lstrcpy(buf, CreepingLine);

	// ������� ������������� ������� ������
	GetClientRect(ThrParam->hWnd, &rc);
	rc.top = ThrParam->YPos;
	rc.left = ThrParam->XPos;
	rc.right = rc.right - ThrParam->XPos;

	// ��������� ��������� ��� ������ ������
	hDC = GetDC(ThrParam->hWnd);

	// ����������� ���� ������ ����� ������� �� N �����
	while (TRUE)
	{
		/* // �������� ���������� �������������� ������
		// ����� ����� ����� ��� ����� �������
		WaitForSingleObject(g_hMutex, INFINITE);
		*/

		cRun = 0;
		while (cRun < N - 1)// ���� ������ ����� �� N �����
		{
			// ���� ������������ ����������� ������ �� ����������
				// ������� �� �������(����������� ����� ������� � ������� ������)
			for (int j = 0; j < StringLength; j++)
			{
				if (iBeginningIndex == 0)
				{
					iBeginningIndex = StringLength;
					cRun++; // ������� ���������� ������ �������� ������
				}

				// C���� �������� � ������� ������ �� ���� �������
				TCHAR c;
				c = buf[StringLength];
				for (int i = StringLength; i > 0; i--)// ���� ������
					buf[i] = buf[i - 1];
				buf[0] = c;

				// ������ ������
				DrawText(hDC, buf, -1, &rc, DT_LEFT | DT_SINGLELINE);

				iBeginningIndex--;

				Sleep(200); // ������������ ������ �� 200 ���� - ���������� �����

				//  ��������������" ������ - ���������� "��������" ����� 
				// ��� ������ �� ������� ������ ����� ���������� for_delay(mywait).
				//  ��������� ��������� ������ �� ��������� ����������� �������.
				//  for_delay(mywait) - ����� ������, ���������� ���������
				//  �� ��������� �����

			} // ����� ����� ������� ������������ ��������� ������

		} // ����� ����� ������ ����� �����

			/* // ����� ������������ ������� ���� � ����� ����� �����
			ReleaseMutex(g_hMutex);
			*/
	}
	return 0;
}

// �������, ����������� � ������ ��������� ������(������� ������)
DWORD WINAPI ThreadFuncWait(PVOID pvParam)
{
	THREAD_PARAM * ThrParam; // ��������� ���������� ��� �������� ����������� ���������
	ThrParam = (THREAD_PARAM *)pvParam;

#define MAXCHARCOUNT 100

	TCHAR CreepingLine[MAXCHARCOUNT];	// ����� ��� �������� ������� ������
	TCHAR buf[MAXCHARCOUNT] = { 0 };		// ������� ����� ��� ������������ ������ ������
	int   iBeginningIndex;			// ������ ������ ��������� ������������������ ��������
	int   StringLength = 0;			// ����� ������
	RECT  rc;
	HDC   hDC;
	int cRun = 0;	// ������� ��������� ������
	int N = 5;		// ���������� ��������� � �����

					// ������������ ������ ������� ������
	wsprintf(CreepingLine,
		TEXT("60331,�������� �.�.,����� � %d, ������������� - 0x%.8X. ###"),
		ThrParam->Num, g_lpThread[ThrParam->Num].ThreadId);
	// ������ ������
	StringLength = iBeginningIndex = lstrlen(CreepingLine);
	lstrcpy(buf, CreepingLine);

	// ������� ������������� ������� ������
	GetClientRect(ThrParam->hWnd, &rc);
	rc.top = ThrParam->YPos;
	rc.left = ThrParam->XPos;
	rc.right = rc.right - ThrParam->XPos;

	// ��������� ��������� ��� ������ ������
	hDC = GetDC(ThrParam->hWnd);

	// ����������� ���� ������ ����� ������� �� N �����
	while (TRUE)
	{
		DWORD dw;
		dw = WaitForSingleObject(g_lpThread[1].ThreadHandle, 500);

		switch(dw)
		{
			case WAIT_OBJECT_0:
			{
				cRun = 0;
				while (cRun < N - 1)// ���� ������ ����� �� N �����
				{
					// ���� ������������ ����������� ������ �� ����������
					// ������� �� �������(����������� ����� ������� � ������� ������)
					for (int j = 0; j < StringLength; j++)
					{
						if (iBeginningIndex == 0)
						{
							iBeginningIndex = StringLength;
							cRun++; // ������� ���������� ������ �������� ������
						}

						// C���� �������� � ������� ������ �� ���� �������
						TCHAR c;
						c = buf[StringLength];
						for (int i = StringLength; i > 0; i--)// ���� ������
							buf[i] = buf[i - 1];
						buf[0] = c;

						// ������ ������
						DrawText(hDC, buf, -1, &rc, DT_LEFT | DT_SINGLELINE);

						iBeginningIndex--;

						Sleep(200); // ������������ ������ �� 200 ���� - ���������� �����

									//  ��������������" ������ - ���������� "��������" ����� 
									// ��� ������ �� ������� ������ ����� ���������� for_delay(mywait).
									//  ��������� ��������� ������ �� ��������� ����������� �������.
									//  for_delay(mywait) - ����� ������, ���������� ���������
									//  �� ��������� �����

					} // ����� ����� ������� ������������ ��������� ������

				} // ����� ����� ������ ����� �����				
			}
			break;
			case WAIT_TIMEOUT:
			{    
				// ������� �� ���������� �� 5000 ��   ...   
			}
			break;
			case WAIT_FAILED:
			{
				// ������ ���������� �������   ...
			}
			break;
		} 
		  /* // ����� ������������ ������� ���� � ����� ����� �����
		  ReleaseMutex(g_hMutex);
		  */
	}
	return 0;
}



