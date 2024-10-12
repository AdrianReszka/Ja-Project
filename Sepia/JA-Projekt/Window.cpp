#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <chrono> // Do obs³ugi czasu

/*  Deklaracja procedury okna  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Globalna zmienna z nazw¹ klasy  */
TCHAR szClassName[] = _T("WindowsApp");

HWND hTextFieldFilePath, hTextBoxFilePath, hTextFieldThreads, hTextFieldThreadRange, hUpDownThreads, hEditThreads,
hUpDownFactor, hEditFactor, hTextFieldFactor, hButtonASM, hButtonCPP, OKButton, hTimeDisplay;

// Zmienna globalna dla timera
std::chrono::steady_clock::time_point startTime;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    /* Ustawienia klasy okna */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = CreateSolidBrush(0x00F0F0F0); // Kolor t³a

    /* Rejestracja klasy okna */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* Tworzenie okna */
    hwnd = CreateWindowEx(
        0,
        szClassName,
        _T("Interactive GUI"),
        WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,
        363,
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        NULL
    );

    /* Wyœwietlenie okna */
    ShowWindow(hwnd, nCmdShow);

    /* Pêtla wiadomoœci */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

/* Funkcja obs³ugi wiadomoœci */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Pole tekstowe do wpisania œcie¿ki pliku
        hTextFieldFilePath = CreateWindow("STATIC", "File path:", WS_VISIBLE | WS_CHILD, 20, 20, 150, 20, hwnd, NULL, NULL, NULL);
        hTextBoxFilePath = CreateWindow("EDIT", "", WS_BORDER | WS_VISIBLE | WS_CHILD, 120, 20, 400, 20, hwnd, NULL, NULL, NULL);

        // Kontrolki do wprowadzania liczby w¹tków - tytu³
        hTextFieldThreads = CreateWindow("STATIC", "Number of threads:", WS_VISIBLE | WS_CHILD, 20, 50, 150, 20, hwnd, NULL, NULL, NULL);
        hTextFieldThreadRange = CreateWindow("STATIC", "(1-64):", WS_VISIBLE | WS_CHILD, 20, 70, 150, 20, hwnd, NULL, NULL, NULL); // Nowa kontrolka dla zakresu

        // Ustawienia liczby w¹tków
        hUpDownThreads = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        hEditThreads = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT, 160, 50, 70, 25, hwnd, NULL, NULL, NULL);
        SendMessageW(hUpDownThreads, UDM_SETBUDDY, (WPARAM)hEditThreads, 0);
        SendMessageW(hUpDownThreads, UDM_SETRANGE, 0, MAKELPARAM(64, 1));

        // Kontrolka do wprowadzania wspó³czynnika
        hTextFieldFactor = CreateWindow("STATIC", "Factor (20-40):", WS_VISIBLE | WS_CHILD, 20, 120, 130, 20, hwnd, NULL, NULL, NULL);
        hUpDownFactor = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        hEditFactor = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT, 160, 120, 70, 25, hwnd, NULL, NULL, NULL);
        SendMessageW(hUpDownFactor, UDM_SETBUDDY, (WPARAM)hEditFactor, 0);
        SendMessageW(hUpDownFactor, UDM_SETRANGE, 0, MAKELPARAM(40, 20));

        // Przycisk wyboru ASM i C++
        CreateWindow("STATIC", "Choose DLL:", WS_VISIBLE | WS_CHILD, 20, 160, 100, 20, hwnd, NULL, NULL, NULL);
        hButtonASM = CreateWindow("BUTTON", "ASM", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 40, 185, 150, 20, hwnd, (HMENU)1, NULL, NULL);
        hButtonCPP = CreateWindow("BUTTON", "C++", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 40, 210, 150, 20, hwnd, (HMENU)2, NULL, NULL);

        // Zaznaczenie domyœlnie C++
        SendDlgItemMessage(hwnd, 2, BM_SETCHECK, 1, 0);

        // Przycisk do uruchomienia operacji
        OKButton = CreateWindow("BUTTON", "Make sepia", WS_VISIBLE | WS_CHILD, 20, 250, 150, 30, hwnd, (HMENU)3, NULL, NULL);

        // Pole do wyœwietlania czasu
        hTimeDisplay = CreateWindow("STATIC", "Time: 0.000 s", WS_VISIBLE | WS_CHILD, 20, 290, 150, 30, hwnd, NULL, NULL, NULL);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: // Klikniêcie przycisku ASM
            SendDlgItemMessage(hwnd, 1, BM_SETCHECK, 1, 0);
            SendDlgItemMessage(hwnd, 2, BM_SETCHECK, 0, 0);
            break;
        case 2: // Klikniêcie przycisku C++
            SendDlgItemMessage(hwnd, 2, BM_SETCHECK, 1, 0);
            SendDlgItemMessage(hwnd, 1, BM_SETCHECK, 0, 0);
            break;
        case 3: // Klikniêcie przycisku "Make sepia"
            // Start timera
            startTime = std::chrono::steady_clock::now();

            // Symulacja operacji (np. przetwarzanie sepia)
            Sleep(2000); // Symulacja d³ugiej operacji

            // Koniec pomiaru czasu
            auto endTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsedTime = endTime - startTime;

            // Wyœwietlenie wyniku
            char buffer[50];
            sprintf(buffer, "Time: %.3f s", elapsedTime.count());
            SetWindowText(hTimeDisplay, buffer);

            MessageBox(hwnd, "Sepia conversion finished!", "Info", MB_OK);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
