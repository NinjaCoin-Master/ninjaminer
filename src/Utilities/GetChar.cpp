// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

////////////////////
#include "GetChar.h"
////////////////////

#include <iostream>

#if _WIN32

    #include "Console.h"

    #include <conio.h>

    #define ETX 3

    char getCharNoBuffer()
    {
        char c = _getch();

        if (static_cast<int>(c) == ETX)
        {
            Console::exitOrWaitForInput(0);
        }

        return c;
    }

#else

    #include <termios.h>

    static termios oldTerm;
    static termios newTerm;

    char getCharNoBuffer()
    {
        tcgetattr(0, &oldTerm);

        newTerm = oldTerm;

        newTerm.c_lflag &= ~ICANON;
        newTerm.c_lflag &= ~ECHO;

        tcsetattr(0, TCSANOW, &newTerm);

        const char c = getchar();

        tcsetattr(0, TCSANOW, &oldTerm);

        return c;
    }

#endif
