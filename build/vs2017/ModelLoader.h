#pragma once

#include <graphics/scene.h>
#include <graphics\mesh_instance.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
namespace gef
{
	class Scene;
	class Platform;
	class MeshInstance;
	class Mesh;
}

class ModelLoader //Currently only used in puzzle piece class, but made it's own incase need for 3d menu items
{
public:
	ModelLoader(gef::Platform* , const char *);
	~ModelLoader();

	gef::MeshInstance getMesh() { return *mesh; };
	void update(gef::Matrix44);

private:
	gef::Scene* model_scene_;

	gef::Mesh* GetFirstMesh(gef::Scene* scene, gef::Platform*);
	gef::MeshInstance *mesh;
};

