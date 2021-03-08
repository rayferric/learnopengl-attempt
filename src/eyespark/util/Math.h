#ifndef EYESPARK_MATH_H
#define EYESPARK_MATH_H

#define EYESPARK_MATH_SQRT2 1.4142135623730950488016887242
#define EYESPARK_MATH_PI 3.1415926535897932384626433833
#define EYESPARK_MATH_DEG2RAD 0.0174532925199432957692369077
#define EYESPARK_MATH_RAD2DEG 57.295779513082320876798154814

class Math {
public:
    inline static double sqrt(double x) { return ::sqrt(x); }
    inline static double sin(double x) { return ::sin(x); }
    inline static double cos(double x) { return ::cos(x); }
    inline static double tan(double x) { return ::tan(x); }
    inline static double pow(double x, double exp) { return ::pow(x, exp); }
    inline static double clamp(double x, double min, double max) { return (x < min ? min : x) > max ? max : x; }
    inline static double radians(double x) { return x*EYESPARK_MATH_DEG2RAD; }
};

#endif // EYESPARK_MATH_H
