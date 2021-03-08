#ifndef EYESPARK_VECTOR4_H
#define EYESPARK_VECTOR4_H

#include "../util/Math.h"

class Vector4 {
    friend Vector4 operator*(const float lhs, const Vector4 rhs);
public:
    float x, y, z ,w;

    Vector4() {}
    Vector4(float x) {
        this->x = y = z = w = x;
    }
    Vector4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    inline Vector4 operator+(const Vector4 &rhs) const {
        return Vector4(x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w);
    }
    inline Vector4 &operator+=(const Vector4 &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    inline Vector4 operator-() const {
        return Vector4(-x, -y, -z, -w);
    }
    inline Vector4 operator-(const Vector4 &rhs) const {
        return Vector4(x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w);
    }
    inline Vector4 &operator-=(const Vector4 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }
    inline Vector4 operator*(const float rhs) const {
        return Vector4(x*rhs, y*rhs, z*rhs, w*rhs);
    }
    inline Vector4 &operator*=(const float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }
    inline Vector4 operator/(const float rhs) const {
        return Vector4(x/rhs, y/rhs, z/rhs, w/rhs);
    }
    inline Vector4 &operator/=(const float rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }
    inline bool operator==(const Vector4 &rhs) const {
        return x==rhs.x && y==rhs.y && z==rhs.z && w==rhs.w;
    }
    inline bool operator!=(const Vector4 &rhs) const {
        return x!=rhs.x || y!=rhs.y || z!=rhs.z || w!=rhs.w;
    }
    inline float operator[](int rhs) const {
        switch(rhs) {
        default:
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
    }
    inline float &operator[](int rhs) {
        switch(rhs) {
        default:
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
    }

    float length() {
        return Math::sqrt(x*x + y*y + z*z + w*w);
	}
    Vector4 normalize() {
		return *this/length();
	}
    float dot(const Vector4 &v) {
		return x*v.x + y*v.y + z*v.z + w*v.w;
	}
};

inline Vector4 operator*(const float lhs, const Vector4 rhs) {
    return Vector4(rhs.x*lhs, rhs.y*lhs, rhs.z*lhs, rhs.w*lhs);
}

#endif // EYESPARK_VECTOR4_H
