#ifndef EYESPARK_VECTOR2_H
#define EYESPARK_VECTOR2_H

#include "../util/Math.h"

class Vector2 {
    friend Vector2 operator*(const float lhs, const Vector2 rhs);
public:
    float x, y;

    Vector2() {}
    Vector2(float x) {
        this->x = y = x;
    }
    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    inline Vector2 operator+(const Vector2 &rhs) const {
        return Vector2(x+rhs.x, y+rhs.y);
    }
    inline Vector2 &operator+=(const Vector2 &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    inline Vector2 operator-() const {
        return Vector2(-x, -y);
    }
    inline Vector2 operator-(const Vector2 &rhs) const {
        return Vector2(x-rhs.x, y-rhs.y);
    }
    inline Vector2 &operator-=(const Vector2 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    inline Vector2 operator*(const float rhs) const {
        return Vector2(x*rhs, y*rhs);
    }
    inline Vector2 &operator*=(const float rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    inline Vector2 operator/(const float rhs) const {
        return Vector2(x/rhs, y/rhs);
    }
    inline Vector2 &operator/=(const float rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }
    inline bool operator==(const Vector2 &rhs) const {
        return x==rhs.x && y==rhs.y;
    }
    inline bool operator!=(const Vector2 &rhs) const {
        return x!=rhs.x || y!=rhs.y;
    }
    inline float operator[](int rhs) const {
        switch(rhs) {
        default:
        case 0: return x;
        case 1: return y;
        }
    }
    inline float &operator[](int rhs) {
        switch(rhs) {
        default:
        case 0: return x;
        case 1: return y;
        }
    }

    float length() {
        return Math::sqrt(x*x + y*y);
    }
    Vector2 normalize() {
        return *this/length();
    }
    float dot(const Vector2 &v) {
        return x*v.x + y*v.y;
    }
};

inline Vector2 operator*(const float lhs, const Vector2 rhs) {
    return Vector2(rhs.x*lhs, rhs.y*lhs);
}

#endif // EYESPARK_VECTOR2_H
