#pragma once
#include "Vec2.h"

template <class T> inline T magnitude(const Vec2<T>& v)

{
	return sqrt(v*v);
}

template <class T> inline Vec2<T> normalize(const Vec2<T>& v)

{
	T mag = magnitude(v);
	Vec2<T> normalized = v / mag;
	return normalized;
}

template <class T> inline Vec2<T> rotate(const Vec2<T>& v, const T& angle)

{
	return Vec2<T>(v.x*cos(angle) - v.y*sin(angle), v.x*sin(angle) + v.y*cos(angle));
}

template <class T> inline T distanceSquared(const Vec2<T>& v1, const Vec2<T>& v2)

{
	return pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2);
}

template <class T> inline T distance(const Vec2<T>& v1, const Vec2<T>& v2)

{
	return sqrt(distanceSquared(v1, v2));
}

template <class T> inline Vec2<T> vecAbs(const Vec2<T>& v)

{
	return Vec2<T>(abs(v.x), abs(v.y));
}

template <class T> inline Vec2<T> lerp(const Vec2<T>& v1, const Vec2<T>& v2, const T& weight)

{
	Vec2<T> ret;
	ret.x = v1.x + weight * (v2.x - v1.x);
	ret.y = v1.y + weight * (v2.y - v1.y);
	return ret;
}

static inline int randomi(const int min, const int max)

{
	return rand() % (max - min + 1) + min;
}

static inline float randomf(const float min, const float max)

{
	float fraction = (float)rand() / (float)RAND_MAX;
	return (max - min) * fraction + min;
}

