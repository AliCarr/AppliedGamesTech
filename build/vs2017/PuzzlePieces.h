#pragma once

//#include <libsmart.h>
#include <graphics/scene.h>
//#include <graphics\mesh_instance.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
#include "ModelLoader.h"

#define STANDERED_SCALE 0.02f
#define STANDERED_ROTATION 1.1f
#define STANDERED_OFFSET gef::Vector4(-7, -2, -7, 0)

namespace gef
{
	//class Scene;
	class Platform;
	//class MeshInstance;
	//class Mesh;
}

class PuzzlePieces
{
public:
	PuzzlePieces(gef::Platform*, const char *, gef::Vector4, float);
	~PuzzlePieces();

	void update( gef::Matrix44*, float, float);
	void onRender(gef::Renderer3D*);

	gef::Matrix44 getSolutionPosition() { return markerSolution; };
	gef::Matrix44 getMarkerPosition() { return markerPosition; };
	gef::Matrix44 getFinalMatrix() { return finalMatrix; };
	float getHeight() { return height; };
	//float getSolutionHeight() { return solutionHeight; };

	gef::Matrix44 setSolutionPosition(gef::Matrix44 m) { markerSolution = m; };
	gef::Vector4 setSolutionPositionTranslation(gef::Vector4 v) { markerSolution.SetTranslation(v); };

private:
	gef::Matrix44 scaleMatrix;
	gef::Matrix44 finalMatrix, extraTranslation, markerPosition;
	gef::Matrix44 markerSolution;
	gef::Matrix44 rotation;

	gef::Vector4 zAxis;
	gef::Vector4 position;
	ModelLoader *model;
	float height;
	//float solutionHeight;
	
};

