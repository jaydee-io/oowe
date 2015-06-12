#include <oowe/oowe.h>
#include <iostream>
#include <iomanip>
#include <fstream>

template<class Clock, class Duration = typename Clock::duration>
std::ostream & operator << (std::ostream & os, std::chrono::time_point<Clock, Duration> point)
{
    std::time_t      timePoint = std::chrono::system_clock::to_time_t(point);
    struct std::tm * tmPoint = std::localtime(&timePoint);

    os << std::put_time(tmPoint, "%c");

    return os;
}

int main(int argc, char ** argv)
{
    oowe::StdOutputStream<std::ofstream> output("/dev/null");
    oowe::StdInputStream <std::ifstream> input ("README.md");
    oowe::HttpSession session;

    session.setFileTime(true);
    session.setOutputStream(&output);
    session.setInputStream (&input );
    session.setUpload(true);
    session.setFollowLocation(true);

    for(int i=1 ; i<argc; i++)
    {
        session.setUrl(argv[i]);

        std::cout << "***********************************************************************" << std::endl;
        std::cout << "BEGIN Fetching \"" << session.getEffectiveUrl() << '"' << std::endl;
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        session.perform();
        std::chrono::time_point<std::chrono::system_clock> end   = std::chrono::system_clock::now();

        std::cout << "    HTTP response code = " << session.getResponseCode() << std::endl;
        std::cout << "    File time = \"" << session.getFileTime() << '"' << std::endl;


        std::chrono::duration<double> elapsed = end - start; 
        std::cout << "    START " << start << std::endl;
        std::cout << "        |  " << std::endl << std::fixed;
        std::cout << "        |---> NAME LOOKUP                                   = " << session.getNameLookupTime()   .count() << 's' << std::endl;
        std::cout << "        |-----|---> CONNECT                                 = " << session.getConnectTime()      .count() << 's' << std::endl;
        std::cout << "        |-----|-----|---> APP CONNECT                       = " << session.getAppConnectTime()   .count() << 's' << std::endl;
        std::cout << "        |-----|-----|-----|---> PRETRANSFER                 = " << session.getPretransferTime()  .count() << 's' << std::endl;
        std::cout << "        |-----|-----|-----|-----|---> START TRANSFERT       = " << session.getStarttransferTime().count() << 's' << std::endl;
        std::cout << "        |-----|-----|-----|-----|-----|---> TOTAL           = " << session.getTotalTime()        .count() << 's' << std::endl;
        std::cout << "        |-----|-----|-----|-----|-----|---> REDIRECT TIME   = " << session.getRedirectTime()     .count() << 's' << std::endl;
        std::cout << "    END = " << end << " (" << elapsed.count() << "s)" << std::endl;

        std::cout << "    Cookies = " << session.getCookieList() << std::endl;
        std::cout << "    SSL Engines = " << session.getSslEngines() << std::endl;

        std::cout << "END   Fetching \"" << session.getEffectiveUrl() << '"' << std::endl;
        std::cout << "***********************************************************************" << std::endl << std::endl;
    }

    return 0;
}
