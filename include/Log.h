#include <iostream>

#ifndef GAMEIDJ_LOG_H
#define GAMEIDJ_LOG_H

using namespace std;

#define TIMESTAMP_FORMAT "%d/%m/%Y-%H:%M:%S "
#define TIMESTAMP_SIZE 100

class Log {
public:
    static void LogMessage(string message);
    static void LogError(string message);
};


#endif //GAMEIDJ_LOG_H
