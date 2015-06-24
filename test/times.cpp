#include <oowe/oowe.h>
#include <list>
#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace oowe;

struct Site
{
    string                  initialUrl;
    string                  finalUrl;
    list<Session::Duration> timeList;

    Session::Duration       average;
    Session::Duration       standardDeviation;
};

struct Stats
{
    map<const char *, Site> sites;
    int                     nbRun;

    int                     initialUrlMaxSize;
    int                     finalUrlMaxSize;
};

Stats stats;




#define PADDING 3

string timeToString(Session::Duration & val)
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

Session::Duration operator *(Session::Duration & lhs, Session::Duration & rhs)
{
    return Session::Duration(lhs.count() * rhs.count());
}

void addSite(const char * initialUrl, string & finalUrl, Session::Duration & time)
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

void finalizeStatsCalculation(void)
{
    for(auto & siteIter : stats.sites)
    {
        Site & site = siteIter.second;
        site.average           = Session::Duration(site.average.count() / site.timeList.size());
        site.standardDeviation = Session::Duration(sqrt((site.standardDeviation.count() / site.timeList.size()) - (site.average.count() * site.average.count())));
    }
}

#define DOUBLE_SIZE 21
void printStats(void)
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

int main(int argc, char ** argv)
{
    StdOutputStream<ofstream> output("/dev/null");
    HttpSession               session;

    string            finalUrl;
    Session::Duration time;

    session.setOutputStream(&output);
    session.setFollowLocation(true);

    stats.nbRun             = atoi(argv[1]);
    stats.initialUrlMaxSize = 0;
    stats.finalUrlMaxSize   = 0;

    for(int i=2 ; i<argc; i++)
    {
        std::cout << "Running \"" << argv[i] << '\"' << std::endl;

        session.setUrl(argv[i]);
        for(int j=1; j<=stats.nbRun; j++)
        {
            Site site;

            session.perform();

            finalUrl   = session.getEffectiveUrl();
            time       = session.getTotalTime();

            addSite(argv[i], finalUrl, time);

            if((j % 10) == 0)
                cout << j << ' ' << flush;
        }

        cout << endl;
    }

    finalizeStatsCalculation();
    printStats();

    return 0;
}
