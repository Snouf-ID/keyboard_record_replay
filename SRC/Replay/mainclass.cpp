#include "mainclass.h"

#include "Windows.h"

#include <fstream>
#include <vector>

struct Data {
    float f;
    int x, y;
};

std::vector<Data> readData(const std::string& filename) {
    std::vector<Data> data;

    std::ifstream infile(filename);
    float f;
    int x, y;
    while (infile >> f >> x >> y) {
        data.push_back({ f, x, y });
    }

    return data;
}

#include <stdio.h>

clock_t start_time;

void click_func(int x, int y)
{
    INPUT Inputs[2] = { 0 };

    //Inputs[0].type = INPUT_MOUSE;
    //Inputs[0].mi.dx = x; // desired X coordinate
    //Inputs[0].mi.dy = y; // desired Y coordinate
    //Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    SetCursorPos(x, y);

    Inputs[0].type = INPUT_MOUSE;
    Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    Inputs[1].type = INPUT_MOUSE;
    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, Inputs, sizeof(INPUT));
}

int main(int argc, char* argv[]) {

    while (true)
    {
        if (GetAsyncKeyState(VK_DELETE))
        {
            break;
        }
        Sleep(10);
    }

    if (argc < 2) {
        printf("Veuillez glisser-déposer un fichier sur l'exe.\n");
        return 1;
    }

    std::string filename = argv[1];
    std::vector<Data> data = readData(filename);

    int cmptv = 0;
    while (cmptv < 25)
    {
        start_time = clock();
        bool continue_ = true;
        int cmpt = 0;

        while (continue_)
        {
            clock_t current_time = clock();
            float elapsed_time = (float)(current_time - start_time) / CLOCKS_PER_SEC;

            if (data.size() > cmpt)
            {
                if (data[cmpt].f < elapsed_time)
                {
                    click_func(data[cmpt].x, data[cmpt].y);
                    cmpt = cmpt + 1;
                }
            }
            else
            {
                break;
            }
            Sleep(10);
        }
        cmptv = cmptv + 1;
    }

    // Utilisez le vecteur de données ici ...

    return 0;
}