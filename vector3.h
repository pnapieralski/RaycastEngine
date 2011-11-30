/**
 * @author Phil Napieralski \ pnapieralski.com
 * @remarks Based off Vector class from CPSC805 renderer project
 */

#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3
{
public:
    Vector3();

    Vector3( const Vector3 &v );

    Vector3( const float a, const float b, const float c );

    //! Set all three components
    void set( const float a, const float b, const float c );

    //! Add two Vector3s
    const Vector3 operator+( const Vector3& v ) const;

    //! Subtract two Vector3s
    const Vector3 operator-( const Vector3& v ) const;

    //! Multiplcation of a vecotr with a constant
    const Vector3 operator*( const float v ) const;

    const Vector3 operator/( const float v ) const;

    const float dot( const Vector3& v ) const;

    const Vector3 cross( const Vector3& v) const;

    //! Read-write access to components by index
    float& operator[]( const int v );

    const float magnitude() const;

    const Vector3 unit() const;

    void normalize();

    float x,y,z;

};

#endif // VECTOR3_H
