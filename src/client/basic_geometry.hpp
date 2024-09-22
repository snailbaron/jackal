#pragma once

#include <type_traits>
#include <algorithm>
#include <cmath>

/**
 * 2-dimensional vector.
 */
template <class T, class Tag>
struct Vector {
    static_assert(std::is_arithmetic<T>::value,
        "Vector can only hold arithmetic types");

    T x;
    T y;

    Vector() : x(0), y(0) {}
    Vector(T x, T y) : x(x), y(y) {}
    Vector(const Vector& other) : x(other.x), y(other.y) {}

    Vector& operator=(const Vector& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    // Arithmetic operators

    Vector& operator+=(const Vector& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector& operator-=(const Vector& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector& operator*=(const Vector& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vector& operator*=(T mul)
    {
        x *= mul;
        y *= mul;
        return *this;
    }

    Vector& operator/=(const Vector& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    Vector& operator/=(T mul)
    {
        x /= mul;
        y /= mul;
        return *this;
    }

    friend Vector operator+(Vector left, const Vector& right)
    {
        left += right;
        return left;
    }

    friend Vector operator-(Vector left, const Vector& right)
    {
        left -= right;
        return left;
    }

    friend Vector operator/(Vector vec, T mul)
    {
        vec /= mul;
        return vec;
    }

    // Relational operators

    friend bool operator==(const Vector& left, const Vector& right)
    {
        return left.x == right.x && left.y == right.y;
    }

    friend bool operator!=(const Vector& left, const Vector& right)
    {
        return !(left == right);
    }

    // Operators <, >, <=, >= are not used, to avoid ruining their standard
    // semantics (which imply strict weak ordering).

    bool leq(const Vector& other) const
    {
        return x <= other.x && y <= other.y;
    }

    bool strictLess(const Vector& other) const
    {
        return x < other.x && y < other.y;
    }

    bool weakLess(const Vector& other) const
    {
        return leq(other) && *this != other;
    }

    friend bool operator>(const Vector& left, const Vector& right)
    {
        return right < left;
    }

    // Various operations

    template <class Zoom>
    Vector zoomed(Zoom zoom) const
    {
        using U = std::common_type_t<T, Zoom>;
        return {
            static_cast<T>(static_cast<U>(x) * static_cast<U>(zoom)),
            static_cast<T>(static_cast<U>(y) * static_cast<U>(zoom))
        };
    }

    Vector loClamped(T lo) const
    {
        return Vector(std::max(x, lo), std::max(y, lo));
    }

    Vector hiClamped(T hi) const
    {
        return Vector(std::min(x, hi), std::min(y, hi));
    }

    Vector clamped(T lo, T hi) const
    {
        return Vector(
            std::min(std::max(x, lo), hi), std::min(std::max(y, lo), hi));
    }

    Vector rounded() const
    {
        return Vector(std::round(x), std::round(y));
    }
};

template <class T, class Mul, class Tag>
Vector<decltype(T{} *Mul{}), Tag> operator*(Vector<T, Tag> vec, Mul mul)
{
    return{ vec.x * mul, vec.y * mul };
}

template <class T, class Mul, class Tag>
Vector<decltype(T{} *Mul{}), Tag> operator*(Mul mul, Vector<T, Tag> vec)
{
    return vec * mul;
}

template <class Dst, class DstTag, class Src, class SrcTag>
Vector<Dst, DstTag> vectorCast(const Vector<Src, SrcTag>& vec)
{
    return{ static_cast<Dst>(vec.x), static_cast<Dst>(vec.y) };
}

template <class Dst, class Src, class SrcTag>
Vector<Dst, SrcTag> vectorCast(const Vector<Src, SrcTag>& vec)
{
    return { static_cast<Dst>(vec.x), static_cast<Dst>(vec.y) };
}

template <class T, class Tag>
inline bool strictLess(const Vector<T, Tag>& left, const Vector<T, Tag>& right)
{
    return left.x < right.x && left.y < right.y;
}

/**
 * Axis-aligned rectangle.
 */
template <class V, class Tag>
struct Rect {
    V origin;
    V size;

    template <class T>
    Rect(T x, T y, T w, T h) : origin(x, y), size(w, h) {}

    Rect() : origin(), size() {}
    Rect(const V& origin, const V& size) : origin(origin), size(size) {}
    Rect(const Rect& other) : origin(other.origin), size(other.size) {}

    Rect& operator=(const Rect& other)
    {
        origin = other.origin;
        size = other.size;
        return *this;
    }

    bool contains(const V& point) const
    {
        return origin.leq(point) && point.strictLess(origin + size);
    }

    Rect padded(const V& padding) const
    {
        return { origin + padding, size - 2 * padding };
    }

    template <class T>
    Rect padded(T padding) const
    {
        return padded(V(padding, padding));
    }

    Rect offset(const V& offset) const
    {
        return { origin + offset, size };
    }

    template <class Zoom>
    Rect zoomed(Zoom zoom) const
    {
        V newSize = size.zoomed(zoom);
        return { origin - (newSize - size) / 2, newSize };
    }
};
