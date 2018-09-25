#include <fstream>
#include <ctime>
#include "Log.h"

using namespace std;

void Log::LogMessage(string message) {
    time_t nowSec = time(nullptr);
    char formattedTime[500];
    strftime(formattedTime, TIMESTAMP_SIZE, TIMESTAMP_FORMAT, localtime(&nowSec));
    cout << formattedTime << message << endl;
}

void Log::LogError(string message) {
    LogMessage("ERROR: " + message);
}
