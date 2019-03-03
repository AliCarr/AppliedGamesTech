#pragma once

#include "PuzzlePieces.h"
#include <system/platform.h>

//Puzzle presets
#define NUMBER_OF_PIECES 3
#define PIECE_ONE 0
#define PIECE_TWO 1
#define PIECE_THREE 2
#define TOTAL_SOLVED_SECTIONS 2

//Available Puzzles
#define TUTORIAL_PUZZLE 0
#define MONSTER_PUZZLE 1

namespace gef
{
	class Platform;
}

class Puzzle
{
public:
	Puzzle(gef::Platform*, int);
	~Puzzle();
	void update(gef::Matrix44*, float,  int, bool&, int);
	void onRender(gef::Renderer3D*, int);
	bool hasThePuzzleBeenSolved();
	void updateWhenSolved(gef::Matrix44*, gef::Matrix44*, float, int, int);
	bool areOneAndTwoSolved, areTwoAndThreeSolved;
	void reset();
private:
	PuzzlePieces * pieces[NUMBER_OF_PIECES];
	gef::Matrix44 inverseMarker, solutionDisFromOneToTwo, solutionDisFromTwoToThree;
	gef::Matrix44 markerMatTest, distanceFromMarkerOneToTwo, distanceFromMarkerTwoToThree;

	float scaleFactor[NUMBER_OF_PIECES];
	float solutionHeight[NUMBER_OF_PIECES];
	float heightLeeway, positionLeeway;

	gef::Vector4 puzzleSolutionVectors[NUMBER_OF_PIECES];

	bool vectorCheck(gef::Vector4, gef::Vector4);
	bool areTheRelativeHeightsRightFor(float, float, float, float);
	void determineMarkerSolutions(gef::Matrix44, gef::Matrix44, gef::Matrix44);

	float solvedXOffset[TOTAL_SOLVED_SECTIONS], 
		  solvedYOffset[TOTAL_SOLVED_SECTIONS], 
		  solvedHeightOffset[TOTAL_SOLVED_SECTIONS];

	bool isMarkerInView[NUMBER_OF_PIECES];
};

