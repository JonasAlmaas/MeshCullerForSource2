#include "Triangle.hpp"

bool Triangle::RayIntersect(const glm::vec3& origin, const glm::vec3& direction, bool& overlap) const
{
	const float det = -glm::dot(direction, Normal);
	const float invdet = 1.0f / det;

	const glm::vec3 ao = origin - A;
	const float t = glm::dot(ao, Normal) * invdet;
	if (t < 0.0f) return false;
	const glm::vec3 dao = glm::cross(ao, direction);

	const float u = glm::dot(CA, dao) * invdet;
	if (u < 0.0f) return false;
	const float v = glm::dot(AB, dao) * invdet;
	if (v < 0.0f) return false;
	if (u + v > 1.0f) return false;

	if (t <= glm::length(direction)) overlap = true;
	return true;
}

void TriangleIntersect(Triangle& T1, Triangle& T2)
{
	bool overlap = false;

	if (T2.RayIntersect(T1.A, T1.AB, overlap)) T1.Inside = !T1.Inside;
	T2.RayIntersect(T1.B, T1.BC, overlap);
	T2.RayIntersect(T1.C, T1.CA, overlap);

	if (T1.RayIntersect(T2.A, T2.AB, overlap)) T2.Inside = !T2.Inside;
	T1.RayIntersect(T2.B, T2.BC, overlap);
	T1.RayIntersect(T2.C, T2.CA, overlap);

	if (overlap) {
		T1.Overlap = true;
		T2.Overlap = true;
	}
}
