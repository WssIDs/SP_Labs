#include "sp_pr2-3.h"


void for_delay(int param)
{
	double x, y, z; x = 2.0; y = 3.0;
	for (int i = 0; i<param; i++) { z = x*y - 1; z = z + 1; }
}


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
	ThrParam1.hWnd = hWnd;
	ThrParam2.hWnd = hWnd;


	g_lpszThread1Menu = GetSubMenu(g_lpszMainMenu, 1);
	g_lpszThread2Menu = GetSubMenu(g_lpszMainMenu, 2);

	g_lpThread[0].ThreadHandle = GetCurrentThread();//дескпритор текущего потока
	g_lpThread[0].ThreadId = GetCurrentThreadId();//id текущего потока

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
	for (int i = 1; i < ARRAYSIZE(g_lpThread); i++)
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
	PostQuitMessage(0); // Посылка WM_QUIT приложению
}


// WM_COMMAND
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	TCHAR buff[200];
	wsprintf(buff, TEXT("%d"), id);
	DWORD exitCode = 0;
	HANDLE hThread1;
	HANDLE hThread2;

	switch (id)
	{
		case IDM_FILE_EXIT:
		{
			DestroyWindow(hWnd);
		}
		break;
		case IDM_THREAD1_CREATE: // создать поток
		{
			hThread1 = CreateThread(NULL, 0L, ThreadFunc1, &ThrParam1, 0L, &g_lpThread[1].ThreadId);

			if (hThread1 == NULL)//проверка на создание
			{
				MessageBox(hWnd, TEXT("Failed to create thread.\r\n"), TEXT("Ошибка"), MB_OK);
				break;
			}

			g_lpThread[1].ThreadHandle = hThread1;
			g_uThCount++;
			g_lpThread[1].SecThreadState = 0;//активный поток

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_GRAYED;

			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATE, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATEWAIT, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_RESUME, FALSE, &mii);

			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_SUSPEND, FALSE, &mii);

			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DESTROY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_UPPRIORITY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DOWNPRIORITY, FALSE, &mii);

		}
		break;
		case IDM_THREAD1_CREATEWAIT: // создать ждущий поток
		{
			hThread1 = CreateThread(NULL, 0L, ThreadFunc1, &ThrParam1, CREATE_SUSPENDED, &g_lpThread[1].ThreadId);

			if (hThread1 == NULL)//проверка на создание
			{
				MessageBox(hWnd, TEXT("Failed to create thread.\r\n"), TEXT("Ошибка"), MB_OK);
				break;
			}

			g_lpThread[1].ThreadHandle = hThread1;
			g_uThCount++;
			g_lpThread[1].SecThreadState = 1;//спящий поток

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_GRAYED;

			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATE, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_CREATEWAIT, FALSE, &mii);			
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_SUSPEND, FALSE, &mii);

			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_RESUME, FALSE, &mii);


			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DESTROY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_UPPRIORITY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_DOWNPRIORITY, FALSE, &mii);
		}
		break;
		case IDM_THREAD1_SUSPEND: // приостановить
		{
			SuspendThread(g_lpThread[1].ThreadHandle);
			g_lpThread[1].SecThreadState = 1;//спящий поток

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_RESUME, FALSE, &mii);
			
			mii.fState = MFS_GRAYED;
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_SUSPEND, FALSE, &mii);

		}
		break;
		case IDM_THREAD1_RESUME: // продолжить
		{
			ResumeThread(g_lpThread[1].ThreadHandle);
			g_lpThread[1].SecThreadState = 0;//спящий поток

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_SUSPEND, FALSE, &mii);

			mii.fState = MFS_GRAYED;
			SetMenuItemInfo(g_lpszThread1Menu, IDM_THREAD1_RESUME, FALSE, &mii);
	
		}
		break;
		case IDM_THREAD1_DESTROY: // уничтожить
		{
			if (TerminateThread(g_lpThread[1].ThreadHandle, 1))
			{
					g_uThCount--;
					//if (CloseHandle(g_lpThread[0].ThreadHandle))
					//{
					//	g_lpThread[0].ThreadHandle = NULL;

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
				//}
			}
		}
		break;
		case IDM_THREAD1_UPPRIORITY:
		{
			int priority = GetThreadPriority(g_lpThread[1].ThreadHandle);

			if (priority < THREAD_PRIORITY_HIGHEST)
			{
				SetThreadPriority(g_lpThread[1].ThreadHandle, ++priority);
			}
			// Используем метод блокировки команд меню
			if (priority == THREAD_PRIORITY_HIGHEST)
			{
				EnableMenuItem(g_lpszThread1Menu, IDM_THREAD1_UPPRIORITY, MF_GRAYED);
				EnableMenuItem(g_lpszThread1Menu, IDM_THREAD1_DOWNPRIORITY, MF_ENABLED);
			}
		}
		break;
		case IDM_THREAD1_DOWNPRIORITY:
		{
			int priority = GetThreadPriority(g_lpThread[1].ThreadHandle);
			if (priority > THREAD_PRIORITY_LOWEST)
			{
				SetThreadPriority(g_lpThread[1].ThreadHandle, --priority);
			}
			// Используем метод блокировки команд меню
			if (priority == THREAD_PRIORITY_LOWEST)
			{
				EnableMenuItem(g_lpszThread1Menu, IDM_THREAD1_DOWNPRIORITY, MF_GRAYED);
				EnableMenuItem(g_lpszThread1Menu, IDM_THREAD1_UPPRIORITY, MF_ENABLED);
			}
		}
		break;
		case IDM_THREAD2_CREATE: // создать поток
		{
			hThread2 = CreateThread(NULL, 0L, ThreadFunc1, &ThrParam2, 0L, &g_lpThread[2].ThreadId);

			if (hThread2 == NULL)//проверка на создание
			{
				MessageBox(hWnd, TEXT("Failed to create thread.\r\n"), TEXT("Ошибка"), MB_OK);
				break;
			}

			g_lpThread[2].ThreadHandle = hThread2;
			g_uThCount++;
			g_lpThread[2].SecThreadState = 0;//активный поток

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_GRAYED;

			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_CREATE, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_CREATEWAIT, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_RESUME, FALSE, &mii);

			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_SUSPEND, FALSE, &mii);

			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_DESTROY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_UPPRIORITY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_DOWNPRIORITY, FALSE, &mii);

		}
		break;
		case IDM_THREAD2_CREATEWAIT: // создать ждущий поток
		{
			hThread2 = CreateThread(NULL, 0L, ThreadFunc1, &ThrParam2, CREATE_SUSPENDED, &g_lpThread[2].ThreadId);

			if (hThread2 == NULL)//проверка на создание
			{
				MessageBox(hWnd, TEXT("Failed to create thread.\r\n"), TEXT("Ошибка"), MB_OK);
				break;
			}

			g_lpThread[2].ThreadHandle = hThread2;
			g_uThCount++;
			g_lpThread[2].SecThreadState = 1;//спящий поток

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_GRAYED;

			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_CREATE, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_CREATEWAIT, FALSE, &mii);

			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_SUSPEND, FALSE, &mii);
			mii.fState = MFS_ENABLED;
	
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_RESUME, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_DESTROY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_UPPRIORITY, FALSE, &mii);
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_DOWNPRIORITY, FALSE, &mii);
		}
		break;
		case IDM_THREAD2_SUSPEND: // приостановить
		{
			SuspendThread(g_lpThread[2].ThreadHandle);
			g_lpThread[2].SecThreadState = 1;//спящий поток

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_RESUME, FALSE, &mii);

			mii.fState = MFS_GRAYED;
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_SUSPEND, FALSE, &mii);

		}
		break;
		case IDM_THREAD2_RESUME: // продолжить
		{
			ResumeThread(g_lpThread[2].ThreadHandle);
			g_lpThread[2].SecThreadState = 0;//спящий поток

			MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
			mii.fMask = MIIM_STATE;
			mii.fState = MFS_ENABLED;
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_SUSPEND, FALSE, &mii);

			mii.fState = MFS_GRAYED;
			SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_RESUME, FALSE, &mii);

		}
		break;
		case IDM_THREAD2_DESTROY: // уничтожить
		{
			if (TerminateThread(g_lpThread[2].ThreadHandle, 1))
			{
				g_uThCount--;
				//if (CloseHandle(g_lpThread[0].ThreadHandle))
				//{
				//	g_lpThread[0].ThreadHandle = NULL;

				MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
				mii.fMask = MIIM_STATE;
				mii.fState = MFS_ENABLED;

				SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_CREATE, FALSE, &mii);
				SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_CREATEWAIT, FALSE, &mii);

				mii.fState = MFS_GRAYED;
				SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_SUSPEND, FALSE, &mii);
				SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_RESUME, FALSE, &mii);
				SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_DESTROY, FALSE, &mii);
				SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_UPPRIORITY, FALSE, &mii);
				SetMenuItemInfo(g_lpszThread2Menu, IDM_THREAD2_DOWNPRIORITY, FALSE, &mii);
				//}
			}
		}
		break;
		case IDM_THREAD2_UPPRIORITY:
		{
			int priority = GetThreadPriority(g_lpThread[2].ThreadHandle);

			if (priority < THREAD_PRIORITY_HIGHEST)
			{
				SetThreadPriority(g_lpThread[2].ThreadHandle, ++priority);
			}
			// Используем метод блокировки команд меню
			if (priority == THREAD_PRIORITY_HIGHEST)
			{
				EnableMenuItem(g_lpszThread2Menu, IDM_THREAD2_UPPRIORITY, MF_GRAYED);
				EnableMenuItem(g_lpszThread2Menu, IDM_THREAD2_DOWNPRIORITY, MF_ENABLED);
			}
		}
		break;
		case IDM_THREAD2_DOWNPRIORITY:
		{
			int priority = GetThreadPriority(g_lpThread[2].ThreadHandle);
			if (priority > THREAD_PRIORITY_LOWEST)
			{
				SetThreadPriority(g_lpThread[2].ThreadHandle, --priority);
			}
			// Используем метод блокировки команд меню
			if (priority == THREAD_PRIORITY_LOWEST)
			{
				EnableMenuItem(g_lpszThread2Menu, IDM_THREAD2_DOWNPRIORITY, MF_GRAYED);
				EnableMenuItem(g_lpszThread2Menu, IDM_THREAD2_UPPRIORITY, MF_ENABLED);
			}
		}
		break;
		case IDM_THREADINFO_THREAD1:
		{
			DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, ThreadDlgProc, 1);
		}
		break;
		case IDM_THREADINFO_THREAD2:
		{
			DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, ThreadDlgProc, 2);
		}
		break;
		case IDM_SYNC_ENABLEAGREE:
		{
			InitializeCriticalSection(&g_cs);
		}
		break;
		case IDM_SYNC_DISABLEAGREE:
		{
			DeleteCriticalSection(&g_cs);
		}
		break;
		default:
		{
			MessageBox(hWnd, TEXT("Команда не реализована"), buff, MB_OK);
		}
		break;
	}
}

BOOL CALLBACK ThreadDlgProc(HWND hDlg, UINT mes, WPARAM wParam, LPARAM lParam)
{
	switch (mes)
	{
	case WM_INITDIALOG:
	{
		TCHAR buff[200];
		wsprintf(buff, TEXT("%d"), g_lpThread[lParam].ThreadId);

		SetDlgItemText(hDlg, IDC_THREADID, buff);

		TCHAR threadHandle[128];
		wsprintf(threadHandle, TEXT("0x%.8X"), g_lpThread[lParam].ThreadHandle);
		SetDlgItemText(hDlg, IDC_THREADHANDLE, threadHandle);

		DWORD dwExitCodeThread;
		TCHAR ExitCodeThreadText[64];

		if (!GetExitCodeThread(g_lpThread[lParam].ThreadHandle, &dwExitCodeThread))
		{

		}
		else
		{
			if (dwExitCodeThread == STILL_ACTIVE)
			{
				if (g_lpThread[lParam].SecThreadState == 0) 	wsprintf(ExitCodeThreadText, TEXT("Активен"));
				if (g_lpThread[lParam].SecThreadState == 1) 	wsprintf(ExitCodeThreadText, TEXT("Ожидание"));
			}
			else wsprintf(ExitCodeThreadText, TEXT("%d"), dwExitCodeThread);

			SetDlgItemText(hDlg, IDC_THREADSTATE, ExitCodeThreadText);
		}

		if (dwExitCodeThread == STILL_ACTIVE) {
			TCHAR priorityText[128];
			int priority = GetThreadPriority(g_lpThread[lParam].ThreadHandle);
			wsprintf(priorityText, TEXT("%d"), GetThreadPriority(g_lpThread[lParam].ThreadHandle));
			SetDlgItemText(hDlg, IDC_THREADPRIORITY, priorityText);
		}
		else SetDlgItemText(hDlg, IDC_THREADPRIORITY, TEXT("-"));


		ULARGE_INTEGER uiCreationTime, uiExitTime, uiLifeTime, uiSystemTime;
		FILETIME CreationTime, ExitTime, KernelTime, UserTime, SystemTime, LifeTime;

		//Извлекает текущую системную дату и время. Информация находится в формате согласованного универсального времени (UTC).
		GetSystemTimeAsFileTime(&SystemTime);
		uiSystemTime.LowPart = SystemTime.dwLowDateTime;
		uiSystemTime.HighPart = SystemTime.dwHighDateTime;

		//Извлекает информацию о времени для указанного процесса.
		GetThreadTimes(g_lpThread[lParam].ThreadHandle, &CreationTime, &ExitTime, &KernelTime, &UserTime);

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

		SYSTEMTIME stLifeTime;

		FileTimeToSystemTime(&LifeTime, &stLifeTime);
		TCHAR BuffLifeTime[50];
		wsprintf(BuffLifeTime, TEXT("%02d:%02d:%02d:%03d"), stLifeTime.wHour, stLifeTime.wMinute, stLifeTime.wSecond,
			stLifeTime.wMilliseconds);
		SetDlgItemText(hDlg, IDC_THREADTIME, BuffLifeTime);

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


// Функция, выполняемая в каждом отдельном потоке(функция потока)
DWORD WINAPI ThreadFunc1(PVOID pvParam)
{
	THREAD_PARAM * ThrParam; // Локальная переменная для хранения переданного параметра
	ThrParam = (THREAD_PARAM *)pvParam;

	TCHAR CreepingLine[100]; // Буфер для символов бегущей строки
	TCHAR buf[100] = { 0 }; // Рабочий буфер для циклического сдвига строки

	int   iBeginningIndex; // Индекс начала выводимой последовательности символов

	int   StringLength = 0; // Длина строки

	RECT  rc;
	HDC   hDC;
	int cRun = 0; // Счетчик “пробегов” строки
	int N = 5; //  Количество “пробегов” в серии

			   // Формирование текста бегущей строки
	wsprintf(CreepingLine,
		TEXT("Поток создал Володько В.И."));
	//  Длинна строки
	StringLength = iBeginningIndex = lstrlen(CreepingLine);

	lstrcpy(buf, CreepingLine);

	//  Задание прямоугольной области вывода
	GetClientRect(ThrParam->hWnd, &rc);
	rc.top = ThrParam->YPos;
	rc.left = ThrParam->XPos;
	rc.right = rc.right - ThrParam->XPos;

	//  Получение контекста для вывода строки		
	hDC = GetDC(ThrParam->hWnd);

	//  Бесконечный цикл вывода строк сериями по N строк 
	while (TRUE)
	{
		/* // Взаимное исключение одновременного вывода
		// серии строк более чем одним потоком
		WaitForSingleObject(g_hMutex, INFINITE);
		*/			
		WaitForSingleObject(g_lpThread[1].ThreadHandle, INFINITE);//Чтобы первый поток пошёл занимать секцию только когда она инициализируется
		EnterCriticalSection(&g_cs);
		
		cRun = 0;
		while (cRun < N - 1)// Цикл вывода серии из N строк
		{
			// Цикл однократного продвижения строки от последнего 
			// символа до первого (перемещение слева направо в области вывода)

			for (int j = 0; j < StringLength; j++)
			{
				if (iBeginningIndex == 0)
				{
					iBeginningIndex = StringLength;
					cRun++; // Подсчет количества полных пробегов строки
				}

				// Cдвиг символов в рабочем буфере на одну позицию
				TCHAR c;
				c = buf[StringLength];
				for (int i = StringLength; i > 0; i--)// Цикл сдвига
					buf[i] = buf[i - 1];
				buf[0] = c;

				// Ввывод строки
				DrawText(hDC, buf, -1, &rc, DT_LEFT | DT_SINGLELINE);

				iBeginningIndex--;

				Sleep(200); // приостановка потока на 200 мсек - замедление цикла



			} // Конец цикла полного однократного “пробега” строки


		}// Конец цикла вывода серии строк 
		 /* // конец критического участка кода – вывод серии строк
		 ReleaseMutex(g_hMutex);		
		 */
		LeaveCriticalSection(&g_cs);
	}
	return 0;
}