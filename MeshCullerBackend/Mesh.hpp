#pragma once

#include <vector>

#include <fbxsdk.h>
#include <glm/glm.hpp>

#include "Math.hpp"

class Triangle
{
private:
	glm::vec3 A, B, C;
	glm::vec3 AB, BC, CA;
	glm::vec3 Normal;

	bool Overlap = false;
	bool Inside = false;
	bool Cull = false;

public:
	Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
		: A(a), B(b), C(c), AB(B - A), BC(C - B), CA(A - C), Normal(glm::cross(AB, CA)) { }

private:
	bool RayIntersect(const glm::vec3& origin, const glm::vec3& direction, bool& overlap) const
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

public:
	friend void TriangleIntersect(Triangle& T1, Triangle& T2)
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

	void UpdateAndReset()
	{
		if (!Overlap && Inside) Cull = true;
		Overlap = false;
		Inside = false;
	}

	bool ShouldCull() const
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
