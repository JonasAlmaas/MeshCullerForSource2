#pragma once

#include <vector>

#include <fbxsdk.h>
#include <glm/glm.hpp>

#include "Math.hpp"

class Triangle
{
private:
	glm::vec3 A, B, C;
	glm::vec3 E1, E2;
	glm::vec3 Normal;

public:
	bool Cull = false;

	Triangle(glm::vec3& a, glm::vec3& b, glm::vec3& c)
	{
		A = a; B = b; C = c;
		E1 = B - A; E2 = C - A;
		Normal = glm::cross(E1, E2);
	}

	bool RayIntersect(glm::vec3 origin, glm::vec3 direction)
	{
		const float det = -glm::dot(direction, Normal);
		if (det > -1e-6f) return false;

		const float invdet = 1.0f / det;

		const glm::vec3 AO = origin - A;

		const float t = glm::dot(AO, Normal) * invdet;
		if (t < 0.0f) return false;

		const glm::vec3 DAO = glm::cross(AO, direction);

		const float u = glm::dot(E2, DAO) * invdet;
		if (u < 0.0f) return false;

		const float v = -glm::dot(E1, DAO) * invdet;
		if (v < 0.0f) return false;
		
		return (u + v) <= 1.0f;
	}

	bool InsideTriangle(Triangle other)
	{
		glm::vec3 direction = { 1.0, 0.0, 0.0 };

		if (other.RayIntersect(A, direction)) return false;
		if (other.RayIntersect(B, direction)) return false;
		if (other.RayIntersect(C, direction)) return false;

		if (other.RayIntersect((A + B) * glm::vec3(0.5), direction)) return false;
		if (other.RayIntersect((B + C) * glm::vec3(0.5), direction)) return false;
		if (other.RayIntersect((C + A) * glm::vec3(0.5), direction)) return false;

		// TODO: Sample more rays.

		return true;
	}
};

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
