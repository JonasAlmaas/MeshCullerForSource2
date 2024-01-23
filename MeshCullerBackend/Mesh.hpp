#pragma once

#include <vector>

#include <fbxsdk.h>
#include <glm/glm.hpp>

#include "Math.hpp"

class Triangle
{
public:
	glm::vec3 A, B, C;
	glm::vec3 E1, E2;
	glm::vec3 Normal;

	bool Intersect = false;
	bool Inside = false;
	bool Cull = false;

	Triangle(glm::vec3& a, glm::vec3& b, glm::vec3& c)
	{
		A = a; B = b; C = c;
		E1 = B - A; E2 = C - A;
		Normal = glm::cross(E1, E2);
	}

	bool RayIntersect(const glm::vec3& origin, const glm::vec3& direction, float& distance)
	{
		const float det = -glm::dot(direction, Normal);
		const float invdet = 1.0f / det;

		const glm::vec3 AO = origin - A;

		distance = glm::dot(AO, Normal) * invdet;
		if (distance < 0.0f) return false;

		const glm::vec3 DAO = glm::cross(AO, direction);

		const float u = glm::dot(E2, DAO) * invdet;
		if (u < 0.0f) return false;

		const float v = -glm::dot(E1, DAO) * invdet;
		if (v < 0.0f) return false;
		
		return (u + v) <= 1.0f;
	}

	void UpdateAndReset()
	{
		if (!Intersect && Inside) Cull = true;
		Intersect = false;
		Inside = false;
	}
};

bool RayCastTriangles(Triangle& source, Triangle& target, const glm::vec3& origin, const glm::vec3& direction)
{
	float distance;

	if (!target.RayIntersect(origin, direction, distance)) return false;
	
	if (distance <= glm::length(direction)) {
		source.Intersect = true;
		target.Intersect = true;
	}

	return true;
}

void CompareTriangles(Triangle& t1, Triangle& t2)
{
	if (RayCastTriangles(t1, t2, t1.A, t1.B - t1.A)) t1.Inside = !t1.Inside;
	RayCastTriangles(t1, t2, t1.B, t1.C - t1.B);
	RayCastTriangles(t1, t2, t1.C, t1.A - t1.C);

	if (RayCastTriangles(t2, t1, t2.A, t2.B - t2.A)) t2.Inside = !t2.Inside;
	RayCastTriangles(t2, t1, t2.B, t2.C - t2.B);
	RayCastTriangles(t2, t1, t2.C, t2.A - t2.C);
}

class Mesh
{
public:
	std::vector<glm::vec3> Vertices;
	std::vector<Triangle> Triangles;

	Mesh(FbxNode* node)
	{
		glm::mat4 global = fbxMat2glmMat(node->EvaluateGlobalTransform());
		glm::mat4 local = fbxMat2glmMat(node->EvaluateLocalTransform());
		glm::mat4 parent = glm::inverse(local) * global;

		FbxMatrix temp;
		temp.SetIdentity();
		temp.SetTRS(node->LclTranslation.Get(), node->LclRotation.Get(), node->LclScaling.Get());

		local = fbxMat2glmMat(temp);
		glm::mat4 matrix = parent * local;

		// TODO: Test if matrix works as intended.

		FbxMesh* mesh = node->GetMesh();

		FbxVector4* points = mesh->GetControlPoints();
		int pointCount = mesh->GetControlPointsCount();

		for (int i = 0; i < pointCount; ++i) {
			FbxVector4& point = points[i];
			glm::vec4 vec = glm::vec4(point[0], point[1], point[2], 1.0);
			Vertices.push_back(glm::vec3(matrix * vec));
		}

		int* indices = mesh->GetPolygonVertices();
		int polygonCount = mesh->GetPolygonCount();

		for (int i = 0; i < polygonCount; ++i) {
			glm::vec3& a = Vertices[indices[i * 3 + 0]];
			glm::vec3& b = Vertices[indices[i * 3 + 1]];
			glm::vec3& c = Vertices[indices[i * 3 + 2]];

			Triangles.push_back(Triangle(a, b, c));
		}
	}
};
