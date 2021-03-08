#ifndef EYESPARK_STRING_H
#define EYESPARK_STRING_H

class String {
    friend String operator+(const char *lhs, const String &rhs);
private:
    int size;
    char *buffer;
public:
    String() {
        size = 0;
        buffer = new char[size+1];
        buffer[0] = '\0';
    }
    String(const String &str) {
        size = str.size;
        buffer = new char[size+1];
        strcpy(buffer, str.buffer);
    }
    String(const char *str) {
        size = strlen(str);
        buffer = new char[size+1];
        strcpy(buffer, str);
    }

    inline String operator+(const String &rhs) const {
        char buf[size+rhs.size+1];
        strcpy(buf, buffer);
        strcat(buf, rhs.buffer);
        return String(buf);
    }
    inline String operator+(const char *rhs) const {
        char buf[size+strlen(rhs)+1];
        strcpy(buf, buffer);
        strcat(buf, rhs);
        return String(buf);
    }
    inline String &operator+=(const String &rhs) {
        size += rhs.size;
        char *buf = new char[size+1];
        strcpy(buf, buffer);
        strcat(buf, rhs.buffer);
        delete[] buffer;
        buffer = buf;
        return *this;
    }
    inline String &operator+=(const char *rhs) {
        size += strlen(rhs);
        char *buf = new char[size+1];
        strcpy(buf, buffer);
        strcat(buf, rhs);
        delete[] buffer;
        buffer = buf;
        return *this;
    }
    inline String &operator=(const String &rhs) {
        size = rhs.size;
        delete[] buffer;
        buffer = new char[size+1];
        strcpy(buffer, rhs.buffer);
        return *this;
    }
    inline String &operator=(const char *rhs) {
        size = strlen(rhs);
        delete[] buffer;
        buffer = new char[size+1];
        strcpy(buffer, rhs);
        return *this;
    }
    inline bool operator==(const String &rhs) const {
        return !(bool)strcmp(buffer, rhs.buffer);
    }
    inline bool operator==(const char *rhs) const {
        return !(bool)strcmp(buffer, rhs);
    }
    inline bool operator!=(const String &rhs) const {
        return strcmp(buffer, rhs.buffer);
    }
    inline bool operator!=(const char *rhs) const {
        return strcmp(buffer, rhs);
    }
    inline char operator[](int index) const {
        return buffer[index];
    }
    inline char &operator[](int index) {
        return buffer[index];
    }

    const char *data() {
        return buffer;
    }
    int length() const {
        return size;
    }
    String substr(int index, int length) {
        char buf[length+1];
        memcpy(buf, buffer+index, length-index > size ? size : length);
        buf[length] = '\0';
        return String(buf);
    }
    String substr(int index) {
        return substr(index, size-index);
    }
    int find(const String &str) {
        int queryPos = 0;
        int strPos = 0;
        for(strPos=0; strPos < size-str.size; strPos++) {
            if(buffer[strPos] == str[queryPos]) {
                if(++queryPos == str.size)return strPos-str.size+1;
            } else {
                strPos -= queryPos;
                queryPos = 0;
            }
        }
        return -1;
    }
    int find(const char *str) {
        int queryLength = strlen(str);
        int queryPos = 0;
        int strPos = 0;
        for(strPos=0; strPos < size-queryLength; strPos++) {
            if(buffer[strPos] == str[queryPos]) {
                if(++queryPos == queryLength)return strPos-queryLength+1;
            } else {
                strPos -= queryPos;
                queryPos = 0;
            }
        }
        return -1;
    }
    String toLowerCase() {
        char buf[size+1];
        for(int i=0; i<size; i++) {
            buf[i] = tolower(buffer[i]);
        }
        buf[size] = '\0';
        return String(buf);
    }
    String toUpperCase() {
        char buf[size+1];
        for(int i=0; i<size; i++) {
            buf[i] = toupper(buffer[i]);
        }
        buf[size] = '\0';
        return String(buf);
    }

    static String toString(int value, int base) {
        char buf[33];
        itoa(value, buf, base);
        return String(buf);
    }
    static String toString(int value) {
        char buf[11];
        itoa(value, buf, 10);
        return String(buf);
    }
    static String toString(float value) {
        char buf[16];
        snprintf(buf, 16, "%.11g", value);
        return String(buf);
    }
    static String toString(double value) {
        char buf[25];
        snprintf(buf, 25, "%.20g", value);
        return String(buf);
    }
    static String toString(bool value) {
        char buf[6];
        snprintf(buf, 6, value ? "true" : "false");
        return String(buf);
    }

    ~String() {
        delete[] buffer;
    }
};

inline String operator+(const char *lhs, const String &rhs) {
    char buf[strlen(lhs)+rhs.size+1];
    strcpy(buf, lhs);
    strcat(buf, rhs.buffer);
    return String(buf);
}

#endif // EYESPARK_STRING_H
