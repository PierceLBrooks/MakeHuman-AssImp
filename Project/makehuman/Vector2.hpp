#ifndef MH_VECTOR_TWO_HPP
#define MH_VECTOR_TWO_HPP

namespace mh
{
    template <typename T>
    class Vector2;
    
    typedef Vector2<int> Vector2i;
    typedef Vector2<unsigned int> Vector2u;
    typedef Vector2<float> Vector2f;
    typedef Vector2<double> Vector2d;
    
    template <typename T>
    class Vector2
    {
        public:
            T x;
            T y;
            
            Vector2() : x(), y() {}
            Vector2(T x, T y) : x(x), y(y) {}
            Vector2(const Vector2& other) : x(other.x), y(other.y) {}
            template <typename U>
            explicit Vector2(const Vector2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}
    };
    
    template <typename T>
    inline Vector2<T> operator -(const Vector2<T>& unary)
    {
        return Vector2<T>(-unary.x,-unary.y);
    }
    
    template <typename T>
    inline Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
    {
        left.x += right.x;
        left.y += right.y;
        return left;
    }
    
    template <typename T>
    inline Vector2<T>& operator +=(Vector2<T>& left, T right)
    {
        left.x += right;
        left.y += right;
        return left;
    }
    
    template <typename T>
    inline Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
    {
        return Vector2<T>(left.x+right.x,left.y+right.y);
    }
    
    template <typename T>
    inline Vector2<T> operator +(T left, const Vector2<T>& right)
    {
        return Vector2<T>(left+right.x,left+right.y);
    }
    
    template <typename T>
    inline Vector2<T> operator +(const Vector2<T>& left, T right)
    {
        return Vector2<T>(left.x+right,left.y+right);
    }
    
    template <typename T>
    inline Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
    {
        left.x -= right.x;
        left.y -= right.y;
        return left;
    }
    
    template <typename T>
    inline Vector2<T>& operator -=(Vector2<T>& left, T right)
    {
        left.x -= right;
        left.y -= right;
        return left;
    }
    
    template <typename T>
    inline Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
    {
        return Vector2<T>(left.x-right.x,left.y-right.y);
    }
    
    template <typename T>
    inline Vector2<T> operator -(T left, const Vector2<T>& right)
    {
        return Vector2<T>(left-right.x,left-right.y);
    }
    
    template <typename T>
    inline Vector2<T> operator -(const Vector2<T>& left, T right)
    {
        return Vector2<T>(left.x-right,left.y-right);
    }
    
    template <typename T>
    inline Vector2<T>& operator *=(Vector2<T>& left, const Vector2<T>& right)
    {
        left.x *= right.x;
        left.y *= right.y;
        return left;
    }
    
    template <typename T>
    inline Vector2<T>& operator *=(Vector2<T>& left, T right)
    {
        left.x *= right;
        left.y *= right;
        return left;
    }
    
    template <typename T>
    inline Vector2<T> operator *(const Vector2<T> left, const Vector2<T>& right)
    {
        return Vector2<T>(left.x*right.x,left.y*right.y);
    }
    
    template <typename T>
    inline Vector2<T> operator *(T left, const Vector2<T>& right)
    {
        return Vector2<T>(left*right.x,left*right.y);
    }
    
    template <typename T>
    inline Vector2<T> operator *(const Vector2<T>& left, T right)
    {
        return Vector2<T>(left.x*right,left.y*right);
    }
    
    template <typename T>
    inline Vector2<T>& operator /=(Vector2<T>& left, const Vector2<T>& right)
    {
        left.x /= right.x;
        left.y /= right.y;
        return left;
    }
    
    template <typename T>
    inline Vector2<T>& operator /=(Vector2<T>& left, T right)
    {
        left.x /= right;
        left.y /= right;
        return left;
    }
    
    template <typename T>
    inline Vector2<T> operator /(const Vector2<T>& left, const Vector2<T>& right)
    {
        return Vector2<T>(left.x/right.x,left.y/right.y);
    }
    
    template <typename T>
    inline Vector2<T> operator /(T left, const Vector2<T>& right)
    {
        return Vector2<T>(left/right.x,left/right.y);
    }
    
    template <typename T>
    inline Vector2<T> operator /(const Vector2<T>& left, T right)
    {
        return Vector2<T>(left.x/right,left.y/right);
    }
    
    template <typename T>
    inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
    {
        return ((left.x==right.x)&&(left.y==right.y));
    }
    
    template <typename T>
    inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
    {
        return ((left.x!=right.x)||(left.y!=right.y));
    }
}

#endif

