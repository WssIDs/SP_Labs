// Line_drawing.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

HWND hFrameWnd=NULL;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK FrameDlgProc(HWND, UINT, WPARAM, LPARAM); 


LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LINE_DRAWING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_LINE_DRAWING);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_LINE_DRAWING);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_LINE_DRAWING;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc; 
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);


	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_KEYDOWN:
		static				RECT wi;
    POINT ptClientUpL;              // Клиентский верхний левый угол
    POINT ptClientLowR;              // Клиентский правый нижний угол 


		GetWindowRect(hFrameWnd,&wi);
		ptClientUpL.x=wi.left;
		ptClientUpL.y=wi.top;
		ptClientLowR.x=wi.right;
		ptClientLowR.y=wi.bottom;
		ScreenToClient(hWnd,&ptClientUpL);
		ScreenToClient(hWnd,&ptClientLowR);
            switch (wParam) 
            { 
			case 'D'://VK_LEFT: 
                    
                    // Process the LEFT ARROW key. 
                     MoveWindow(hFrameWnd,
											 ptClientUpL.x-5,
											ptClientUpL.y,
											ptClientLowR.x-ptClientUpL.x,
											ptClientLowR.y-ptClientUpL.y,
											TRUE);
                    break; 
 
                case VK_RIGHT: 
                    
                    // Process the RIGHT ARROW key. 
                     MoveWindow(hFrameWnd,
											 ptClientUpL.x+5,
											ptClientUpL.y,
											ptClientLowR.x-ptClientUpL.x,
											ptClientLowR.y-ptClientUpL.y,
											TRUE);							
                     
                    break; 
 
                case VK_UP: 
                    
                    // Process the UP ARROW key. 
                     
                    break; 
 
                case VK_DOWN: 
                  
                    // Process the DOWN ARROW key. 
                     
                    break; 
 
                case VK_F2: 
                    
                    // Process the F2 key. 
                    
                    break; 
 
                
                // Process other non-character keystrokes. 
                 
                default: 
                    break; 
            } 
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			{
				hFrameWnd=CreateDialog((HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE),
					"FrameTemplate",hWnd,(DLGPROC)FrameDlgProc);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

BOOL CALLBACK FrameDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    HDC hdc;                       // дескриптор контекста устройства
    RECT rcClient;                 // Прямоугольник клиентской области 
    POINT ptClientUL;              // Клиентский верхний левый угол
    POINT ptClientLR;              // Клиентский правый нижний угол 
    static POINTS ptsBegin;        // Стартовая точка
    static POINTS ptsEnd;          // Новая концевая точка 
    static POINTS ptsPrevEnd;      // Предыдущая концевая точка 
    static BOOL fPrevLine = FALSE; // Предыдущий флаг линии 


    switch (message) 
    { 
     // Place message cases here. 

        case WM_LBUTTONDOWN: 
 
            // Захват ввода мыши 
             SetCapture(hWnd); 
 
            // Получение аппаратных координат дисплея для клиентской области,
            // и преобразование их в клиентские координаты.. 
            GetClientRect(hWnd, &rcClient); 
            ptClientUL.x = rcClient.left; 
            ptClientUL.y = rcClient.top; 
 
            // Добавляем по единице к правой и нижней сторонам,
            // потому что координаты, полученные через  GetClientRect 
            // не включают самый левый и самый нижний пикселы.
 
            ptClientLR.x = rcClient.right + 1; 
            ptClientLR.y = rcClient.bottom + 1; 
            ClientToScreen(hWnd, &ptClientUL); 
            ClientToScreen(hWnd, &ptClientLR); 
 
            // Копируем клиентные координаты клиентной области в структуру rcClient 
            // Передавая  структуру rcClient  функции ClipCursor ограничим 
            // перемешения курсора мыши клиентской областью.
             SetRect(&rcClient, ptClientUL.x, ptClientUL.y, 
                           ptClientLR.x, ptClientLR.y); 
            ClipCursor(&rcClient); 
 
            // Конвертируем координаты курсора в структуру POINTS, 
            // которая задает стартовую точку линии, 
            // выводимой при обработке сообщения WM_MOUSEMOVE. 
             ptsBegin = MAKEPOINTS(lParam); 
            return 0; 
 
        case WM_MOUSEMOVE: 
 
            // Для рисования линиии при перемещении мыши нужно удерживать
           // нажатой левую кнопку мыши 
             if (wParam & MK_LBUTTON) 
            { 
                 // Получаем контекст устройства для клиентной области 
                 hdc = GetDC(hWnd); 
                 // Следуящая функция обеспечивает вывод пиксел 
                 //  ранее нарисованной линии белым цветом, а пиксел новой линии черным 
                 SetROP2(hdc, R2_NOTXORPEN); 
 
                // Если вывод идет по линии, нарисованной при обработке предыдущего
                // сообщения  WM_MOUSEMOVE, то рисование осуществляется поверх. 
               // Установка цвета к белому приводит к стиранию линии 
                if (fPrevLine) 
                { 
                    MoveToEx(hdc, ptsBegin.x, ptsBegin.y, 
                        (LPPOINT) NULL); 
                    LineTo(hdc, ptsPrevEnd.x, ptsPrevEnd.y); 
                } 
 
                // Пеобразование текущих координат курсора к структуре 
                // POINTS  и рисование новой линии. 
 
                ptsEnd = MAKEPOINTS(lParam); 
                MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT) NULL); 
                LineTo(hdc, ptsEnd.x, ptsEnd.y); 
 
                //  Устанавливае флаг предыдущей линии, сохраняем конечную точку
                //  новой линии и освобождаем  контекст устройства 
                 fPrevLine = TRUE; 
                ptsPrevEnd = ptsEnd; 
                ReleaseDC(hWnd, hdc); 
            } 
            break; 
 
        case WM_LBUTTONUP: 
 
            // Пользователь закончил рисование линии. 
            // Сбрасываем флаг предыдущей линии,
            //  освобождаем курсор и освобождаем захват мыши 
            fPrevLine = FALSE; 
            ClipCursor(NULL); 
            ReleaseCapture(); 
            return 0; 

			//////////////////////////

        default: return FALSE;
             
    } return FALSE;
} 


// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
