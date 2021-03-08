#ifndef EYESPARK_WINAPI_H
#define EYESPARK_WINAPI_H

#include "../util/String.h"

class WinAPI {
public:
    static void error(String message) {
        MessageBox(nullptr, message.data(), "Error", MB_ICONERROR | MB_OK);
    }
    static void exit(int code) {
        ExitProcess(code);
    }
    static String fullPath(String path) {
        char buffer[4096];
        GetFullPathName(path.data(), 4096, buffer, nullptr);
        return String(buffer);
    }
};

#endif // EYESPARK_WINAPI_H
