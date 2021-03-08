#ifndef EYESPARK_LOGGER_H
#define EYESPARK_LOGGER_H

#include "../util/String.h"

class Logger {
private:
    FILE *file;

    Logger() {
        file = fopen("latest.log", "w");
        file = freopen("latest.log", "a+", file);
    }

    void log(const char *type, const char *message) {
        SYSTEMTIME time;
        GetLocalTime(&time);
        fprintf(file, "[%0*d:%0*d:%0*d] [%s]: %s\n", 2, time.wHour, 2, time.wMinute, 2, time.wSecond, type, message);
        fflush(file);

        if(print)printf("[%s]: %s\n", type, message);
    }
public:
    bool print = true;

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger) = delete;

    void info(String message) { log("INFO", message.data()); }
    void warning(String message) { log("WARNING", message.data()); }
    void error(String message) { log("ERROR", message.data()); }

    static Logger &getInstance() {
        static Logger instance;
        return instance;
    }

    ~Logger() {
        fclose(file);
    }
};

#endif // EYESPARK_LOGGER_H
