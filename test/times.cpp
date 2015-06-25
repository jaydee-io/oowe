#include <oowe/oowe.h>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <unistd.h>

using namespace std;
using namespace oowe;

#define ITER_MODULO 10

struct Site
{
    string         initialUrl;
    string         finalUrl;
    list<Duration> timeList;

    Duration       average;
    Duration       standardDeviation;
};

struct Stats
{
    map<const char *, Site> sites;
    int                     nbRun;

    int                     initialUrlMaxSize;
    int                     finalUrlMaxSize;
};





#define PADDING 3

string timeToString(Duration & val)
{
    ostringstream os;
    long long int longVal = static_cast<long long int>(val.count() * 1000000.0);
    long long int longSec      = 0;
    long long int longMilliSec = 0;
    long long int longMicroSec = 0;
    long long int longMinute   = 0;
    lldiv_t       res;

    // Micro seconds
    res = lldiv(longVal, 1000);
    longMicroSec = res.rem;
    longVal      = res.quot;

    // Milli seconds
    res = lldiv(longVal, 1000);
    longMilliSec = res.rem;
    longVal      = res.quot;

    // Seconds
    res = lldiv(longVal, 60);
    longSec = res.rem;
    longVal = res.quot;

    // Minute
    res = lldiv(longVal, 60);
    longMinute = res.rem;
    longVal   = res.quot;

    if(longMinute                                                                 ) os                                            << longMinute   << "min"; // Minutes
    if(longSec      || (!longMinute && !longSec && !longMilliSec && !longMicroSec)) os << setw(os.str().size() ? PADDING + 1 : 0) << longSec      << "s  "; // Seconds
    if(longMilliSec || ( longSec &&  longMicroSec)                                ) os << setw(os.str().size() ? PADDING + 1 : 0) << longMilliSec << "ms "; // Milli seconds
    if(longMicroSec                                                               ) os << setw(os.str().size() ? PADDING + 1 : 0) << longMicroSec << "us "; // Micro seconds

    return os.str();
}

Duration operator *(Duration & lhs, Duration & rhs)
{
    return Duration(lhs.count() * rhs.count());
}

void addSite(Stats & stats, const char * initialUrl, const char * finalUrl, Duration && time)
{
    auto siteIter = stats.sites.find(initialUrl);

    if(siteIter != stats.sites.end())
    {
        Site & site = siteIter->second;

        site.timeList.push_back(time);
        site.average           +=  time;
        site.standardDeviation += (time * time);
    }
    else
    {
        Site & site = stats.sites[initialUrl];

        site.initialUrl = initialUrl;
        site.finalUrl   = finalUrl;
        site.timeList.push_back(time);
        site.average           =  time;
        site.standardDeviation = (time * time);

        if(site.initialUrl.size() > stats.initialUrlMaxSize) stats.initialUrlMaxSize = site.initialUrl.size();
        if(site.finalUrl  .size() > stats.finalUrlMaxSize  ) stats.finalUrlMaxSize   = site.finalUrl  .size();
    }
}

void finalizeStatsCalculation(Stats & stats)
{
    for(auto & siteIter : stats.sites)
    {
        Site & site = siteIter.second;
        site.average           = Duration(site.average.count() / site.timeList.size());
        site.standardDeviation = Duration(sqrt((site.standardDeviation.count() / site.timeList.size()) - (site.average.count() * site.average.count())));
    }
}

#define DOUBLE_SIZE 21
void printStats(Stats & stats)
{
    cout << endl;
    cout << setw(stats.initialUrlMaxSize) << left  << "Initial URL" << " | "
         << setw(stats.finalUrlMaxSize  ) << left  << "Final URL"   << " | "
         << setw(DOUBLE_SIZE)             << right << "Average"     << " | "
         << setw(DOUBLE_SIZE)             << right << "Std Dev."    << " | "
         << endl;
    cout << setfill('-') << setw(stats.initialUrlMaxSize + stats.finalUrlMaxSize + 2 * DOUBLE_SIZE + 4 * 3) << '-' << setfill(' ') << endl;
    cout << fixed << setprecision(6);

    for(auto siteIter : stats.sites)
    {
        Site & site = siteIter.second;

        cout << setw(stats.initialUrlMaxSize) << left  << site.initialUrl                      << " | "
             << setw(stats.finalUrlMaxSize  ) << left  << site.finalUrl                        << " | "
             << setw(DOUBLE_SIZE)             << right << timeToString(site.average          ) << " | "
             << setw(DOUBLE_SIZE)             << right << timeToString(site.standardDeviation) << " | "
             << endl;
    }
}

void simpleSessions(int argc, char ** argv)
{
    StdOutputStream<ofstream> output("/dev/null");
    Stats                     stats;

    stats.nbRun             = atoi(argv[1]);
    stats.initialUrlMaxSize = 0;
    stats.finalUrlMaxSize   = 0;

    cout << "###########################################" << endl;
    cout << "# Easy interface"                            << endl;
    cout << "###########################################" << endl;

    for(int i=2 ; i<argc; i++)
    {
        cout << "Running \"" << argv[i] << '\"' << std::endl;

        HttpSession session;
        session.setOutputStream(&output);
        session.setFollowLocation(true);
        session.setUrl(argv[i]);

        for(int j=1; j<=stats.nbRun; j++)
        {
            session.perform();
            addSite(stats, argv[i], session.getEffectiveUrl(), session.getTotalTime());

            if((j % ITER_MODULO) == 0)
                cout << j << ' ' << flush;
        }
        cout << endl;
    }

    finalizeStatsCalculation(stats);
    printStats(stats);
    cout << endl;
}

void multipleSessionsURLs(int argc, char ** argv)
{
    StdOutputStream<ofstream> output("/dev/null");
    Stats                     stats;

    stats.nbRun             = atoi(argv[1]);
    stats.initialUrlMaxSize = 0;
    stats.finalUrlMaxSize   = 0;

    cout << "###########################################" << endl;
    cout << "# Multi interface (Parallel URL)"            << endl;
    cout << "###########################################" << endl;

    
    MultiSession        multiSession;
    vector<HttpSession> sessions(argc - 2);
    for(int i=2 ; i<argc; i++)
    {
        sessions[i - 2].setOutputStream(&output);
        sessions[i - 2].setFollowLocation(true);
        sessions[i - 2].setUrl(argv[i]);
    }

    for(int j=1; j<=stats.nbRun; j++)
    {
        for(int i=2 ; i<argc; i++)
            multiSession += sessions[i - 2];

        fd_set readSet;
        fd_set writeSet;
        fd_set excepSet;
        int    maxFd = -1;
        int    nbReady;
        struct timeval timeout;
        struct timeval oldTimeout;

        do
        {
            FD_ZERO(&readSet);
            FD_ZERO(&writeSet);
            FD_ZERO(&excepSet);
            timeout = multiSession.getTimeout();
            oldTimeout = timeout;

            multiSession.getFileDescriptorsSet(&maxFd, &readSet, &writeSet, &excepSet);
            if(maxFd == -1)
            {
                usleep(100000);
                nbReady = -2;
            }
            else
                nbReady = select(maxFd + 1, &readSet, &writeSet, &excepSet, &timeout);
        
            switch(nbReady)
            {
                case -1  :
                    cerr << "Error on select() call : " << strerror(errno) << " (" << errno << ") [maxFd = " << maxFd << " " << timeout.tv_sec << "s " << setw(6) << timeout.tv_usec << "us]" << endl;
                    return;
                    break;
                case 0  :
                    /* No break */
                default :
                    multiSession.perform();
                    break;
            }
        } while(multiSession.getSessionsRunning() > 0);

        for(int i=2 ; i<argc; i++)
        {
            addSite(stats, argv[i], sessions[i - 2].getEffectiveUrl(), sessions[i - 2].getTotalTime());
            multiSession -= sessions[i - 2];
        }

        if((j % ITER_MODULO) == 0)
            cout << j << ' ' << flush;
    }
    cout << endl;

    finalizeStatsCalculation(stats);
    printStats(stats);
    cout << endl;
}

void multipleSessionsIterations(int argc, char ** argv)
{
    StdOutputStream<ofstream> output("/dev/null");
    Stats                     stats;

    stats.nbRun             = atoi(argv[1]);
    stats.initialUrlMaxSize = 0;
    stats.finalUrlMaxSize   = 0;

    cout << "###########################################" << endl;
    cout << "# Multi interface (Parallel iterations)"     << endl;
    cout << "###########################################" << endl;

    
    MultiSession multiSession;
    for(int i=2 ; i<argc; i++)
    {
        cout << "Running \"" << argv[i] << '\"' << std::endl;

        vector<HttpSession> sessions(stats.nbRun);
        for(int j=1; j<=stats.nbRun; j++)
        {
            sessions[j - 1].setOutputStream(&output);
            sessions[j - 1].setFollowLocation(true);
            sessions[j - 1].setUrl(argv[i]);

            multiSession += sessions[j - 1];
        }

        fd_set readSet;
        fd_set writeSet;
        fd_set excepSet;
        int    maxFd = -1;
        int    nbReady;
        struct timeval timeout;
        struct timeval oldTimeout;

        do
        {
            FD_ZERO(&readSet);
            FD_ZERO(&writeSet);
            FD_ZERO(&excepSet);
            timeout = multiSession.getTimeout();
            oldTimeout = timeout;

            multiSession.getFileDescriptorsSet(&maxFd, &readSet, &writeSet, &excepSet);
            if(maxFd == -1)
            {
                usleep(100000);
                nbReady = -2;
            }
            else
                nbReady = select(maxFd + 1, &readSet, &writeSet, &excepSet, &timeout);
        
            switch(nbReady)
            {
                case -1  :
                    cerr << "Error on select() call : " << strerror(errno) << " (" << errno << ") [maxFd = " << maxFd << " " << timeout.tv_sec << "s " << setw(6) << timeout.tv_usec << "us]" << endl;
                    return;
                    break;
                case 0  :
                    /* No break */
                default :
                    multiSession.perform();
                    break;
            }
        } while(multiSession.getSessionsRunning() > 0);

        for(int j=1; j<=stats.nbRun; j++)
        {
            addSite(stats, argv[i], sessions[j - 1].getEffectiveUrl(), sessions[j - 1].getTotalTime());
            multiSession -= sessions[j - 1];
        }
    }

    finalizeStatsCalculation(stats);
    printStats(stats);
    cout << endl;
}

int main(int argc, char ** argv)
{
    simpleSessions            (argc, argv);
    multipleSessionsURLs      (argc, argv);
    multipleSessionsIterations(argc, argv);

    return 0;
}
