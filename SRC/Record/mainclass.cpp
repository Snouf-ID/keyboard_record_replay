#include "mainclass.h"

#include "Windows.h"
#include <iostream>

void ShowDesktop()
{
    //System.Debug.OutputString(L"Sending 'Win-D'\r\n");
    //INPUT inputs[4] = {};
    //ZeroMemory(inputs, sizeof(inputs));

    //inputs[0].type = INPUT_KEYBOARD;
    //inputs[0].ki.wVk = VK_LWIN;

    //inputs[1].type = INPUT_KEYBOARD;
    //inputs[1].ki.wVk = 0x44;

    //inputs[2].type = INPUT_KEYBOARD;
    //inputs[2].ki.wVk = 0x44;
    //inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    //inputs[3].type = INPUT_KEYBOARD;
    //inputs[3].ki.wVk = VK_LWIN;
    //inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    //UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    //if (uSent != ARRAYSIZE(inputs))
    //{
    //    //OutputString(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    //}
}

//void empty()
//{
//    INPUT Inputs[3] = { 0 };
//
//    Inputs[0].type = INPUT_MOUSE;
//    Inputs[0].mi.dx = 5; // desired X coordinate
//    Inputs[0].mi.dy = 5; // desired Y coordinate
//    Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
//
//    Inputs[1].type = INPUT_MOUSE;
//    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
//
//    Inputs[2].type = INPUT_MOUSE;
//    Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
//
//    SendInput(3, Inputs, sizeof(INPUT));
//
//    //system("pause");
//}
//
//int main()
//{
//    POINT ptCursorPos;
//    GetCursorPos(&ptCursorPos);
//    //SetCursorPos(x, y);
//    //mouse_event(MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0);
//    //mouse_event(MOUSEEVENTF_RIGHTUP, x, y, 0, 0);
//
//    UINT button = GET_XBUTTON_WPARAM(wParam);
//    if (button == XBUTTON1)
//    {
//        // XBUTTON1 was clicked.
//    }
//    else if (button == XBUTTON2)
//    {
//        // XBUTTON2 was clicked.
//    }
//
//    empty();
//}

#include <stdio.h>

clock_t start_time;

// Déclaration de la fonction de rappel de la souris
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // Vérifier si le clic de souris est enfoncé
    if (nCode == HC_ACTION && wParam == WM_LBUTTONDOWN)
    {
        // Clic de souris détecté, obtenir les coordonnées de la souris
        POINT cursorPosition;
        GetCursorPos(&cursorPosition);

        clock_t current_time = clock();
        double elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;

        // Écrire les informations de position de la souris dans un fichier texte
        FILE* fp;
        errno_t err = fopen_s(&fp, "mouse_positions.txt", "a");
        if (err != 0)
        {
            printf("Impossible d'ouvrir le fichier\n");
            exit(1);
        }
        if (fp != NULL)
        {
            fprintf(fp, "%.2f %ld %ld\n", elapsed_time, cursorPosition.x, cursorPosition.y);
            fclose(fp);
        }
    }

    // Passer l'appel à la procédure de rappel suivante dans la chaîne
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
    while(true)
    {
        if (GetAsyncKeyState(VK_DELETE))
        {
            break;
        }
        Sleep(10);
    }

    if (remove("mouse_positions.txt") != 0) {
        //perror("Erreur lors de la suppression du fichier");
    }
    // Obtenir un handle vers le module de l'application
    HINSTANCE hInstance = GetModuleHandle(NULL);

    start_time = clock();
    // Installer la fonction de rappel de la souris
    HHOOK hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);

    if (hMouseHook == NULL)
    {
        printf("Impossible d'installer le hook de souris\n");
        return 1;
    }

    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // Si un message est disponible, le récupérer
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            if (GetAsyncKeyState(VK_ESCAPE))
            {
                break;
            }
        }
        Sleep(10);
    }

    // Désinstaller la fonction de rappel de la souris
    UnhookWindowsHookEx(hMouseHook);

    return 0;
}