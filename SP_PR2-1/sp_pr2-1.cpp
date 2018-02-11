#include "sp_pr2-1.h"

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_COMMAND, km_OnCommand);				// Обработка меню
		HANDLE_MSG(hWnd, WM_MENUSELECT, km_OnMenuSelect);		// При смене элемента меню
		HANDLE_MSG(hWnd, WM_CLOSE, km_OnClose);					// Перед разрушением
		HANDLE_MSG(hWnd, WM_CREATE, km_OnCreate);				// Вывод при обновлении окна
		HANDLE_MSG(hWnd, WM_PAINT, km_OnPaint);					// Вывод при обновлении окна
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
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/* ---------- Обработчики сообщений -------------------------------- */

// WM_CREATE
BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct)
{
	// Текущий процесс
	g_lpProcess[0].ProcImage = TEXT("");
	wsprintf(g_lpProcess[0].CmdParam, TEXT("%s"), g_lpProcess[0].ProcImage);

	g_lpProcess[0].ProcId = GetCurrentProcessId();
	g_lpProcess[0].ProcHandle = GetCurrentProcess();
	g_lpProcess[0].ThreadHandle = GetCurrentThread();
	g_lpProcess[0].ThreadId = GetCurrentThreadId();

	// Блокнот
	g_lpProcess[1].ProcImage = TEXT("Notepad");
	wsprintf(g_lpProcess[1].CmdParam, TEXT("%s"), g_lpProcess[1].ProcImage);

	g_lpProcess[2].ProcImage = TEXT("Notepad");
	wsprintf(g_lpProcess[2].CmdParam, TEXT("%s %s"), g_lpProcess[2].ProcImage, TEXT("sp_pr2-1.cpp"));

	// Калькулятор
	g_lpProcess[3].ProcImage = TEXT("C:\\Program Files\\Windows NT\\Accessories\\wordpad.exe");
	wsprintf(g_lpProcess[3].CmdParam, TEXT("%s"), g_lpProcess[3].ProcImage);

	return TRUE;
}

// WM_ONPAINT
void km_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна

	EndPaint(hWnd, &ps); // Завершение обновления окна
}

// WM_ONCLOSE
void km_OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

// WM_MENUSELECT
void km_OnMenuSelect(HWND hWnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
}

// WM_DESTROY
void km_OnDestroy(HWND hWnd)
{
	PostQuitMessage(0); // Посылка WM_QUIT приложению
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
		case IDM_PROCESS_NOTEPAD:
		{
			PROCESS_INFORMATION pi;
			STARTUPINFO sti;
			ZeroMemory(&sti, sizeof(STARTUPINFO));
			sti.cb = sizeof(STARTUPINFO);

			if (CreateProcess(NULL, g_lpProcess[1].CmdParam, NULL, NULL, 0, 0, NULL, NULL, &sti, &pi))
			{
				g_lpProcess[1].ProcHandle = pi.hProcess;
				g_lpProcess[1].ProcId = pi.dwProcessId;
				g_lpProcess[1].ThreadHandle = pi.hThread;
				g_lpProcess[1].ThreadId = pi.dwThreadId;
			}
			else
			{
				MessageBox(NULL, "Процесс блокнот не создан", "Информация", MB_OK);
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
				g_lpProcess[2].ProcHandle = pi.hProcess;
				g_lpProcess[2].ProcId = pi.dwProcessId;
				g_lpProcess[2].ThreadHandle = pi.hThread;
				g_lpProcess[2].ThreadId = pi.dwThreadId;
			}
			else
			{
				MessageBox(NULL, "Процесс блокнот не создан", "Информация", MB_OK);
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
				g_lpProcess[3].ProcHandle = pi.hProcess;
				g_lpProcess[3].ProcId = pi.dwProcessId;
				g_lpProcess[3].ThreadHandle = pi.hThread;
				g_lpProcess[3].ThreadId = pi.dwThreadId;
			}
			else
			{
				MessageBox(NULL, "Процесс калькулятор не создан", "Информация", MB_OK);
			}
		}
		break;
		case IDM_PROCESS_CLOSECALC:
		{
			TerminateProcess(g_lpProcess[3].ProcHandle, 1);
			//CloseHandle(g_lpProcess[3].ProcHandle);
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
			MessageBox(hWnd, TEXT("Команда не реализована"), buff, MB_OK);
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
			TCHAR procNameText[128];
			GetModuleFileNameEx(g_lpProcess[lParam].ProcHandle,NULL, procNameText, 128);
			SetDlgItemText(hDlg, IDC_PROCNAME, procNameText);

			TCHAR procHandleText[128];
			wsprintf(procHandleText, TEXT("0x%.8X"), g_lpProcess[lParam].ProcHandle);
			SetDlgItemText(hDlg, IDC_PROCHANDLE, procHandleText);

			TCHAR procIdText[128];
			wsprintf(procIdText, TEXT("%d"), g_lpProcess[lParam].ProcId);
			SetDlgItemText(hDlg, IDC_PROCID, procIdText);

			TCHAR threadHandleText[128];
			wsprintf(threadHandleText, TEXT("0x%.8X"), g_lpProcess[lParam].ThreadHandle);
			SetDlgItemText(hDlg, IDC_THREADHANDLE, threadHandleText);

			TCHAR threadIdText[128];
			wsprintf(threadIdText, TEXT("%d"), g_lpProcess[lParam].ThreadId);
			SetDlgItemText(hDlg, IDC_THREADID, threadIdText);


			DWORD dwExitCodeProcess;
			TCHAR ExitCodeProcessText[64];
			
			if(!GetExitCodeProcess(g_lpProcess[lParam].ProcHandle, &dwExitCodeProcess))
			{

			}
			else
			{
				if (dwExitCodeProcess == STILL_ACTIVE) wsprintf(ExitCodeProcessText, TEXT("Активен"));
				else wsprintf(ExitCodeProcessText, TEXT("%d"), dwExitCodeProcess);
				
				SetDlgItemText(hDlg, IDC_PROCESSSTATUS, ExitCodeProcessText);
			}


			DWORD dwExitCodeThread;
			TCHAR ExitCodeThreadText[64];
			
			if (!GetExitCodeThread(g_lpProcess[lParam].ThreadHandle, &dwExitCodeThread))
			{

			}
			else
			{
				if (dwExitCodeThread == STILL_ACTIVE) wsprintf(ExitCodeThreadText, TEXT("Активен"));
				else wsprintf(ExitCodeThreadText, TEXT("%d"), dwExitCodeThread);

				SetDlgItemText(hDlg, IDC_THREADSTATUS, ExitCodeThreadText);
			}

			
			/*приоритет потока*/
			DWORD dwPriorityClass = GetPriorityClass(g_lpProcess[lParam].ProcHandle);
			TCHAR BuffPriorityClass[50];
			wsprintf(BuffPriorityClass, TEXT("%d"), dwPriorityClass);
			SetDlgItemText(hDlg, IDC_PROCESSCLASSPRIORITY, BuffPriorityClass);

			/*а) время жизни процесса;
			б) время выполнения в режиме пользователя;
			в) время выполнения в режиме ядра;
			г) время простоя.
			( использовать системные вызовы GetTickCount и GetProcessTimes );*/

			ULARGE_INTEGER uiCreationTime, uiExitTime, uiLifeTime, uiSystemTime, uiKernelTime, uiUserTime, uiIdleTime;
			FILETIME CreationTime, ExitTime, KernelTime, UserTime, SystemTime, LifeTime, IdleTime;

			//Извлекает текущую системную дату и время. Информация находится в формате согласованного универсального времени (UTC).
			GetSystemTimeAsFileTime(&SystemTime);
			uiSystemTime.LowPart = SystemTime.dwLowDateTime;
			uiSystemTime.HighPart = SystemTime.dwHighDateTime;

			//Извлекает информацию о времени для указанного процесса.
			GetProcessTimes(g_lpProcess[lParam].ProcHandle, &CreationTime, &ExitTime, &KernelTime, &UserTime);

			uiCreationTime.LowPart = CreationTime.dwLowDateTime;
			uiCreationTime.HighPart = CreationTime.dwHighDateTime;
			uiExitTime.LowPart = ExitTime.dwLowDateTime;
			uiExitTime.HighPart = ExitTime.dwHighDateTime;

			//QuadPart	An unsigned 64 - bit integer.
			if (uiExitTime.QuadPart != 0)
				uiLifeTime.QuadPart = uiExitTime.QuadPart - uiCreationTime.QuadPart;
			else
				uiLifeTime.QuadPart = uiSystemTime.QuadPart - uiCreationTime.QuadPart;

			LifeTime.dwLowDateTime = uiLifeTime.LowPart;
			LifeTime.dwHighDateTime = uiLifeTime.HighPart;

			SYSTEMTIME stLifeTime, stKernelTime, stUserTime, stIdleTime;

			FileTimeToSystemTime(&LifeTime, &stLifeTime);
			TCHAR BuffLifeTime[50];
			wsprintf(BuffLifeTime, TEXT("%02d:%02d:%02d:%03d"), stLifeTime.wHour, stLifeTime.wMinute, stLifeTime.wSecond,
				stLifeTime.wMilliseconds);
			SetDlgItemText(hDlg, IDC_PROCESSTIME, BuffLifeTime);

			FileTimeToSystemTime(&KernelTime, &stKernelTime);
			TCHAR BuffKernelTime[50];
			wsprintf(BuffKernelTime, TEXT("%02d:%02d:%02d:%03d"), stKernelTime.wHour, stKernelTime.wMinute, stKernelTime.wSecond,
				stKernelTime.wMilliseconds);
			SetDlgItemText(hDlg, IDC_PROCESSKERNELTIME, BuffKernelTime);

			FileTimeToSystemTime(&UserTime, &stUserTime);
			TCHAR BuffUserTime[50];
			wsprintf(BuffUserTime, TEXT("%02d:%02d:%02d:%03d"), stUserTime.wHour, stUserTime.wMinute, stUserTime.wSecond,
				stUserTime.wMilliseconds);
			SetDlgItemText(hDlg, IDC_PROCESSUSERTIME, BuffUserTime);

			uiKernelTime.LowPart = KernelTime.dwLowDateTime;
			uiKernelTime.HighPart = KernelTime.dwHighDateTime;
			uiUserTime.LowPart = UserTime.dwLowDateTime;
			uiUserTime.HighPart = UserTime.dwHighDateTime;

			uiIdleTime.QuadPart = uiLifeTime.QuadPart - uiKernelTime.QuadPart - uiUserTime.QuadPart;

			IdleTime.dwLowDateTime = uiIdleTime.LowPart;
			IdleTime.dwHighDateTime = uiIdleTime.HighPart;

			FileTimeToSystemTime(&IdleTime, &stIdleTime);
			TCHAR BuffIdleTime[50];
			wsprintf(BuffIdleTime, TEXT("%02d:%02d:%02d:%03d"), stIdleTime.wHour, stIdleTime.wMinute, stIdleTime.wSecond,
				stIdleTime.wMilliseconds);
			SetDlgItemText(hDlg, IDC_PROCESSIDLETIME, BuffIdleTime);

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

