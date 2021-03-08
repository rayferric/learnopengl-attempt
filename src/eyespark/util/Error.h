#ifndef EYESPARK_ERROR_H
#define EYESPARK_ERROR_H

#include "../util/Logger.h"
#include "../util/String.h"
#include "../util/WinAPI.h"

class Error {
private:
    static void (*callback)(int, int, String *);
public:
    static const int WINDOW_CREATION_ERROR = 1;
    static const int CONTEXT_CREATION_ERROR = 2;
    static const int EXTENSIONS_LOADING_ERROR = 3;
    static const int FILE_OPENING_ERROR = 4;
    static const int SHADER_COMPILATION_ERROR = 5;
    static const int PROGRAM_LINKING_ERROR = 6;
    static const int FRAMEBUFFER_CREATION_ERROR = 7;

    static void setCallback(void (*func)(int, int, String *)) {
        callback = func;
    };
    static void parse(int code, int size, String *info) {
        String message;
        switch(code) {
        case WINDOW_CREATION_ERROR:
            message = "Failed to create window instance.\n\nOpenGL 3.3 support is required.";
            break;
        case CONTEXT_CREATION_ERROR:
            message = "Failed to create OpenGL context.\n\nOpenGL 3.3 support is required.";
            break;
        case EXTENSIONS_LOADING_ERROR:
            message = "Failed to load OpenGL extensions.\n\nOpenGL 3.3 support is required.";
            break;
        case FILE_OPENING_ERROR:
            message = "Failed to open file.\n" + info[0] + "\n";
            break;
        case SHADER_COMPILATION_ERROR:
            message = "Failed to compile shader.\n" + info[0] + "\n\n" + info[1] + "OpenGL 3.3 support is required.";
            break;
        case PROGRAM_LINKING_ERROR:
            message = "Failed to link shader program.\n" + info[0] + "\n\n" + info[2] + "\n\nOpenGL 3.3 support is required.";
            break;
        case FRAMEBUFFER_CREATION_ERROR:
            message = "Failed to create framebuffer.\n\nOpenGL 3.3 support is required.";
        default:
            message = info[0];
            for(int i=1; i<size; i++)message += "\n" + info[i];
            break;
        }
        message += "\nThe program will now terminate. (0x" + String::toString(code, 16).toUpperCase() + ")";
        Logger::getInstance().error(message);
        WinAPI::error(message);
        WinAPI::exit(code);
    }
    static void error(int code, int size, String *info) {
        callback(code, size, info);
    }
};

void (*Error::callback)(int, int, String *) = Error::parse;

#endif // EYESPARK_ERROR_H
