#include "Vector2.h"
#include <DirectXMath.h>


float Vector2::GetLength() const
{
	return sqrtf(x * x + y * y);
}

void Vector2::Normalized()
{
	*this /= GetLength();
}

Vector2 Vector2::GetNormalize()
{
	float len = GetLength();
	if (len > 0) return *this / len;

	return *this;
}

float Vector2::Cross(const Vector2& other)const
{
	return x * other.y - y * other.x;
}

float Vector2::Dot(const Vector2& other)const
{
	return x * other.x + y * other.y;
}



//-----------------------------------------------------------------
//演算子オーバーロード
Vector2 Vector2::operator-()
{
	return { -x,-y };
}

Vector2 Vector2::operator+()
{
	return *this;
}


Vector2 Vector2::operator*(const float& other)
{
	Vector2 v;
	v.x = x * other;
	v.y = y * other;

	return v;
}

Vector2& Vector2::operator*=(const float& other)
{
	*this = *this * other;//thisはアドレスなので*つける

	return *this;
}



Vector2 Vector2::operator/(const float& other)
{
	Vector2 v;
	v.x = x / other;
	v.y = y / other;

	return v;
}

Vector2& Vector2::operator/=(const float& other)
{
	*this = *this / other;//thisはアドレスなので*つける

	return *this;
}




const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator*(float s, const Vector2& v)
{
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator/(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp /= s;
}

const Vector2 lerp(const Vector2& start, const Vector2& end, const float t) {
	return start * (1.0f - t) + end * t;
}