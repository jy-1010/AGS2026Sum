#include "Vector.h"

IntVector3 IntVector3::operator+(const IntVector3& _value)const
{
	return { x + _value.x , y + _value.y, z + _value.z };
}

void IntVector3::operator+=(const IntVector3& _value)
{
	x += _value.x;
	y += _value.y;
	z += _value.z;
}

IntVector3 IntVector3::operator-(const IntVector3& _value)const
{
	return { x - _value.x , y - _value.y, z - _value.z };
}

void IntVector3::operator-=(const IntVector3& _value)
{
	x -= _value.x;
	y -= _value.y;
	z -= _value.z;
}

IntVector3 IntVector3::operator*(const int _value)const
{
	return { x * _value , y * _value, z * _value };
}

void IntVector3::operator*=(const int _value)
{
	x *= _value;
	y *= _value;
	z *= _value;
}

IntVector3 IntVector3::operator/(const int _value)const
{
	return { x / _value , y / _value, z / _value };
}

void IntVector3::operator/=(const int _value)
{
	x /= _value;
	y /= _value;
	z /= _value;
}

VECTOR IntVector3::operator*(const VECTOR& _value) const
{
	return VECTOR(static_cast<float>(x) * _value.x, static_cast<float>(y) * _value.y, static_cast<float>(z) * _value.z);
}

bool IntVector3::operator==(const IntVector3& _value) const
{
	bool diffX = x == _value.x;
	bool diffY = y == _value.y;
	bool diffZ = z == _value.z;

	return diffX && diffY && diffZ;
}

bool IntVector3::operator<(const IntVector3& _value) const
{
	bool diffX = x < _value.x;
	bool diffY = y < _value.y;
	bool diffZ = z < _value.z;

	return diffX && diffY && diffZ;
}

bool IntVector3::operator<=(const IntVector3& _value) const
{
	bool diffX = x <= _value.x;
	bool diffY = y <= _value.y;
	bool diffZ = z <= _value.z;

	return diffX && diffY && diffZ;
}

bool IntVector3::operator>(const IntVector3& _value) const
{
	bool diffX = x > _value.x;
	bool diffY = y > _value.y;
	bool diffZ = z > _value.z;

	return diffX && diffY && diffZ;
}

bool IntVector3::operator>=(const IntVector3& _value) const
{
	bool diffX = x >= _value.x;
	bool diffY = y >= _value.y;
	bool diffZ = z >= _value.z;

	return diffX && diffY && diffZ;
}

VECTOR IntVector3::ToVECTOR(void) const
{
	return VECTOR(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

VECTOR IntVectorToVECTOR(const IntVector3& _iv)
{
	float vx = static_cast<float>(_iv.x);
	float vy = static_cast<float>(_iv.y);
	float vz = static_cast<float>(_iv.z);

	return VGet(vx, vy, vz);
}

static IntVector3 VECTORToIntVector(VECTOR _v)
{
	IntVector3 intVec;

	intVec.x = static_cast<int>(_v.x);
	intVec.y = static_cast<int>(_v.y);
	intVec.z = static_cast<int>(_v.z);

	return intVec;
}

Vector2F::Vector2F(const Vector2I& _value)
{
	x = static_cast<float>(_value.x);
	y = static_cast<float>(_value.y);
}

Vector2F Vector2F::operator+(const Vector2F& _value) const
{
	return Vector2F(x + _value.x, y + _value.y);
}

void Vector2F::operator+=(const Vector2F& _value)
{
	x += _value.x;
	y += _value.y;
}

Vector2F Vector2F::operator-(const Vector2F& _value) const
{
	return Vector2F(x - _value.x, y - _value.y);
}

void Vector2F::operator-=(const Vector2F& _value)
{
	x -= _value.x;
	y -= _value.y;
}

Vector2F Vector2F::operator*(const float _value) const
{
	return Vector2F(x * _value, y * _value);
}

void Vector2F::operator*=(const float _value)
{
	x *= _value;
	y *= _value;
}

Vector2F Vector2F::operator/(const float _value) const
{
	return Vector2F(x / _value, y / _value);
}

void Vector2F::operator/=(const float _value)
{
	x /= _value;
	y /= _value;
}

Vector2F Vector2F::operator*(const Vector2F& _value) const
{
	return Vector2F(x * _value.x, y * _value.y);
}

void Vector2F::operator*=(const Vector2F& _value)
{
	x *= _value.x;
	y *= _value.y;
}

Vector2F Vector2F::operator/(const Vector2F& _value) const
{
	return Vector2F(x / _value.x, y / _value.y);
}

void Vector2F::operator/=(const Vector2F& _value)
{
	x /= _value.x;
	y /= _value.y;
}

Vector2I::Vector2I(const Vector2F& _value)
{
	x = static_cast<int>(_value.x);
	y = static_cast<int>(_value.y);
}

Vector2I Vector2I::operator+(const Vector2I& _value) const
{
	return Vector2I(x + _value.x, y + _value.y);
}

void Vector2I::operator+=(const Vector2I& _value)
{
	x += _value.x;
	y += _value.y;
}

Vector2I Vector2I::operator-(const Vector2I& _value) const
{
	return Vector2I(x - _value.x, y - _value.y);
}

void Vector2I::operator-=(const Vector2I& _value)
{
	x -= _value.x;
	y -= _value.y;
}

Vector2I Vector2I::operator*(const int _value) const
{
	return Vector2I(x * _value, y * _value);
}

void Vector2I::operator*=(const int _value)
{
	x *= _value;
	y *= _value;
}

Vector2I Vector2I::operator/(const int _value) const
{
	return Vector2I(x / _value, y / _value);
}

void Vector2I::operator/=(const int _value)
{
	x /= _value;
	y /= _value;
}

Vector2I Vector2I::operator*(const Vector2I& _value) const
{
	return Vector2I(x * _value.x, y * _value.y);
}

void Vector2I::operator*=(const Vector2I& _value)
{
	x *= _value.x;
	y *= _value.y;
}

Vector2I Vector2I::operator/(const Vector2I& _value) const
{
	return Vector2I(x / _value.x, y / _value.y);
}

void Vector2I::operator/=(const Vector2I& _value)
{
	x /= _value.x;
	y /= _value.y;
}