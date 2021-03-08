#ifndef EYESPARK_SHADER_H
#define EYESPARK_SHADER_H

#include "../util/Error.h"
#include "../util/Logger.h"
#include "../util/Matrix4.h"
#include "../util/String.h"
#include "../util/Vector3.h"
#include "../util/Vector4.h"
#include "../util/WinAPI.h"

class Shader {
private:
    unsigned int program;

    unsigned int createShader(const char *code, unsigned int type, String path) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &code, nullptr);
        glCompileShader(shader);
        int status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE) {
            int size;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
            GLchar info[size];
            glGetShaderInfoLog(shader, size, nullptr, &info[0]);
            glDeleteShader(shader);
            String args[2] = { WinAPI::fullPath(path), info };
            Error::error(Error::SHADER_COMPILATION_ERROR, 2, args);
        }
        return shader;
    }
public:
    Shader(String path) {
        FILE *file = fopen(path.data(), "r");
        if(!file) {
            String args[] = { WinAPI::fullPath(path) };
            Error::error(Error::FILE_OPENING_ERROR, 1, args);
        }
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *code[2] = { new char[length+1], new char[length+1] };
        code[0][0] = '\0';
        code[1][0] = '\0';
        int target = -1;
        char line[length+1];
        while(fgets(line, length+1, file) != nullptr) {
            if(strcmp(line, "@vertex\n") == 0) {
                target = 0;
                strcat(code[0], "\n");
                strcat(code[1], "\n");
                continue;
            }
            if(strcmp(line, "@fragment\n") == 0) {
                target = 1;
                strcat(code[0], "\n");
                strcat(code[1], "\n");
                continue;
            }
            if(target != -1) {
                strcat(code[target], line);
                strcat(code[(target+1)%2], "\n");
            } else {
                strcat(code[0], "\n");
                strcat(code[1], "\n");
            }
        }

        unsigned int vertex = createShader(code[0], GL_VERTEX_SHADER, path);
        unsigned int fragment = createShader(code[1], GL_FRAGMENT_SHADER, path);

        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        int status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if(status == GL_FALSE) {
            int size;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
            GLchar info[size];
            glGetProgramInfoLog(program, size, nullptr, &info[0]);
            glDetachShader(program, vertex);
            glDetachShader(program, fragment);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(program);
            String args[2] = { WinAPI::fullPath(path), info };
            Error::error(Error::PROGRAM_LINKING_ERROR, 2, args);
        }
        glValidateProgram(program);
        glDetachShader(program, vertex);
        glDetachShader(program, fragment);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void bind() {
        glUseProgram(program);
    }
    int getUniformLocation(String name) {
        return glGetUniformLocation(program, name.data());
    }

    static void setUniform(int location, int value) {
        glUniform1i(location, value);
    }
    static void setUniform(int location, float value) {
        glUniform1f(location, value);
    }
    static void setUniform(int location, Vector3 value) {
        glUniform3f(location, value.x, value.y, value.z);
    }
    static void setUniform(int location, Vector4 value) {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
    static void setUniform(int location, Matrix4 value) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
    static void setUniformArray(int location, int size, int *values) {
        glUniform1iv(location, size, values);
    }
    static void setUniformArray(int location, int size, float *values) {
        glUniform1fv(location, size, values);
    }
    static void setUniformArray(int location, int size, Vector3 *values) {
        glUniform3fv(location, size, &values[0].x);
    }
    static void setUniformArray(int location, int size, Matrix4 *values) {
        glUniformMatrix4fv(location, size, GL_FALSE, &values[0][0].x);
    }

    ~Shader() {
        glDeleteProgram(program);
    }
};

#endif // EYESPARK_SHADER_H
