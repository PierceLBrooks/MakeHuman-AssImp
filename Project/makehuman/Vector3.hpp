#ifndef MH_VECTOR_THREE_HPP
#define MH_VECTOR_THREE_HPP

#include "Vector2.hpp"

namespace mh
{
    template <typename T>
    class Vector3;
    
    typedef Vector3<int> Vector3i;
    typedef Vector3<unsigned int> Vector3u;
    typedef Vector3<float> Vector3f;
    typedef Vector3<double> Vector3d;
    
    template <typename T>
    class Vector3
    {
        public:
            T x;
            T y;
            T z;
            
            Vector3() : x(), y(), z() {}
            Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
            Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}
            Vector3(const Vector2<T>& other, T z) : x(other.x), y(other.y), z(z) {}
            Vector3(T x, const Vector2<T>& other) :
            x(x), y(other.x), z(other.y) {}
            template <typename U>
            explicit Vector3(const Vector2<U>& other, T z) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(z) {}
            template <typename U>
            explicit Vector3(T x, const Vector2<U>& other) :
            x(x), y(static_cast<T>(other.x)), z(static_cast<T>(other.y)) {}
            template <typename U>
            explicit Vector3(const Vector3<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)) {}
    };
    
    template <typename T>
    inline Vector3<T> operator -(const Vector3<T>& unary)
    {
        return Vector3<T>(-unary.x,-unary.y,-unary.z);
    }
    
    template <typename T>
    inline Vector3<T>& operator +=(Vector3<T>& left, const Vector3<T>& right)
    {
        left.x += right.x;
        left.y += right.y;
        left.z += right.z;
        return left;
    }
    
    template <typename T>
    inline Vector3<T>& operator +=(Vector3<T>& left, T right)
    {
        left.x += right;
        left.y += right;
        left.z += right;
        return left;
    }
    
    template <typename T>
    inline Vector3<T> operator +(const Vector3<T>& left, const Vector3<T>& right)
    {
        return Vector3<T>(left.x+right.x,left.y+right.y,left.z+right.z);
    }
    
    template <typename T>
    inline Vector3<T> operator +(T left, const Vector3<T>& right)
    {
        return Vector3<T>(left+right.x,left+right.y,left+right.z);
    }
    
    template <typename T>
    inline Vector3<T> operator +(const Vector3<T>& left, T right)
    {
        return Vector3<T>(left.x+right,left.y+right,left.z+right);
    }
    
    template <typename T>
    inline Vector3<T>& operator -=(Vector3<T>& left, const Vector3<T>& right)
    {
        left.x -= right.x;
        left.y -= right.y;
        left.z -= right.z;
        return left;
    }
    
    template <typename T>
    inline Vector3<T>& operator -=(Vector3<T>& left, T right)
    {
        left.x -= right;
        left.y -= right;
        left.z -= right;
        return left;
    }
    
    template <typename T>
    inline Vector3<T> operator -(const Vector3<T>& left, const Vector3<T>& right)
    {
        return Vector3<T>(left.x-right.x,left.y-right.y,left.z-right.z);
    }
    
    template <typename T>
    inline Vector3<T> operator -(T left, const Vector3<T>& right)
    {
        return Vector3<T>(left-right.x,left-right.y,left-right.z);
    }
    
    template <typename T>
    inline Vector3<T> operator -(const Vector3<T>& left, T right)
    {
        return Vector3<T>(left.x-right,left.y-right,left.z-right);
    }
    
    template <typename T>
    inline Vector3<T>& operator *=(Vector3<T>& left, const Vector3<T>& right)
    {
        left.x *= right.x;
        left.y *= right.y;
        left.z *= right.z;
        return left;
    }
    
    template <typename T>
    inline Vector3<T>& operator *=(Vector3<T>& left, T right)
    {
        left.x *= right;
        left.y *= right;
        left.z *= right;
        return left;
    }
    
    template <typename T>
    inline Vector3<T> operator *(const Vector3<T> left, const Vector3<T>& right)
    {
        return Vector3<T>(left.x*right.x,left.y*right.y,left.z*right.z);
    }
    
    template <typename T>
    inline Vector3<T> operator *(T left, const Vector3<T>& right)
    {
        return Vector3<T>(left*right.x,left*right.y,left*right.z);
    }
    
    template <typename T>
    inline Vector3<T> operator *(const Vector3<T>& left, T right)
    {
        return Vector3<T>(left.x*right,left.y*right,left.z*right);
    }
    
    template <typename T>
    inline Vector3<T>& operator /=(Vector3<T>& left, const Vector3<T>& right)
    {
        left.x /= right.x;
        left.y /= right.y;
        left.z /= right.z;
        return left;
    }
    
    template <typename T>
    inline Vector3<T>& operator /=(Vector3<T>& left, T right)
    {
        left.x /= right;
        left.y /= right;
        left.z /= right;
        return left;
    }
    
    template <typename T>
    inline Vector3<T> operator /(const Vector3<T>& left, const Vector3<T>& right)
    {
        return Vector3<T>(left.x/right.x,left.y/right.y,left.z/right.z);
    }
    
    template <typename T>
    inline Vector3<T> operator /(T left, const Vector3<T>& right)
    {
        return Vector3<T>(left/right.x,left/right.y,left/right.z);
    }
    
    template <typename T>
    inline Vector3<T> operator /(const Vector3<T>& left, T right)
    {
        return Vector3<T>(left.x/right,left.y/right,left.z/right);
    }
    
    template <typename T>
    inline bool operator ==(const Vector3<T>& left, const Vector3<T>& right)
    {
        return ((left.x==right.x)&&(left.y==right.y)&&(left.z==right.z));
    }
    
    template <typename T>
    inline bool operator !=(const Vector3<T>& left, const Vector3<T>& right)
    {
        return ((left.x!=right.x)||(left.y!=right.y)||(left.z!=right.z));
    }
}

#endif

