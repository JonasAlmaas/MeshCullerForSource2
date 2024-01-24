#pragma once

#include <vector>

#include <fbxsdk.h>
#include <glm/glm.hpp>

#include "Math.hpp"
#include "Triangle.hpp"

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
