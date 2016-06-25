#ifndef HELPER_H
#define HELPER_H
#include <ctime>
#include <string>
#include <sstream>

namespace Helper
    {
        template <class T>
        std::string ToString (const T &); //prototype
        //helper struct to represent date and time
        struct DateTime
            {
                //constructors

                //empty, create DateTime instance and fills it with
                //current date & time
                DateTime ()
                    {
                        time_t ms;
                        time (&ms);
                        struct tm *info = localtime(&ms);
                        D = info->tm_mday;
                        m = info->tm_mon + 1;
                        y = 1900 + info->tm_year;
                        M = info->tm_min;
                        H = info->tm_hour;
                        S = info->tm_sec;
                    }
                //create a DateTime instance, user provide all the parameters
                DateTime (int D, int m, int y, int H, int M, int S)
                        : D(D), m(m), y(y), H(H), M(M), S(S) {}
                //create a DateTime instance, user provide date parameters
                //time parameters are set to 0
                DateTime (int D, int m, int y)
                    : D(D), m(m), y(y), H(0), M(0), S(0) {}
                //get current date time
                DateTime Now () const
                    {
                        //just call of the constructor
                        return DateTime();
                    }
                //attributes
                int D, m, y, H, M, S;
                //returns date in format "DD.mm.YYYY"
                std::string GetDateString() const
                    {
                        //fills with zero if D or m are single-digit
                        //for example "02.05.2015"
                        return std::string(D < 10 ? "0" : "") + ToString (D) +
                               std::string (m < 10 ? ".0" : ".") + ToString (m) + "." +
                               ToString (y);
                    }
                //returns time in format "HH:MM:SS"
                std::string GetTimeString(const std::string &sep = ":") const
                    {
                        //fills with zero if H, M or s are single-digit
                        //for example "00:01:03"
                        //additionally, separator is specified, that is because we will use this
                        //function to save logs, and : is not a valid windows path character
                        //we may pass "_" for example...
                        return std::string(H < 10 ? "0" : "") + ToString (H) + sep +
                               std::string (M < 10 ? "0" : "") + ToString (M) + sep+
                               std::string (S < 10 ? sep : "") + ToString (S);
                    }
                //return combined date time, format "DD.mm.YYYY HH:MM::SS"
                //or sep instead of ":"
                std::string GetDateTimeString (const std::string &sep = ":") const
                    {
                        return GetDateString () + " " + GetTimeString (sep);
                    }
            };
        //generic function that will try to convert anything to string
        //"anything" have to support output via << operator
        template <class T>
        std::string ToString (const T &e)
            {
                //output string stream object
                std::ostringstream s;
                //"e" must support << operator
                s << e;
                //return strream to string
                return s.str();
            }
        //this function is temporary, it writes application log to AppLog.txt file
        //it is used for debugging
        void WriteAppLog (const std::string &s)
            {
            //file, for appending
            std::ofstream file ("AppLog.txt", std::ios::app);
            //dump data
            file << "[" << Helper::DateTime().GetDateTimeString () << "]" << "\n" << s << std::endl << "\n";
            file.close();
            }
    }
#endif //HELPER_H
