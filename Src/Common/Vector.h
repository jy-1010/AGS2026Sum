#pragma once
#include <DxLib.h>

struct IntVector3;
//struct Vector2F;
struct Vector2I;

struct IntVector3
{
	union 
	{
		struct { int x, y, z; };
		struct { int u, v, w; };
		struct { int r, g, b; };
		int data[3];
	};
	IntVector3(void) : x(0), y(0), z(0) {}
	IntVector3(int vX, int vY, int vZ) : x(vX), y(vY), z(vZ) {}
	IntVector3(VECTOR _value) : x(static_cast<int>(_value.x)), y(static_cast<int>(_value.y)), z(static_cast<int>(_value.z)) {}

	//演算
	IntVector3 operator+(const IntVector3& _value)const;
	void operator+=(const IntVector3& _value);
	IntVector3 operator-(const IntVector3& _value)const;
	void operator-=(const IntVector3& _value);
	IntVector3 operator*(const int _value)const;
	void operator*=(const int _value);
	IntVector3 operator/(const int _value)const;
	void operator/=(const int _value);
	VECTOR operator*(const VECTOR& _value)const;
	//比較
	bool operator==(const IntVector3& _value)const;
	bool operator<(const IntVector3& _value)const;
	bool operator<=(const IntVector3& _value)const;
	bool operator>(const IntVector3& _value)const;
	bool operator>=(const IntVector3& _value)const;
	//変換
	VECTOR ToVECTOR(void)const;
};

struct Vector2F
{
	union
	{
		struct { float x, y; };
		struct { float u, v; };
		float data[2];
	};

	Vector2F() : x(0.0f), y(0.0f) {}
	Vector2F(float vX, float vY) : x(vX), y(vY) {}
	Vector2F(const Vector2I& _value);

	// 演算
	Vector2F operator+(const Vector2F& _value) const;
	void operator+=(const Vector2F& _value);
	Vector2F operator-(const Vector2F& _value) const;
	void operator-=(const Vector2F& _value);
	Vector2F operator*(float _value) const;
	void operator*=(float _value);
	Vector2F operator/(float _value) const;
	void operator/=(float _value);
	Vector2F operator*(const Vector2F& _value) const;
	void operator*=(const Vector2F& _value);
	Vector2F operator/(const Vector2F& _value) const;
	void operator/=(const Vector2F& _value);

	// 比較
	bool operator==(const Vector2F& _value) const;
	bool operator!=(const Vector2F& _value) const;
};

struct Vector2I
{
	union
	{
		struct { int x, y; };
		struct { int u, v; };
		int data[2];
	};

	Vector2I() : x(0), y(0) {}
	Vector2I(int vX, int vY) : x(vX), y(vY) {}
	Vector2I(const Vector2F& _value);

	// 演算
	Vector2I operator+(const Vector2I& _value) const;
	void operator+=(const Vector2I& _value);
	Vector2I operator-(const Vector2I& _value) const;
	void operator-=(const Vector2I& _value);
	Vector2I operator*(int _value) const;
	void operator*=(int _value);
	Vector2I operator/(int _value) const;
	void operator/=(int _value);
	Vector2I operator*(const Vector2I& _value) const;
	void operator*=(const Vector2I& _value);
	Vector2I operator/(const Vector2I& _value) const;
	void operator/=(const Vector2I& _value);
	// 比較
	bool operator==(const Vector2I& _value) const;
	bool operator!=(const Vector2I& _value) const;
};

//static constexpr IntVector3 ZERO_INT_VECTOR3 = {0, 0, 0};