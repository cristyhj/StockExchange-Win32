// Win32-StockExchange.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32-StockExchange.h"

#define MAX_LOADSTRING 100
#define MAX_GFRECORDS 80

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HFONT myFont;									// change font for main window
Heap heap;										// keep information about Heap
Company investition;							// keep information about the investition
HBITMAP hBitmap = NULL;
int xGfPosition[MAX_GFRECORDS];								// pozitia pe grafic X
int yGfPosition[MAX_GFRECORDS];								// pozitia pe grafic Y
int iCurr = 0;									// indexii I

HWND pushButton;								// Butoane si edit
HWND companyNameBox;
HWND incomeBox;
HWND investButton;
HWND investSumBox;
HWND withdrawButton;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Dialog1(HWND, UINT, WPARAM, LPARAM);
void				PushButtonAction();
void				InvestButtonAction();
void				WithdrawButtonAction(HWND);
int					TransformFloat(float);
void				UpdateGf(HWND, int);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	srand(time(NULL));
	heap.Load();
			// Impartim coord X a gf in MAX_GF parti
	for (int i = 370; iCurr < MAX_GFRECORDS; i += (330 / MAX_GFRECORDS)) {
		xGfPosition[iCurr++] = i;
	}
			// Initializari:
	iCurr = 0;
	investition.income = 0;

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32STOCKEXCHANGE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32STOCKEXCHANGE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32STOCKEXCHANGE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+0);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32STOCKEXCHANGE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, 780, 480, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // Create windows:: butoane si text
   HWND group;

   pushButton = CreateWindowEx(0, L"BUTTON", L"Add Company", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
	   20, 90, 275, 30, hWnd, NULL, NULL, NULL);
   companyNameBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT,
	   60, 30, 235, 20, hWnd, NULL, NULL, NULL);
   incomeBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_NUMBER,
	   60, 60, 235, 20, hWnd, NULL, NULL, NULL);
   investButton = CreateWindowEx(0, L"BUTTON", L"Invest!", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
	   20, 190, 275, 30, hWnd, NULL, NULL, NULL);
   investSumBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_NUMBER,
	   60, 160, 235, 20, hWnd, NULL, NULL, NULL);
   withdrawButton = CreateWindowEx(0, L"BUTTON", L"Vindeti Acum!", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
	   20, 240, 275, 40, hWnd, NULL, NULL, NULL);

   group = CreateWindowEx(WS_EX_CONTROLPARENT, L"BUTTON", L"Adaugati o companie:", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP,
	   10, 10, 300, 120, hWnd, NULL, NULL, NULL);
   SendMessage(group, WM_SETFONT, WPARAM(myFont), TRUE);
   group = CreateWindowEx(WS_EX_CONTROLPARENT, L"BUTTON", L"Faceti o investitie:", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP,
	   10, 140, 300, 90, hWnd, NULL, NULL, NULL);
   SendMessage(group, WM_SETFONT, WPARAM(myFont), TRUE);


   SendMessage(pushButton, WM_SETFONT, WPARAM(myFont), TRUE);
   SendMessage(companyNameBox, WM_SETFONT, WPARAM(myFont), TRUE);
   SendMessage(incomeBox, WM_SETFONT, WPARAM(myFont), TRUE);
   SendMessage(investButton, WM_SETFONT, WPARAM(myFont), TRUE);
   SendMessage(investSumBox, WM_SETFONT, WPARAM(myFont), TRUE);
   SendMessage(withdrawButton, WM_SETFONT, WPARAM(myFont), TRUE);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			myFont = CreateFont(-MulDiv(12, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 100),
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"MS Shell Dlg");
			// timer pentru grafic
			int ret = SetTimer(hWnd, 1, 250, NULL);
			if (ret == 0)
				MessageBox(hWnd, L"Could not SetTimer()!", L"Error", MB_OK | MB_ICONEXCLAMATION);
			hBitmap = (HBITMAP)LoadImage(hInst, L"graph.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			SelectObject(hdc, myFont);
			RECT rect;
			SetBkColor(hdc, RGB(240, 240, 240));
			
			SetRect(&rect, 20, 33, 100, 100);
			DrawText(hdc, L"Name:", -1, &rect, DT_BOTTOM | DT_NOCLIP);
			SetRect(&rect, 20, 63, 100, 100);
			DrawText(hdc, L"Income:", -1, &rect, DT_BOTTOM | DT_NOCLIP);
			SetRect(&rect, 20, 163, 100, 100);
			DrawText(hdc, L"Money:", -1, &rect, DT_BOTTOM | DT_NOCLIP);

			//Load bitMap:
			BITMAP 			bitmap;
			HDC 			hdcMem;
			HGDIOBJ 		oldBitmap;

			hdcMem = CreateCompatibleDC(hdc);
			oldBitmap = SelectObject(hdcMem, hBitmap);

			GetObject(hBitmap, sizeof(bitmap), &bitmap);
			BitBlt(hdc, 340, 10, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

			SelectObject(hdcMem, oldBitmap);
			DeleteDC(hdcMem);
			
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		case BN_CLICKED:
			// place some if's here for every button
			if (pushButton == (HWND)lParam) {
				PushButtonAction();
			}
			if (investButton == (HWND)lParam) {
				if (!investition.income)
					InvestButtonAction();
			}
			if (withdrawButton == (HWND)lParam) {
				if (investition.income)
					WithdrawButtonAction(hWnd);
			}
			break;
		case ID_HELP_INFO:
		{
			HWND aDiag = CreateDialog(hInst,
				MAKEINTRESOURCE(IDD_DIALOG1),
				hWnd,
				(DLGPROC)Dialog1);
			// check for errors
			if (aDiag == NULL) {
				WCHAR dwLE = GetLastError();
				LPWSTR szLE = (LPWSTR)malloc(sizeof(WCHAR) * 16);
				_ltow_s(dwLE, szLE, 16, 10);
				MessageBox(hWnd, szLE, TEXT("Error"), MB_OK);
			}
			ShowWindow(aDiag, SW_SHOW);	
		}
			break;
		case IDM_ABOUT:
			DialogBox(hInst,
				MAKEINTRESOURCE(IDD_ABOUTBOX),
				hWnd,
				(DLGPROC)Dialog1);
			break;
		case ID_FILE_RESETHEAP:
			if (!investition.income)
				heap.Free();
			else
				MessageBox(hWnd, TEXT("O investitie in desfasurare. Vindeti inainte de a reseta Heap-ul."), TEXT("Error"), MB_OK);
			break;
		case ID_FILE_SAVE:
			heap.Save();
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_TIMER:
		{
			// trece si schimba random fiecare inregistrare din heap
			for (int i = 0; i < heap.Get(); i++) {
				heap.RandomizeVary(i);
			}
			// se updateaza graficul cu inca o liniuta
			if (investition.income) {
				Company temp = heap.ReturnCompany(heap.Search(investition, 0));
				yGfPosition[iCurr] = TransformFloat(temp.variation);
				if (iCurr) {
					UpdateGf(hWnd, iCurr);
					if (iCurr == MAX_GFRECORDS) {
						iCurr = 0;
						break;
					}
				}
				iCurr++;
			}
		}
		break;
    case WM_DESTROY:
		KillTimer(hWnd, 1);
		heap.Save();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Dialog1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		SendMessage(hDlg, WM_CREATE, wParam, lParam);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CREATE:
	{
		HWND hText = GetDlgItem(hDlg, IDC_EDIT1);
		LPWSTR infoLP = heap.InfoList();
		SetWindowText(hText, infoLP);
		free(infoLP);
	}
	}
	return (INT_PTR)FALSE;
}


// Button functions


void PushButtonAction() {
	LPWSTR companyNameLPWSTR = (LPWSTR)malloc(sizeof(WCHAR) * MAX_PATH);
	LPWSTR incomeLPWSTR = (LPWSTR)malloc(sizeof(WCHAR) * MAX_PATH);

	GetWindowText(companyNameBox, companyNameLPWSTR, MAX_PATH);
	GetWindowText(incomeBox, incomeLPWSTR, MAX_PATH);
	if (wcslen(companyNameLPWSTR) == 0 || wcslen(incomeLPWSTR) == 0) {
		MessageBox(NULL, L"Intoduceti valorile!", L"Error", MB_OK | MB_ICONINFORMATION);
		return;
	}

	Company comp;
	wcscpy_s(comp.name, companyNameLPWSTR);
	comp.income = _wtoi(incomeLPWSTR);
	comp.variation = 0;
	heap.Push(comp);

	MessageBox(NULL, L"Compania a fost adaugata cu succes", L"Succes!", MB_OK);
	SetWindowText(companyNameBox, L"");
	SetWindowText(incomeBox, L"");

	free(companyNameLPWSTR);
	free(incomeLPWSTR);
}

void InvestButtonAction() {
	LPWSTR investLPWSTR = (LPWSTR)malloc(sizeof(WCHAR) * MAX_PATH);
	LPWSTR message = (LPWSTR)malloc(sizeof(WCHAR) * MAX_PATH);
	GetWindowText(investSumBox, investLPWSTR, MAX_PATH);
	if (wcslen(investLPWSTR) == 0) {
		MessageBox(NULL, L"Intoduceti valorile!", L"Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	
	int sum = _wtoi(investLPWSTR);
	investition = heap.Peek(sum);

	if (investition.income < 0) {
		int cIncome = investition.income + sum;
		investition.income = 0;
		heap.Peek(-sum);

		wcscpy_s(message, MAX_PATH, L"Nu puteti cumpara mai mult decat detine compania!\r\nCompania detine ");
		_itow_s(cIncome, investLPWSTR, MAX_PATH, 10);
		wcscat_s(message, MAX_PATH, investLPWSTR);
		wcscat_s(message, MAX_PATH, L" $.");
		MessageBox(NULL, message, L"Info", MB_OK | MB_ICONINFORMATION);
		
		free(investLPWSTR);
		free(message);
		return;
	}

	investition.income = sum;

	// do sth with *invest
	wcscpy_s(message, MAX_PATH, L"Ai investit ");
	wcscat_s(message, MAX_PATH, investLPWSTR);
	wcscat_s(message, MAX_PATH, L" in ");
	wcscat_s(message, MAX_PATH, investition.name);
	// sth more than write it
	MessageBox(NULL, message, L"Mesaj", MB_OK);

	free(investLPWSTR);
	free(message);
}

void WithdrawButtonAction(HWND hWnd) {
	int sum = investition.income;
	Company investitionNow = heap.ReturnCompany(heap.Search(investition, 0));
	float win = investitionNow.variation - investition.variation;
	win *= sum;
		// Reinitializari:
	heap.Peek((int)-win);
	investition.income = 0;
	UpdateGf(hWnd, MAX_GFRECORDS);
	iCurr = 0;

	LPWSTR message = (LPWSTR)malloc(sizeof(WCHAR) * MAX_PATH);
	LPWSTR winLP = (LPWSTR)calloc(sizeof(WCHAR), MAX_PATH);

	if (win > 0)
		wcscpy_s(message, MAX_PATH, L"Ai castigat ");
	else
		wcscpy_s(message, MAX_PATH, L"Ai pierdut  ");


	_itow_s((int)win, winLP, 200, 10);
	wcscat_s(message, MAX_PATH, winLP);
	wcscat_s(message, MAX_PATH, L" $!!!");
	MessageBox(NULL, message, L"Mesaj", MB_OK);

	free(message);
	free(winLP);
}

int TransformFloat(float f) {
	// Duce intervalul [-20; 20] -> [75; 360]
	if (f < -20) f = -24;
	if (f > 20) f = 24;
	f += 24;
	f = f * (360 - 75) / 48;
	f = 285 - f;
	f += 75;
	return (int)f;
}

void UpdateGf(HWND hWnd, int iCurr) {
	HDC hdc = GetDC(hWnd);
	if (iCurr == MAX_GFRECORDS) {
		//Cod de adaugat pentru cazul in care se termina graficul
					// mai desenez o data peste bitmap-ul
		iCurr = 0;

		BITMAP 			bitmap;
		HDC 			hdcMem;
		HGDIOBJ 		oldBitmap;

		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);

		GetObject(hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 340, 10, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
		
		ReleaseDC(hWnd, hdc);
		return;
	}

	DrawALine(hdc, RGB(255, 0, 0), 3, xGfPosition[iCurr - 1], yGfPosition[iCurr - 1], xGfPosition[iCurr], yGfPosition[iCurr]);


	ReleaseDC(hWnd, hdc);
}