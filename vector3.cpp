#include "Vector3.h"
#include <cmath>

Vector3::Vector3()
{
    x = y = z = 0;
}

Vector3::Vector3(const Vector3 &v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

Vector3::Vector3(const float a, const float b, const float c) {
    x = a;
    y = b;
    z = c;
}

void Vector3::set(const float a, const float b, const float c) {
    x = a;
    y = b;
    z = c;
}

const Vector3 Vector3::operator+(const Vector3 &v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

const Vector3 Vector3::operator-(const Vector3 &v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

const Vector3 Vector3::operator*(const float f) const {
    return Vector3(x*f, y*f, z*f);
}

const Vector3 Vector3::operator/(const float f) const {
    return Vector3(x/f, y/f, z/f);
}

const float Vector3::dot(const Vector3 &v) const {
    return x*v.x + y*v.y + z*v.z;
}

const Vector3 Vector3::cross(const Vector3 &v) const {
    return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y-y*v.x);
}

float& Vector3::operator[]( const int f ) {
    // Return x,y or z value
    return *((&x)+f);
}

const Vector3 Vector3::unit() const {
    return *this/magnitude();
}

void Vector3::normalize() {
    float mag = magnitude();
    x /= mag;
    y /= mag;
    z /= mag;
}

const float Vector3::magnitude() const {
    return sqrt( x*x + y*y + z*z );
}
