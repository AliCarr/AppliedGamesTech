#include "ModelLoader.h"

ModelLoader::ModelLoader(gef::Platform* platform, const char *name)
{
	//Set up model scene with the model specified in ar_app.cpp
	model_scene_ = new gef::Scene();
	model_scene_->ReadSceneFromFile(*platform, name);
	model_scene_->CreateMaterials(*platform);

	//Set the loaded model in it's own mesh object
	mesh = new gef::MeshInstance();
	mesh->set_mesh(GetFirstMesh(model_scene_, platform));
}


ModelLoader::~ModelLoader()
{
	delete model_scene_;
	model_scene_ = NULL;

	delete mesh;
	mesh = NULL;
}

gef::Mesh* ModelLoader::GetFirstMesh(gef::Scene* scene, gef::Platform* platform)
{
	gef::Mesh* mesh = NULL;

	if (scene->mesh_data.size() > 0)
		mesh = model_scene_->CreateMesh(*platform, scene->mesh_data.front());

	return mesh;
}

void ModelLoader::update(gef::Matrix44 mat)
{
	//Updated Matrix is determined in the puzzle piece class
	mesh->set_transform(mat);
}
