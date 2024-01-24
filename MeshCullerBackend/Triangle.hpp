#pragma once

#include <glm/glm.hpp>

class Triangle
{
public:
	inline Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
		: P1(a), P2(b), P3(c), E1(P2 - P1), E2(P3 - P2), E3(P1 - P3),
		L1(glm::length(E1)), L2(glm::length(E2)), L3(glm::length(E3)),
		Normal(glm::cross(E1, E3))
	{ }

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

private:
	bool rayIntersect(const glm::vec3& origin, const glm::vec3& direction, float length, bool& overlap) const;

	static void triangleIntersect(Triangle& T1, Triangle& T2);
	
private:
	glm::vec3 P1, P2, P3;
	glm::vec3 E1, E2, E3;
	float L1, L2, L3;
	glm::vec3 Normal;

	bool Overlap = false;
	bool Inside = false;
	bool Cull = false;
	
};
