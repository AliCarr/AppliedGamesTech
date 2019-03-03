#include "PuzzlePieces.h"

PuzzlePieces::PuzzlePieces(gef::Platform* platform, const char *name, gef::Vector4 solutionPos, float solutionHeight)
{
	model = new ModelLoader( platform, name);
}

PuzzlePieces::~PuzzlePieces()
{
	delete model;
	model = NULL;
}

void PuzzlePieces::update( gef::Matrix44* markerMatrix, float heightOffset, float scaleOffset)
{
	//Reset the scae matrix
	scaleMatrix.SetIdentity();

	//Base rotation, scaling, and translation to ensure the mesh sits on marker, at the right size
	scaleMatrix.Scale(gef::Vector4(STANDERED_SCALE*scaleOffset, 
								   STANDERED_SCALE*scaleOffset, 
								   STANDERED_SCALE*scaleOffset, 
								   1.f));

	extraTranslation.SetTranslation(STANDERED_OFFSET);

	rotation.RotationX(STANDERED_ROTATION);

	finalMatrix = scaleMatrix * rotation *(*markerMatrix);

	markerPosition = *markerMatrix;

	zAxis = gef::Vector4(markerMatrix->m(2, 0), 
						 markerMatrix->m(2, 1), 
						 markerMatrix->m(2, 2));

	position = markerPosition.GetTranslation() + zAxis * heightOffset; //Last float is how far you want it up or down
	
	finalMatrix.SetTranslation(position);
	
	//The current height is needed for testing, so it's updated every call
	height = heightOffset;

	if (model)
		model->getMesh().set_transform(finalMatrix);

	//The model loader is a separate class, so it has it's own update function
	model->update(finalMatrix);
}

void PuzzlePieces::onRender(gef::Renderer3D *renderer)
{
	if (model)
		renderer->DrawMesh(model->getMesh());
}
