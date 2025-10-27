////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Angle.hpp> // NOLINT(misc-header-include-cycle)

#include <cassert>


namespace sf
{
namespace priv
{
inline constexpr float pi  = 3.141592654f;
inline constexpr float tau = pi * 2.f;

inline constexpr float positiveRemainder(float a, float b)
{
    assert(b > 0.f && "Cannot calculate remainder with non-positive divisor");
    const float val = a - static_cast<float>(static_cast<int>(a / b)) * b;
    return val >= 0.f ? val : val + b;
}
} // namespace priv

////////////////////////////////////////////////////////////
inline constexpr float Angle::asDegrees() const
{
    return m_radians * (180.f / priv::pi);
}


////////////////////////////////////////////////////////////
inline constexpr float Angle::asRadians() const
{
    return m_radians;
}


////////////////////////////////////////////////////////////
inline constexpr Angle Angle::wrapSigned() const
{
    return radians(priv::positiveRemainder(m_radians + priv::pi, priv::tau) - priv::pi);
}


////////////////////////////////////////////////////////////
inline constexpr Angle Angle::wrapUnsigned() const
{
    return radians(priv::positiveRemainder(m_radians, priv::tau));
}


////////////////////////////////////////////////////////////
inline constexpr Angle::Angle(float radians) : m_radians(radians)
{
}


////////////////////////////////////////////////////////////
inline constexpr Angle degrees(float angle)
{
    return Angle(angle * (priv::pi / 180.f));
}


////////////////////////////////////////////////////////////
inline constexpr Angle radians(float angle)
{
    return Angle(angle);
}


////////////////////////////////////////////////////////////
inline constexpr bool operator==(Angle left, Angle right)
{
    return left.asRadians() == right.asRadians();
}


////////////////////////////////////////////////////////////
inline constexpr bool operator!=(Angle left, Angle right)
{
    return left.asRadians() != right.asRadians();
}


////////////////////////////////////////////////////////////
inline constexpr bool operator<(Angle left, Angle right)
{
    return left.asRadians() < right.asRadians();
}


////////////////////////////////////////////////////////////
inline constexpr bool operator>(Angle left, Angle right)
{
    return left.asRadians() > right.asRadians();
}


////////////////////////////////////////////////////////////
inline constexpr bool operator<=(Angle left, Angle right)
{
    return left.asRadians() <= right.asRadians();
}


////////////////////////////////////////////////////////////
inline constexpr bool operator>=(Angle left, Angle right)
{
    return left.asRadians() >= right.asRadians();
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator-(Angle right)
{
    return radians(-right.asRadians());
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator+(Angle left, Angle right)
{
    return radians(left.asRadians() + right.asRadians());
}


////////////////////////////////////////////////////////////
inline constexpr Angle& operator+=(Angle& left, Angle right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator-(Angle left, Angle right)
{
    return radians(left.asRadians() - right.asRadians());
}


////////////////////////////////////////////////////////////
inline constexpr Angle& operator-=(Angle& left, Angle right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator*(Angle left, float right)
{
    return radians(left.asRadians() * right);
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator*(float left, Angle right)
{
    return right * left;
}


////////////////////////////////////////////////////////////
inline constexpr Angle& operator*=(Angle& left, float right)
{
    return left = left * right;
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator/(Angle left, float right)
{
    assert(right != 0.f && "Angle::operator/ cannot divide by 0");
    return radians(left.asRadians() / right);
}


////////////////////////////////////////////////////////////
inline constexpr Angle& operator/=(Angle& left, float right)
{
    assert(right != 0.f && "Angle::operator/= cannot divide by 0");
    return left = left / right;
}


////////////////////////////////////////////////////////////
inline constexpr float operator/(Angle left, Angle right)
{
    assert(right.asRadians() != 0.f && "Angle::operator/ cannot divide by 0");
    return left.asRadians() / right.asRadians();
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator%(Angle left, Angle right)
{
    assert(right.asRadians() != 0.f && "Angle::operator% cannot modulus by 0");
    return radians(priv::positiveRemainder(left.asRadians(), right.asRadians()));
}


////////////////////////////////////////////////////////////
inline constexpr Angle& operator%=(Angle& left, Angle right)
{
    assert(right.asRadians() != 0.f && "Angle::operator%= cannot modulus by 0");
    return left = left % right;
}

namespace Literals
{
inline constexpr Angle operator""_deg(long double angle)
{
    return degrees(static_cast<float>(angle));
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator""_deg(unsigned long long angle)
{
    return degrees(static_cast<float>(angle));
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator""_rad(long double angle)
{
    return radians(static_cast<float>(angle));
}


////////////////////////////////////////////////////////////
inline constexpr Angle operator""_rad(unsigned long long angle)
{
    return radians(static_cast<float>(angle));
}
} // namespace Literals


////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////

// Note: the 'inline' keyword here is technically not required, but VS2019 fails
// to compile with a bogus "multiple definition" error if not explicitly used.
inline constexpr Angle Angle::Zero;

} // namespace sf
