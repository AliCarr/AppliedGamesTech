#include "Controls.h"

Controls::Controls(gef::SonyControllerInputManager* inputManager,  gef::Platform& platform)
{
	for(int c = 0; c< NUMBER_OF_PIECES; c++)
		difference[c] = 0;

	controller = new gef::SonyController();
	
	//Controller manager is passed in here, and updated in the main application
	controllerManger = inputManager;

	//Start off with the first piece
	currentPiece = PIECE_ONE;
}

Controls::~Controls()
{
	delete controller;
	controller = NULL;

	delete controllerManger;
	controllerManger = NULL;
}

void Controls::update()
{
	//controllerManger->Update();
	controller = controllerManger->GetController(0);
	
	//Moves the object dependant on the joysticks change in access
	difference[currentPiece] -= controller->left_stick_y_axis()/ RATE_OF_CHANGE;

	if (controller->buttons_pressed() == gef_SONY_CTRL_CIRCLE)
		currentPiece += 1;

	//Set hard limits so the piece doesn't move out of viewable space
	if (difference[currentPiece] < HEIGHT_MIN)
		difference[currentPiece] = HEIGHT_MIN;

	if (difference[currentPiece] > HEIGHT_MAX)
		difference[currentPiece] = HEIGHT_MAX;
	
	//To prevent the app from access an element the array doesn't have
	if (currentPiece > PIECE_THREE)
		currentPiece = PIECE_ONE;
}

float Controls::getDifference()
{
	return difference[currentPiece];
}

void Controls::resetDifferences()
{
	for(int c = 0 ; c< NUMBER_OF_PIECES; c++)
		difference[c] = 0;

}
