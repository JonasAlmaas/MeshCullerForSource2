#pragma once

#include <iostream>

class Vec
{
public:
	double X, Y, Z;

	Vec(double x, double y, double z)
		: X(x), Y(y), Z(z) { }

	inline Vec operator+(const Vec& other) const
	{
		return { X + other.X, Y + other.Y, Z + other.Z };
	}

	inline Vec operator-(const Vec& other) const
	{
		return { X - other.X, Y - other.Y, Z - other.Z };
	}

	inline Vec operator*(double scalar) const
	{
		return { X * scalar, Y * scalar, Z * scalar };
	}

	inline Vec operator/(double scalar) const
	{
		return { X / scalar, Y / scalar, Z / scalar };
	}

	inline double dot(const Vec& other) const
	{
		return (X * other.X) + (Y * other.Y) + (Z * other.Z);
	}

	inline Vec cross(const Vec& other) const
	{
		return {
			Y * other.Z - Z * other.Y,
			-(X * other.Z - Z * other.X),
			X * other.Y - Y * other.X
		};
	}

	friend auto operator<<(std::ostream& os, const Vec& vec) -> std::ostream&
	{
		return os << "Vec(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
	}
};

class Ray
{
public:
	Vec Origin, Direction;

	Ray(const Vec& origin, const Vec& direction)
		: Origin(origin), Direction(direction) { }

	bool IntersectsTriangle(const Vec& a, const Vec& b, const Vec& c) const;

	friend auto operator<<(std::ostream& os, const Ray& ray) -> std::ostream&
	{
		return os << "Ray(" << ray.Origin << ", " << ray.Direction << ")";
	}
};

inline double radians(double value)
{
	return value * 0.01745329251994329576923690768489;
}

inline Vec radians(Vec vec)
{
	return { radians(vec.X), radians(vec.Y), radians(vec.Z) };
}

inline double degrees(double value)
{
	return value * 57.295779513082320876798154814105;
}

inline Vec degrees(Vec vec)
{
	return { degrees(vec.X), degrees(vec.Y), degrees(vec.Z) };
}
