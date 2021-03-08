#ifndef EYESPARK_MATRIX4_H
#define EYESPARK_MATRIX4_H

#include "../util/Math.h"
#include "../util/Vector3.h"
#include "../util/Vector4.h"

class Matrix4 {
private:
    Vector4 m[4];

    void identity(float x) {
        m[0][0]=x;m[0][1]=0;m[0][2]=0;m[0][3]=0;
        m[1][0]=0;m[1][1]=x;m[1][2]=0;m[1][3]=0;
        m[2][0]=0;m[2][1]=0;m[2][2]=x;m[2][3]=0;
        m[3][0]=0;m[3][1]=0;m[3][2]=0;m[3][3]=x;
    }
public:
    Matrix4() {}
    Matrix4(float x) {
        identity(x);
    }
    Matrix4(Vector4 v0, Vector4 v1, Vector4 v2, Vector4 v3) {
        m[0] = v0;
        m[1] = v1;
        m[2] = v2;
        m[3] = v3;
    }
    Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) {
        m[0][0]=m00;m[0][1]=m01;m[0][2]=m02;m[0][3]=m03;
        m[1][0]=m10;m[1][1]=m11;m[1][2]=m12;m[1][3]=m13;
        m[2][0]=m20;m[2][1]=m21;m[2][2]=m22;m[2][3]=m23;
        m[3][0]=m30;m[3][1]=m31;m[3][2]=m32;m[3][3]=m33;
    }

    inline Vector4 operator*(const Vector4 &rhs) const {
        return Vector4(m[0][0]*rhs.x + m[1][0]*rhs.y + m[2][0]*rhs.z + m[3][0]*rhs.w,
                m[0][1]*rhs.x + m[1][1]*rhs.y + m[2][1]*rhs.z + m[3][1]*rhs.w,
                m[0][2]*rhs.x + m[1][2]*rhs.y + m[2][2]*rhs.z + m[3][2]*rhs.w,
                m[0][3]*rhs.x + m[1][3]*rhs.y + m[2][3]*rhs.z + m[3][3]*rhs.w);
    }
    inline Matrix4 operator*(const Matrix4 &rhs) const {
		return Matrix4(m[0]*rhs[0][0] + m[1]*rhs[0][1] + m[2]*rhs[0][2] + m[3]*rhs[0][3],
                m[0]*rhs[1][0] + m[1]*rhs[1][1] + m[2]*rhs[1][2] + m[3]*rhs[1][3],
                m[0]*rhs[2][0] + m[1]*rhs[2][1] + m[2]*rhs[2][2] + m[3]*rhs[2][3],
                m[0]*rhs[3][0] + m[1]*rhs[3][1] + m[2]*rhs[3][2] + m[3]*rhs[3][3]);
    }
    inline Matrix4 &operator*=(const Matrix4 &rhs) {
        return *this = *this * rhs;
    }
    inline bool operator==(const Matrix4 &rhs) const {
        return m[0][0]==rhs[0][0] && m[0][1]==rhs[0][1] && m[0][2]==rhs[0][2] && m[0][3]==rhs[0][3] &&
                m[1][0]==rhs[1][0] && m[1][1]==rhs[1][1] && m[1][2]==rhs[1][2] && m[1][3]==rhs[1][3] &&
                m[2][0]==rhs[2][0] && m[2][1]==rhs[2][1] && m[2][2]==rhs[2][2] && m[2][3]==rhs[2][3] &&
                m[3][0]==rhs[3][0] && m[3][1]==rhs[3][1] && m[3][2]==rhs[3][2] && m[3][3]==rhs[3][3];
    }
    inline bool operator!=(const Matrix4 &rhs) const {
        return m[0][0]!=rhs[0][0] || m[0][1]!=rhs[0][1] || m[0][2]!=rhs[0][2] || m[0][3]!=rhs[0][3] ||
                m[1][0]!=rhs[1][0] || m[1][1]!=rhs[1][1] || m[1][2]!=rhs[1][2] || m[1][3]!=rhs[1][3] ||
                m[2][0]!=rhs[2][0] || m[2][1]!=rhs[2][1] || m[2][2]!=rhs[2][2] || m[2][3]!=rhs[2][3] ||
                m[3][0]!=rhs[3][0] || m[3][1]!=rhs[3][1] || m[3][2]!=rhs[3][2] || m[3][3]!=rhs[3][3];
    }
    inline Vector4 operator[](int rhs) const {
        return m[rhs];
    }
    inline Vector4 &operator[](int rhs) {
        return m[rhs];
    }

    Matrix4 inverse() {
        float s0 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
        float s1 = m[0][0] * m[2][1] - m[0][1] * m[2][0];
        float s2 = m[0][0] * m[3][1] - m[0][1] * m[3][0];
        float s3 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
        float s4 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
        float s5 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
        float c5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
        float c4 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
        float c3 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
        float c2 = m[0][2] * m[3][3] - m[0][3] * m[3][2];
        float c1 = m[0][2] * m[2][3] - m[0][3] * m[2][2];
        float c0 = m[0][2] * m[1][3] - m[0][3] * m[1][2];

        float invdet = 1 / (s0*c5 - s1*c4 + s2*c3 + s3*c2 - s4*c1 + s5*c0);

        return Matrix4(( m[1][1]*c5 - m[2][1]*c4 + m[3][1]*c3) * invdet,
                (-m[0][1]*c5 + m[2][1]*c2 - m[3][1]*c1) * invdet,
                ( m[0][1]*c4 - m[1][1]*c2 + m[3][1]*c0) * invdet,
                (-m[0][1]*c3 + m[1][1]*c1 - m[2][1]*c0) * invdet,
                (-m[1][0]*c5 + m[2][0]*c4 - m[3][0]*c3) * invdet,
                ( m[0][0]*c5 - m[2][0]*c2 + m[3][0]*c1) * invdet,
                (-m[0][0]*c4 + m[1][0]*c2 - m[3][0]*c0) * invdet,
                ( m[0][0]*c3 - m[1][0]*c1 + m[2][0]*c0) * invdet,
                ( m[1][3]*s5 - m[2][3]*s4 + m[3][3]*s3) * invdet,
                (-m[0][3]*s5 + m[2][3]*s2 - m[3][3]*s1) * invdet,
                ( m[0][3]*s4 - m[1][3]*s2 + m[3][3]*s0) * invdet,
                (-m[0][3]*s3 + m[1][3]*s1 - m[2][3]*s0) * invdet,
                (-m[1][2]*s5 + m[2][2]*s4 - m[3][2]*s3) * invdet,
                ( m[0][2]*s5 - m[2][2]*s2 + m[3][2]*s1) * invdet,
                (-m[0][2]*s4 + m[1][2]*s2 - m[3][2]*s0) * invdet,
                ( m[0][2]*s3 - m[1][2]*s1 + m[2][2]*s0) * invdet);
    }

    static Matrix4 transform(Vector3 translation, Vector3 rotation, Vector3 scale) {
		float cx = Math::cos(Math::radians(rotation.x));
		float sx = Math::sin(Math::radians(rotation.x));
		float cy = Math::cos(Math::radians(rotation.y));
		float sy = Math::sin(Math::radians(rotation.y));
		float cz = Math::cos(Math::radians(rotation.z));
		float sz = Math::sin(Math::radians(rotation.z));

        return Matrix4((cy*cz+sy*sx*sz) * scale.x,
                (cx*sz) * scale.x,
                (cy*sx*sz-cz*sy) * scale.x,
                0,
                (cz*sy*sx-cy*sz) * scale.y,
                (cx*cz) * scale.y,
                (cy*cz*sx+sy*sz) * scale.y,
                0,
                (cx*sy) * scale.z,
                (-sx) * scale.z,
                (cy*cx) * scale.z,
                0,
                translation.x,
                translation.y,
                translation.z,
                1);
	}
	static Matrix4 perspective(float fov, float aspect, float nearClip, float farClip) {
        float ht = Math::tan(Math::radians(fov)/2);

        Matrix4 result(0);
	    result[0][0] = 1/(aspect*ht);
        result[1][1] = 1/ht;
        result[2][2] = -(nearClip+farClip)/(farClip-nearClip);
        result[2][3] = -1;
        result[3][2] = (2*nearClip*farClip)/(nearClip-farClip);

        return result;
	}
	static Matrix4 ortho(float width, float height, float nearClip, float farClip) {
	    Matrix4 result(1);
        result[0][0] = 2/width;
		result[1][1] = 2/height;
		result[2][2] = -2/(farClip-nearClip);
		result[3][2] = -(farClip+nearClip)/(farClip-nearClip);

		return result;
	}
	static Matrix4 ortho(float width, float height) {
	    Matrix4 result(1);
        result[0][0] = 2/width;
		result[1][1] = 2/height;

		return result;
	}
};

#endif // EYESPARK_MATRIX4_H
