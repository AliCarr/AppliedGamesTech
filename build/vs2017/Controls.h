#pragma once

#define HEIGHT_MAX 0.2
#define HEIGHT_MIN -0.02

#define NUMBER_OF_PIECES 3
#define PIECE_ONE 0
#define PIECE_TWO 1
#define PIECE_THREE 2

#define RATE_OF_CHANGE 200

#include "input\sony_controller_input_manager.h"
#include <input/input_manager.h>
#include <system/platform.h>

namespace gef
{
	class SonyController;
	class SonyControllerInputManager;
}

class Controls
{
public:
	Controls(gef::SonyControllerInputManager*, gef::Platform& );
	~Controls();
	
	void update();

	float getDifference();

	int getCurrentPiece() { return currentPiece; };
	//button inputs for main application
	bool triangleButtonPressed() { return controller->buttons_pressed() == gef_SONY_CTRL_TRIANGLE; };
	bool crossPressed() {return controller->buttons_pressed() == gef_SONY_CTRL_CROSS;};
	bool circlePressed() { return controller->buttons_pressed() == gef_SONY_CTRL_CIRCLE; };
	bool startPressed() { return controller->buttons_pressed() == gef_SONY_CTRL_START; };
	void resetDifferences();

private:
	const gef::SonyController	*controller;
	gef::SonyControllerInputManager *controllerManger;
	float difference[NUMBER_OF_PIECES];
	int currentPiece;
	

};

