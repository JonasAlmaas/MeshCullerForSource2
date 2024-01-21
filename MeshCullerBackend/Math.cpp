#include "Math.hpp"

bool Ray::IntersectsTriangle(const Vec& a, const Vec& b, const Vec& c) const
{
	const Vec E1 = b - a;
	const Vec E2 = c - a;
	const Vec N = E1.cross(E2);
	const double det = -Direction.dot(N);
	const double invdet = 1.0f / det;
	const Vec AO = Origin - a;
	const Vec DAO = AO.cross(Direction);
	const double u = E2.dot(DAO) * invdet;
	const double v = -E1.dot(DAO) * invdet;
	const double t = AO.dot(N) * invdet;
	return (det >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0);
}
