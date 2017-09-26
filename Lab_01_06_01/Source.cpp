// Обработка ввода мыши и клавиатуры 
// Справочная информация
// Platform SDK/User Interface Services/User Input/Keyboard Input/
// Keyboard Input Reference/Keyboard Input Messages

#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include  <stdio.h>
#include <tchar.h>

// Глобальные переменные
int     nPosX = 10;
int     nPosY = 30;
LPCSTR  lpszAppName = "Демонстрация обработки ввода мыши и клавиатуры";
LPCSTR lpszClassName = "Key And Mause Demo Class";
HWND    hMainWnd;
HDC g_HDC;
BOOL g_fDraw;


//========Предварительное объявление функций=========================
/* Процедура главного окна */
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
/* Регистрация класса главного окна */
BOOL Register(HINSTANCE);
/* Создание главного окна */
HWND Create(HINSTANCE, int);

/* Обработчики соощений */
/*--- WM_DESTROY -----------------------------------------------------*/
void  km_OnDestroy(HWND hwnd);
/*--- WM_CHAR --------------------------------------------------------*/
void  km_OnChar(HWND hwnd, UINT ch, int cRepeat);
/*--- WM_KEYUP,WM_KEYDOWN --------------------------------------------*/
void  km_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
/*--- WM_SYSKEYDOWN, WM_SYSKEYUP  ------------------------------------*/
void  km_OnSysKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
/*--- WM_LBUTTONDOWN, WM_LBUTTONDBLCLK -------------------------------*/
void  km_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
/*--- WM_LBUTTONUP ---------------------------------------------------*/
void  km_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
/*--- WM_MOUSEMOVE ---------------------------------------------------*/
void  km_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
/*--- WM_PAINT -------------------------------------------------------*/
void  km_OnPaint(HWND hwnd);
/*--------------------------------------------------------------------*/
//====================================================================//


// Точка входа в программу ===========================================//
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	MSG Msg;
	BOOL ok;

	ok = Register(hInst);
	if (!ok) return FALSE;

	hMainWnd = Create(hInst, nCmdShow);
	if (!hMainWnd) return FALSE;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

//== Функция для регистрации класса окна =============================//
BOOL Register(HINSTANCE hinst)
{
	WNDCLASSEX WndClass;
	BOOL fRes;

	memset(&WndClass, 0, sizeof(WNDCLASSEX));
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.lpszClassName = lpszClassName;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hinst;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.lpszMenuName = NULL;

	fRes = (BOOL)RegisterClassEx(&WndClass);
	return fRes;
}

//== Функция создания окна ===========================================//
HWND Create(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd = CreateWindowEx(0,
		lpszClassName, lpszAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) return hwnd;

	g_HDC = GetDC(hwnd);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return hwnd;
}

//== Оконная процедура главного окна =================================//
LRESULT WINAPI
WndProc(HWND hwnd, UINT MesId, WPARAM wParam, LPARAM lParam)
{
	static BOOL fDraw;

	switch (MesId)
	{
		HANDLE_MSG(hwnd, WM_DESTROY, km_OnDestroy);
		HANDLE_MSG(hwnd, WM_CHAR, km_OnChar);
		HANDLE_MSG(hwnd, WM_KEYDOWN, km_OnKey);
		HANDLE_MSG(hwnd, WM_KEYUP, km_OnKey);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, km_OnMouseMove);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, km_OnLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, km_OnLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, km_OnLButtonUp);
		HANDLE_MSG(hwnd, WM_PAINT, km_OnPaint);
		HANDLE_MSG(hwnd, WM_SYSKEYUP, km_OnSysKey);
		HANDLE_MSG(hwnd, WM_SYSKEYDOWN, km_OnSysKey);
	default:
		return DefWindowProc(hwnd, MesId, wParam, lParam);
	}
}

//====================================================================//
//====================================================================//

//=== Обработчик сообщения WM_DESTROY ================================//
void km_OnDestroy(HWND hwnd)
{
	ReleaseDC(hwnd, g_HDC);
	PostQuitMessage(0);
}

//=== Обработчик сообщения WM_CHAR ===================================//
void km_OnChar(HWND hwnd, UINT ch, int cRepeat)
{
	char S[100];//Буфер для формирования выводимой строки(100 байт)
	HDC DC = GetDC(hwnd);//Получаем контекст устройства графического вывода

	wsprintf(S, "WM_CHAR ==> Ch = %c   cRepeat = %d    ", ch, cRepeat);
	SetBkColor(DC, GetSysColor(COLOR_WINDOW));//Задаем цвет фона
	TextOut(DC, nPosX, nPosY + 20, S, strlen(S));//Выводим строку

	ReleaseDC(hwnd, DC);//Освобождаем контекст
}

//=== Обработчик сообщения WM_KEYUP,WM_KEYDOWN =======================//
void km_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	char S[100];//Буфер для формирования выводимой строки(100 байт)
	HDC DC = GetDC(hwnd);//Получаем контекст устройства графического вывода

	if (fDown) { //Клавиша нажата
		wsprintf(S, "WM_KEYDOWN ==> vk = %d fDown = %d cRepeat = %d flags = %d    ",
			vk, fDown, cRepeat, flags);
	}
	else {//Клавиша отпущена
		wsprintf(S, "WM_KEYUP ==> vk = %d fDown = %d cRepeat = %d flags = %d      ",
			vk, fDown, cRepeat, flags);
	}
	SetBkColor(DC, GetSysColor(COLOR_WINDOW));//Задаем цвет фона
	TextOut(DC, nPosX, nPosY + 40, S, strlen(S));//Выводим строку
	ReleaseDC(hwnd, DC);//Освобождаем контекст
}


//=== Обработчик сообщения WM_LBUTTONDOWN, WM_LBUTTONDBLCLK ==========//
void km_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x,
	int y, UINT keyFlags)
{
	char S[100];//Буфер для формирования выводимой строки(100 байт)
	HDC DC = GetDC(hwnd);//Получаем контекст устройства графического вывода

	if (fDoubleClick) //Двойной щелчек
		wsprintf(S, "WM_LBUTTONDBLCLK ==> Db = %d x = %d y = %d Flags = %d ",
			fDoubleClick, x, y, keyFlags);
	else // Одиночный щелчек 
	{
		g_fDraw = TRUE;
		MoveToEx(g_HDC, x, y, NULL);
		SetCapture(hwnd);

		wsprintf(S, "WM_LBUTTONDOWN ==> Db = %d x = %d y = %d Flags = %d ",
			fDoubleClick, x, y, keyFlags);
	}

	SetBkColor(DC, GetSysColor(COLOR_WINDOW));//Задаем цвет фона
	TextOut(DC, nPosX, nPosY + 100, S, strlen(S));//Выводим строку
	ReleaseDC(hwnd, DC);//Освобождаем контекст
}

//=== Обработчик сообщения WM_LBUTTONUP ==============================//
void km_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	char S[100];//Буфер для формирования выводимой строки(100 байт)
	HDC  DC = GetDC(hwnd);//Получаем контекст устройства графического вывода

	wsprintf(S, "MM LBUTTONUP ==> x = %d y = %d F = %d   ",
		x, y, keyFlags);

	g_fDraw = FALSE;
	ReleaseCapture();

	SetBkColor(DC, GetSysColor(COLOR_WINDOW));
	TextOut(DC, nPosX, nPosY + 120, S, strlen(S));
	ReleaseDC(hwnd, DC);//Освобождаем контекст
}

//=== Обработчик сообщения WM_MOUSEMOVE ==============================//
void km_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{

	char S[100];//Буфер для формирования выводимой строки(100 байт)
	HDC DC = GetDC(hwnd);//Получаем контекст устройства графического вывода

	wsprintf(S, "WM_MOUSEMOVE ==> x = %d y = %d keyFlags = %d    ",
		x, y, keyFlags);
	//Задаем цвет в зависимости от нажатых клавиш мыши и клавиатуры 
	if ((keyFlags & MK_CONTROL) == MK_CONTROL) SetTextColor(DC, RGB(0, 0, 255));
	if ((keyFlags & MK_LBUTTON) == MK_LBUTTON) SetTextColor(DC, RGB(0, 255, 0));
	if ((keyFlags & MK_RBUTTON) == MK_RBUTTON) SetTextColor(DC, RGB(255, 0, 0));
	if ((keyFlags & MK_SHIFT) == MK_SHIFT)   SetTextColor(DC, RGB(255, 0, 255));
	
	if(g_fDraw)
	LineTo(g_HDC, x, y);
	
	SetBkColor(DC, GetSysColor(COLOR_WINDOW)); //Устанавливаем цвет фона
	TextOut(DC, nPosX, nPosY + 80, S, strlen(S));// Выводим строку текста

	//MoveToEx(DC, x, y, NULL);


	ReleaseDC(hwnd, DC);//Освобождаем контекст
}



//=== Обработчик сообщений WM_SYSKEYDOWN и WM_SYSKEYUP ===============//
void km_OnSysKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	char S[100];//Буфер для формирования выводимой строки(100 байт)
	HDC DC = GetDC(hwnd); //Получаем контекст устройства графического вывода
	SetBkColor(DC, GetSysColor(COLOR_WINDOW)); //Задаем цвет

	if (fDown)
	{// Системная клавиша нажата
		wsprintf(S, "WM_SYSKEYDOWN ==> vk = %d fDown = %d cRepeat = %d flags = %d     ",
			vk, fDown, cRepeat, flags);
		TextOut(DC, nPosX, nPosY + 60, S, strlen(S));
		FORWARD_WM_SYSKEYDOWN(hwnd, vk, cRepeat, flags, DefWindowProc);
	}
	else
	{// Системная клавиша отпущена
		wsprintf(S, "WM_SYSKEYUP == > vk = %d fDown = %d cRepeat = %d flags = %d      ",
			vk, fDown, cRepeat, flags);
		TextOut(DC, nPosX, nPosY + 60, S, strlen(S));
		FORWARD_WM_SYSKEYUP(hwnd, vk, cRepeat, flags, DefWindowProc);
	}
	ReleaseDC(hwnd, DC);//Освобождаем контекст устройства
}

//=== Обработчик сообщения WM_PAINT ==================================//
void km_OnPaint(HWND hwnd)
{
	PAINTSTRUCT PaintStruct;
	RECT Rect;
	//Строки 1 и 2 вверху экрана
	static char* lpszTitle1 = "ДЕМОНСТРАЦИЯ ПЕРЕХВАТА ВВОДА МЫШИ И КЛАВИАТУРЫ";
	static char* lpszTitle2 = "Поэкпериментируйте с мышью и клавиатурой";

	//Массив указателей на строки инициализации экрана 
	static char* SList[] =
	{
		"WM_CHAR     ",
		"WM_KEY      ",
		"WM_SYSKEY   ",
		"WM_MOUSEMOVE",
		"WM_MOUSEDOWN",
		"WM_MOUSEUP  "
	};
	char S[100], S1[100]; //Буферы для формирования строки инициализации экрана
						  //Заполняем массив S1 87 пробелами и закрываем нулем
	memset(&S1[0], ' ', 87);
	S1[87] = '\0';

	HDC PaintDC = BeginPaint(hwnd, &PaintStruct);//Получаем контекст
	SetBkColor(PaintDC, GetSysColor(COLOR_WINDOW));//Задаем цвет
	GetClientRect(hwnd, &Rect);//Получаем координаты клиентной области
	DrawText(PaintDC, lpszTitle1, -1, &Rect, DT_CENTER);//Выводим строку 1

	Rect.top = 20; //Задаем координату Х верха полосы вывода
	Rect.bottom = 40; //Задаем координату Х низа полосы вывода
	DrawText(PaintDC, lpszTitle2, -1, &Rect, DT_CENTER); //Выводим строку 2 

	for (int i = 0; i < 6; i++)
	{// Выводим 6 строк с указателями из SList и дополненных 87 пробелами
		strcpy_s(S, SList[i]);//Копируем в S строку с указателем из SList[i]
		strcat_s(S, S1);// Объединяем строку S со строкой S1    
		TextOut(PaintDC, nPosX, nPosY + (20 * (i + 1)), S, strlen(S));
	}
	EndPaint(hwnd, &PaintStruct);//Освобождаем контекст устройства

}

//====================================================================//\