#pragma once

#include <fbxsdk.h>
#include <glm/glm.hpp>

inline glm::mat4 fbxMat2glmMat(const FbxMatrix& mat)
{
	// TODO: Test whether it should be row-wise or column-wise.
	return {
		mat[0][0], mat[1][0], mat[2][0], mat[3][0],
		mat[0][1], mat[1][1], mat[2][1], mat[3][1],
		mat[0][2], mat[1][2], mat[2][2], mat[3][2],
		mat[0][3], mat[1][3], mat[2][3], mat[3][3]
	};
}

bool rayIntersectsTriangle(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	const glm::vec3 E1 = b - a;
	const glm::vec3 E2 = c - a;
	const glm::vec3 N = glm::cross(E1, E2);
	const float det = -glm::dot(direction, N);
	const float invdet = 1.0f / det;
	const glm::vec3 AO = origin - a;
	const glm::vec3 DAO = glm::cross(AO, direction);
	const float u = glm::dot(E2, DAO) * invdet;
	const float v = -glm::dot(E1, DAO) * invdet;
	const float t = glm::dot(AO, N) * invdet;
	return det >= 1e-6f && t >= 0.0f && u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f;
};
