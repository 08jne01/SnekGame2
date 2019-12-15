#pragma once
#include <iostream>
#include <algorithm>

template <class T> class Vec2

{
public:
	Vec2(T x_, T y_) : x(x_), y(y_)

	{

	}

	Vec2(T v) : x(v), y(v)

	{

	}

	Vec2()

	{

	}
	//Vector plus
	Vec2 operator+ (const Vec2& v) const

	{
		Vec2 result;
		result.x = this->x + v.x;
		result.y = this->y + v.y;
		return result;
	}
	//Vector minus
	Vec2 operator- (const Vec2& v) const

	{
		Vec2 result;
		result.x = this->x - v.x;
		result.y = this->y - v.y;
		return result;
	}
	//Vector plus equals
	Vec2& operator+= (const Vec2& v)

	{
		this->x += v.x;
		this->y += v.y;
		return *this;
	}
	//Vector minus equals
	Vec2& operator-= (const Vec2& v)

	{
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}
	//Dot Product
	T operator* (const Vec2& v) const

	{
		T result;
		result = (this->x * v.x) + (this->y * v.y);
		return result;
	}
	//Vector/Scalar
	Vec2 operator/ (const T& v) const

	{
		Vec2 result;
		result.x = this->x/v;
		result.y = this->y/v;
		return result;
	}
	//Vector times equals scalar
	Vec2& operator*= (const T& v)

	{
		this->x *=v;
		this->y *=v;
		return *this;
	}


	//Vector divide equals scalar
	Vec2& operator/= (const T& v)

	{
		this->x /= v;
		this->y /= v;
		return *this;
	}
	//Vector = -Vector
	Vec2& operator- () 

	{
		this->x = -this->x;
		this->y = -this->y;
		return *this;
	}

	bool operator== (const Vec2& v) const

	{
		return (v.x == this->x && v.y == this->y);
	}

	//In Place Normalize
	inline void normalizeInPlace()

	{
		T mag = magnitude(*this);
		x /= mag;
		y /= mag;
	}

	//In place set mag
	inline void setMagnitude(T mag)

	{
		normalizeInPlace();
		x *= mag;
		y *= mag;
	}

	//In place rotate
	inline void rotateInPlace(T angle)

	{
		*this = rotate(*this, angle);
	}

	inline void limit(T value)

	{
		T mag = magnitudeInPlace();
		T newMag = std::min(mag, value) / mag;
		x *= newMag;
		y *= newMag;
	}

	//In place Magnitude
	inline T magnitudeInPlace()

	{
		return magnitude(*this);
	}

	T x;
	T y;
};

template <class T> Vec2<T> operator* (const T& v, const Vec2<T>& vec)

{
	Vec2<T> result;
	result.x = vec.x*v;
	result.y = vec.y*v;
	return result;
}

template <class T> Vec2<T> operator* (const Vec2<T>& vec, const T& v)

{
	Vec2<T> result;
	result.x = vec.x*v;
	result.y = vec.y*v;
	return result;
}

template <class T> std::ostream& operator<<(std::ostream& os, const Vec2<T> vec)

{
	os << "x: " << vec.x << " y: " << vec.y;
	return os;
}

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;