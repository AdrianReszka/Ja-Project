#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <iostream>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("WindowsApp");

HWND hTextFieldFilePath, hTextBoxFilePath, hTextFieldThreads, hUpDownThreads, hEditThreads,
hUpDownFactor, hEditFactor, hTextFieldFactor, hButtonASM, hButtonCPP, OKButton,
hTextFieldTime, hTextBoxTime; // Dodanie pól do wyœwietlania czasu

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the window class */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = CreateSolidBrush(0x00F0F0F0);  // Light gray background

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program */
    hwnd = CreateWindowEx(
        0,                   /* Extended possibilites for variation */
        szClassName,         /* Classname */
        _T("Sepia Converter"),       /* Title Text */
        WS_MINIMIZEBOX | WS_SYSMENU, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        600,                 /* The program's width */
        350,                 /* Zwiêkszenie wysokoœci dla pola czasu */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        // File path input
        hTextFieldFilePath = CreateWindow("STATIC", "File path:", WS_VISIBLE | WS_CHILD, 20, 20, 150, 20, hwnd, NULL, NULL, NULL);
        hTextBoxFilePath = CreateWindow("EDIT", "", WS_BORDER | WS_VISIBLE | WS_CHILD, 120, 20, 400, 20, hwnd, NULL, NULL, NULL);

        // Threads input
        hTextFieldThreads = CreateWindow("STATIC", "Number of threads: (1-64)", WS_VISIBLE | WS_CHILD, 20, 50, 150, 40, hwnd, NULL, NULL, NULL);
        hUpDownThreads = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        hEditThreads = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT, 160, 50, 70, 25, hwnd, NULL, NULL, NULL);
        SendMessageW(hUpDownThreads, UDM_SETBUDDY, (WPARAM)hEditThreads, 0);
        SendMessageW(hUpDownThreads, UDM_SETRANGE, 0, MAKELPARAM(64, 1));
        SendMessageW(hUpDownThreads, UDM_SETPOS32, 0, 0);

        // Factor input
        hTextFieldFactor = CreateWindow("STATIC", "Factor (20-40):", WS_VISIBLE | WS_CHILD, 20, 100, 130, 20, hwnd, NULL, NULL, NULL);
        hUpDownFactor = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        hEditFactor = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT, 160, 100, 70, 25, hwnd, NULL, NULL, NULL);
        SendMessageW(hUpDownFactor, UDM_SETBUDDY, (WPARAM)hEditFactor, 0);
        SendMessageW(hUpDownFactor, UDM_SETRANGE, 0, MAKELPARAM(40, 20));
        SendMessageW(hUpDownFactor, UDM_SETPOS32, 0, 0);

        // Radio buttons for choosing between ASM and C++
        CreateWindow("STATIC", "Choose DLL:", WS_VISIBLE | WS_CHILD, 20, 130, 100, 20, hwnd, NULL, NULL, NULL);
        hButtonASM = CreateWindow("BUTTON", "ASM", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 40, 155, 150, 20, hwnd, (HMENU)1, NULL, NULL);
        hButtonCPP = CreateWindow("BUTTON", "C++", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 40, 180, 150, 20, hwnd, (HMENU)2, NULL, NULL);

        // Default selection for C++
        SendDlgItemMessage(hwnd, 2, BM_SETCHECK, 1, 0);

        // Button to run the operation
        OKButton = CreateWindow("BUTTON", "Make sepia", WS_VISIBLE | WS_CHILD, 20, 220, 150, 30, hwnd, (HMENU)3, NULL, NULL);

        // Dodanie pola wyœwietlaj¹cego czas operacji
        hTextFieldTime = CreateWindow("STATIC", "Time [s]:", WS_VISIBLE | WS_CHILD, 20, 270, 100, 20, hwnd, NULL, NULL, NULL);
        hTextBoxTime = CreateWindow("EDIT", "0.000", WS_VISIBLE | WS_CHILD | ES_READONLY, 100, 270, 100, 20, hwnd, NULL, NULL, NULL);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: // ASM radio button clicked
            SendDlgItemMessage(hwnd, 1, BM_SETCHECK, 1, 0);
            SendDlgItemMessage(hwnd, 2, BM_SETCHECK, 0, 0);
            break;
        case 2: // C++ radio button clicked
            SendDlgItemMessage(hwnd, 2, BM_SETCHECK, 1, 0);
            SendDlgItemMessage(hwnd, 1, BM_SETCHECK, 0, 0);
            break;
        case 3: // "Make sepia" button clicked
            // Implement logic for the "Make sepia" button
            MessageBox(hwnd, "Sepia conversion started!", "Info", MB_OK);
            SetWindowText(hTextBoxTime, "0.123"); // Przyk³ad ustawienia czasu
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        break;

    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
