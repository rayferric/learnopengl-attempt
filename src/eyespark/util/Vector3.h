#ifndef EYESPARK_VECTOR3_H
#define EYESPARK_VECTOR3_H

#include "../util/Math.h"
#include "../util/Vector4.h"

class Vector3 {
    friend Vector3 operator*(const float lhs, const Vector3 rhs);
public:
    float x, y, z;

    Vector3() {}
    Vector3(float x) {
        this->x = y = z = x;
    }
    Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3(const Vector4 &v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    inline Vector3 operator+(const Vector3 &rhs) const {
        return Vector3(x+rhs.x, y+rhs.y, z+rhs.z);
    }
    inline Vector3 &operator+=(const Vector3 &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    inline Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }
    inline Vector3 operator-(const Vector3 &rhs) const {
        return Vector3(x-rhs.x, y-rhs.y, z-rhs.z);
    }
    inline Vector3 &operator-=(const Vector3 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    inline Vector3 operator*(const float rhs) const {
        return Vector3(x*rhs, y*rhs, z*rhs);
    }
    inline Vector3 &operator*=(const float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    inline Vector3 operator/(const float rhs) const {
        return Vector3(x/rhs, y/rhs, z/rhs);
    }
    inline Vector3 &operator/=(const float rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }
    inline bool operator==(const Vector3 &rhs) const {
        return x==rhs.x && y==rhs.y && z==rhs.z;
    }
    inline bool operator!=(const Vector3 &rhs) const {
        return x!=rhs.x || y!=rhs.y || z!=rhs.z;
    }
    inline float operator[](int rhs) const {
        switch(rhs) {
        default:
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
    }
    inline float &operator[](int rhs) {
        switch(rhs) {
        default:
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
    }

    float length() {
        return Math::sqrt(x*x + y*y + z*z);
	}
    Vector3 normalize() {
		return *this/length();
	}
    float dot(const Vector3 &v) {
		return x*v.x + y*v.y + z*v.z;
	}
};

inline Vector3 operator*(const float lhs, const Vector3 rhs) {
    return Vector3(rhs.x*lhs, rhs.y*lhs, rhs.z*lhs);
}

#endif // EYESPARK_VECTOR3_H
