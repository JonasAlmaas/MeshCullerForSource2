#pragma once

#include <glm/glm.hpp>

class Triangle
{
private:
	glm::vec3 A, B, C;
	glm::vec3 AB, BC, CA;
	glm::vec3 Normal;

	bool Overlap = false;
	bool Inside = false;
	bool Cull = false;

	bool RayIntersect(const glm::vec3& origin, const glm::vec3& direction, bool& overlap) const;
	friend void TriangleIntersect(Triangle& T1, Triangle& T2);

public:
	inline Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
		: A(a), B(b), C(c), AB(B - A), BC(C - B), CA(A - C), Normal(glm::cross(AB, CA)) { }

	inline void UpdateAndReset()
	{
		if (!Overlap && Inside) Cull = true;
		Overlap = false;
		Inside = false;
	}

	inline bool ShouldCull() const
	{
		return Cull;
	}
};
