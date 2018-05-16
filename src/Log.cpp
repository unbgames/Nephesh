#include <fstream>
#include <ctime>
#include "Log.h"

using namespace std;

void Log::LogMessage(string message) {
    ofstream file;
    time_t nowSec = time(nullptr);
    char formattedTime[500];
    strftime(formattedTime, TIMESTAMP_SIZE, TIMESTAMP_FORMAT, localtime(&nowSec));
    file.open("log.txt", ios::app);
    file << formattedTime << message << endl;
    file.close();
}

void Log::LogError(string message) {
    LogMessage("ERROR: " + message);
}