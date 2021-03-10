#include <Utils/logger.hpp>

#include <cstdio>
#include <cstdlib>

void logger::log(type logType, const char *logMsg, const char* area, const char *file="", int line=0) {
    switch (logType) {
        case VERBOSE:
            if (verbose) {
                printf("[VERBOSE: %s] %s\n",area,logMsg);
            }
            break;
        case INFO:
            printf("[INFO: %s] %s\n",area,logMsg);
            break;
        case WARN:
            printf("[WARN: %s] %s\n",area,logMsg);
            break;
        case ERROR:
            printf("[ERROR: %s] %s at %s:%i\n",area,logMsg,file,line);
            break;
        case FATAL:
            printf("[FATAL ERROR: %s] %s at %s:%i\n",area,logMsg,file,line);
            exit(1);
            break;
    }
}

void logger::log(type logType, const char* logMsg, const char* area) {
    log(logType, logMsg, area, "", 0);
}