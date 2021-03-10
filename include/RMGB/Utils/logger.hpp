#ifndef RMGB_UTILS_H
#define RMGB_UTILS_H

namespace logger {
    inline bool verbose = false;

    enum type {
        VERBOSE,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    void log(type logType, const char* logMsg, const char* area);
    void log(type logType, const char* logMsg, const char* area, const char* file, int line);
}

#endif