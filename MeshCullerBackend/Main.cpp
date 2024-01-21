#include <iostream>
#include <filesystem>

#include <fbxsdk.h>
#include <glm.hpp>

#include "Math.hpp"

struct MeshInstance
{
	std::filesystem::path SrcPath;
	std::filesystem::path DstPath;
	Vec Origin;
	Vec Angles;
	Vec Scale;
	double Scalar;
};

int main()
{
	std::filesystem::path outputBase = "C:\\Users\\Jorijn\\Downloads\\CulledMeshes";
	std::filesystem::path filename = "C:\\Users\\Jorijn\\Downloads\\large_rock_01_mesh.fbx";

	std::vector<MeshInstance> instances = {
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_01.fbx", .Origin = { 331.65942, 156.34814, -226.48233 }, .Angles = { -2.2391095, 344.0404, -4.232919 }, .Scale = { 1.0, 1.0, 1.0 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_02.fbx", .Origin = { 306.3142, 247.17712, -120.57349 }, .Angles = { -7.0887804, 253.13556, -19.575325 }, .Scale = { 1.0, 1.0, 1.0 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_03.fbx", .Origin = { 376.9248, 442.10974, -128.0 }, .Angles = { -2.5045664, 3.4257908, -1.1577003 }, .Scale = { 1.0, 1.0, 1.0 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_04.fbx", .Origin = { 392.0, 559.4717, 152.71129 }, .Angles = { -87.12707, 270.00024, 179.9995 }, .Scale = { 1.0, 1.0, 1.0 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_05.fbx", .Origin = { 422.19678, 224.35583, -218.56755 }, .Angles = { 0.0, 194.8226, -13.57437 }, .Scale = { 1.21, 1.21, 1.21 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_06.fbx", .Origin = { 992.0, 624.0, -16.064362 }, .Angles = { -39.52677, 302.17978, 73.41033 }, .Scale = { 1.7710773, 1.7710773, 1.7710773 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_07.fbx", .Origin = { 518.59204, 232.0, -448.0 }, .Angles = { 0.0, 110.06694, 0.0 }, .Scale = { 1.7710773, 1.7710773, 1.7710773 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_08.fbx", .Origin = { 904.0, 343.12085, -501.33936 }, .Angles = { -5.759672, 285.32416, 4.4487433 }, .Scale = { 1.7710773, 1.7710773, 1.7710773 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_09.fbx", .Origin = { 1739.7356, 896.6122, -768.00006 }, .Angles = { -19.436089, 301.53894, 57.5114 }, .Scale = { 3.7723944, 3.7723944, 3.7723944 }, .Scalar = 3.0 },
		MeshInstance { .SrcPath = filename, .DstPath = outputBase / "mesh_10.fbx", .Origin = { 118.0, -38.0, -708.0 }, .Angles = { 0.0, 174.7276, 0.0 }, .Scale = { 1.8950528, 1.8950526, 1.8950528 }, .Scalar = 3.0 },
	};

	std::vector<const char*> names = {
		"Gerry", "Gregg", "Finn", "Pondus", "Connor",
		"Berry", "Dumbledor", "Harry", "Gudrunn", "Per Ivar"
	};

	std::vector<FbxScene*> scenes;

	FbxManager* manager = FbxManager::Create();

	for (size_t i = 0; i < instances.size(); ++i) {
		MeshInstance& instance = instances[i];

		FbxImporter* importer = FbxImporter::Create(manager, "");
		if (!importer->Initialize(instance.SrcPath.generic_string().c_str(), -1, manager->GetIOSettings())) {
			std::cout << "Call to FbxImporter::Initialize() failed.\n";
			std::cout << "Error returned: " << importer->GetStatus().GetErrorString() << "\n\n";
			exit(-1);
		}

		FbxScene* scene = FbxScene::Create(manager, names[i]);
		scenes.push_back(scene);

		importer->Import(scene);
		importer->Destroy();

		// TODO: Apply transforms? Relative to export origin?
		FbxNode* rootNode = scene->GetRootNode();
		if (rootNode) {
			FbxNode* mesh = rootNode->GetChild(0);

			std::cout << mesh->LclTranslation.Get()[0] << " " << mesh->LclTranslation.Get()[1] << " " << mesh->LclTranslation.Get()[2] << "\n";
			std::cout << mesh->LclRotation.Get()[0] << " " << mesh->LclRotation.Get()[1] << " " << mesh->LclRotation.Get()[2] << "\n";
			std::cout << mesh->LclScaling.Get()[0] << " " << mesh->LclScaling.Get()[1] << " " << mesh->LclScaling.Get()[2] << "\n";
			std::cout << "\n";

			for (size_t j = 0; j < mesh->GetMesh()->GetControlPointsCount(); ++j) {
				FbxVector4 controlPoint = mesh->GetMesh()->GetControlPointAt(j);

				controlPoint.Set(
					controlPoint[0] * mesh->LclScaling.Get()[0],
					controlPoint[1] * mesh->LclScaling.Get()[1],
					controlPoint[2] * mesh->LclScaling.Get()[2]);

				//controlPoint.Set(
				//	controlPoint[0] * mesh->LclRotation.Get()[0],
				//	controlPoint[1] * mesh->LclRotation.Get()[1],
				//	controlPoint[2] * mesh->LclRotation.Get()[2]);

				controlPoint.Set(
					controlPoint[0] - mesh->LclTranslation.Get()[0],
					controlPoint[1] - mesh->LclTranslation.Get()[1],
					controlPoint[2] - mesh->LclTranslation.Get()[2]);

				mesh->GetMesh()->SetControlPointAt(controlPoint, j);
			}

			//FbxVector4* controlPoints = mesh->GetMesh()->GetControlPointsCount();
			//controlPoints->Set()

			//Vec translation = instance.Origin;
			//mesh->LclTranslation.Set({ translation.X, translation.Y, translation.Z });
			mesh->LclTranslation.Set({ 0.0, 0.0, 0.0 });
			
			//Vec rotation = instance.Angles;
			//Vec rotation = radians(instance.Angles);
			//mesh->LclRotation.Set({ rotation.X, rotation.Y, rotation.Z });
			mesh->LclRotation.Set({ 0.0, 0.0, 0.0 });
			
			//Vec scaling = instance.Scale * instance.Scalar;
			//mesh->LclScaling.Set({ scaling.X, scaling.Y, scaling.Z });
			mesh->LclScaling.Set({ 1.0, 1.0, 1.0 });

			//mesh->Destroy();
		}
	}

	for (size_t i = 0; i < scenes.size(); ++i) {
		MeshInstance& instance = instances[i];
		FbxScene* scene = scenes[i];

		FbxExporter* exporter = FbxExporter::Create(manager, "");
		if (!exporter->Initialize(instance.DstPath.generic_string().c_str(), -1, manager->GetIOSettings())) {
			std::cout << "Call to FbxExporter::Initialize() failed.\n";
			std::cout << "Error returned: " << exporter->GetStatus().GetErrorString() << "\n\n";
			exit(-1);
		}

		exporter->Export(scene);
		exporter->Destroy();
	}

	manager->Destroy();
}
