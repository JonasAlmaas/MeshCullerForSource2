#include "Triangle.hpp"

bool Triangle::RayIntersect(const glm::vec3& origin, const glm::vec3& direction, float length, bool& overlap) const
{
	const float det = -glm::dot(direction, Normal);
	const float invdet = 1.0f / det;

	const glm::vec3 ao = origin - P1;
	const float t = glm::dot(ao, Normal) * invdet;
	if (t < 0.0f) return false;
	const glm::vec3 dao = glm::cross(ao, direction);

	const float u = glm::dot(E3, dao) * invdet;
	if (u < 0.0f) return false;
	const float v = glm::dot(E1, dao) * invdet;
	if (v < 0.0f) return false;
	if (u + v > 1.0f) return false;

	if (t <= length) overlap = true;
	return true;
}

void TriangleIntersect(Triangle& T1, Triangle& T2)
{
	bool overlap = false;

	if (T2.RayIntersect(T1.P1, T1.E1, T1.L1, overlap)) T1.Inside = !T1.Inside;
	T2.RayIntersect(T1.P2, T1.E2, T1.L2, overlap);
	T2.RayIntersect(T1.P3, T1.E3, T1.L3, overlap);

	if (T1.RayIntersect(T2.P1, T2.E1, T2.L1, overlap)) T2.Inside = !T2.Inside;
	T1.RayIntersect(T2.P2, T2.E2, T2.L2, overlap);
	T1.RayIntersect(T2.P3, T2.E3, T2.L3, overlap);

	if (overlap) {
		T1.Overlap = true;
		T2.Overlap = true;
	}
}
