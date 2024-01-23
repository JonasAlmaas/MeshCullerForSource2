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

	bool Overlap = false;
	bool Inside = false;
	bool Cull = false;

public:
	Triangle(glm::vec3& a, glm::vec3& b, glm::vec3& c)
	{
		A = a; B = b; C = c;
		E1 = B - A; E2 = C - A;
		Normal = glm::cross(E1, E2);
	}

	void Intersect(Triangle& target)
	{
		glm::vec3 origin, direction;
		glm::vec3 origins[] = { A, B, C };
		glm::vec3 directions[] = { B - A, C - B, A - C }; // TODO: Pre-compute these.

		for (int i = 0; i < 3; ++i) {
			origin = origins[i];
			direction = directions[i];

			const float det = -glm::dot(direction, target.Normal);
			const float invdet = 1.0f / det;

			const glm::vec3 ao = origin - target.A;

			const float t = glm::dot(ao, target.Normal) * invdet;
			if (t < 0.0f) continue;

			const glm::vec3 dao = glm::cross(ao, direction);

			const float u = glm::dot(target.E2, dao) * invdet;
			if (u < 0.0f) continue;

			const float v = -glm::dot(target.E1, dao) * invdet;
			if (v < 0.0f) continue;

			if (u + v > 1.0f) continue;
			
			if (t <= glm::length(direction)) {
				Overlap = true;
				target.Overlap = true;
			}

			if (i == 0) Inside = !Inside;
		}
	}

	void UpdateAndReset()
	{
		if (!Overlap && Inside) Cull = true;
		Overlap = false;
		Inside = false;
	}

	bool ShouldCull()
	{
		return Cull;
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
