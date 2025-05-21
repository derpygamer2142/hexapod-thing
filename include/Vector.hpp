#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

const double PI = 3.14159265358;

class vec3 {
  public:
    double e[3];

    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    vec3(double v) : e{v, v, v} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
};


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline double length(const vec3& v) {
    return v.length();
}

inline vec3 normalize(const vec3& v) {
    return v / v.length();
}

// only rotates the x and z around the y axis
// left handed y up superiority
inline vec3 rotate(const vec3& v, double theta) {
    double cosTheta = cos(theta * (PI/180.0));
    double sinTheta = sin(theta * (PI/180.0));
    return vec3(v.x()*cosTheta - v.y()*sinTheta, v.x()*sinTheta + v.y()*cosTheta, v.z());
}

// rotate point v around point j on the xz plane
inline vec3 rotate(const vec3& v, double theta, const vec3& j) {
    return rotate(v - j, theta);
}

// rotate point v around the origin using all 3 sigmas axes
// yaw is rotation around the y axis
// pitch is rotation around the x axis
inline vec3 rotate(const vec3& v, double x, double y, double z) {
    // generate a 3x3 rotation matrix
    float sx = sin(x);
    float sy = sin(y);
    float sz = sin(z);
    float cx = cos(x);
    float cy = cos(y);
    float cz = cos(z);
    float m1, m2, m3, m4, m5, m6, m7, m8, m9;

    m1 = cx*cz - (sx*sy*sz);
    m2 = -sz*cy;
    m3 = sy*cx*sz + sx*cz;
    m4 = sy*sx*cz + cx*sz;
    m5 = cy*cz;
    m6 = sx*sz - sy*cx*cz;
    m7 = -sy*cy;
    m8 = sy;
    m9 = cy * cx;

    return vec3(
        m1*v.x() + m2*v.y() + m3*v.z(),
        m4*v.x() + m5*v.y() + m6*v.z(),
        m7*v.x() + m8*v.y() + m9*v.z()
    );
}

inline vec3 rotate(const vec3& v, double x, double y, double z, const vec3& j) {
    return rotate(v - j, x, y, z);
}

#endif