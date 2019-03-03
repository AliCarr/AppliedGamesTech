#include "Puzzle.h"



Puzzle::Puzzle(gef::Platform *platform, int setPuzzle)
{
	//As each puzzle has it's own solution values, these will be set depending on which puzzle was picked in ar_app.cpp
	if (setPuzzle == MONSTER_PUZZLE)
	{
		pieces[PIECE_ONE] =   new PuzzlePieces(platform, "backMonster.scn",   gef::Vector4(-0.08f, -0.02f, -0.59f, +1.0f), +0.0f);
		pieces[PIECE_TWO] =   new PuzzlePieces(platform, "middleMonster.scn", gef::Vector4(+0.03f, -0.07f, -0.52f, +1.0f), +0.0f);
		pieces[PIECE_THREE] = new PuzzlePieces(platform, "frontMonster.scn",  gef::Vector4(+0.00f, -0.10f, -0.47f, +1.0f), +0.0f);

		puzzleSolutionVectors[PIECE_ONE] =   gef::Vector4(gef::Vector4(-0.08f, -0.02f, -0.59f, +1.0f));
		puzzleSolutionVectors[PIECE_TWO] =   gef::Vector4(gef::Vector4(+0.03f, -0.07f, -0.52f, +1.0f));
		puzzleSolutionVectors[PIECE_THREE] = gef::Vector4(gef::Vector4(+0.00f, -0.10f, -0.47f, +1.0f));
		
		scaleFactor[PIECE_ONE] =   1.0f;
		scaleFactor[PIECE_TWO] =   1.1f;
		scaleFactor[PIECE_THREE] = 0.7f;

		solutionHeight[PIECE_ONE] =   0.00f;
		solutionHeight[PIECE_TWO] =   0.06f;
		solutionHeight[PIECE_THREE] = 0.155f;

		areOneAndTwoSolved = false;

		solvedXOffset[0] = -0.105f;
		solvedYOffset[0] = 0.03f;
		solvedHeightOffset[0] = -0.03f;

		solvedXOffset[1] = -0.062;
		solvedYOffset[1] = -0.06;
		solvedHeightOffset[1] = 0.08;
	}

	if (setPuzzle == TUTORIAL_PUZZLE)
	{
		pieces[PIECE_ONE] =   new PuzzlePieces(platform, "leftBlock.scn",   gef::Vector4(-0.08f, -0.02f, -0.59f, +1.0f), +0.0f);
		pieces[PIECE_TWO] =   new PuzzlePieces(platform, "middleBlock.scn", gef::Vector4(+0.03f, -0.07f, -0.52f, +1.0f), +0.0f);
		pieces[PIECE_THREE] = new PuzzlePieces(platform, "rightBlock.scn",  gef::Vector4(+0.00f, -0.10f, -0.47f, +1.0f), +0.0f);

		puzzleSolutionVectors[PIECE_ONE] =   gef::Vector4(gef::Vector4(-0.12f, -0.015f, -0.69f, +1.0f));
		puzzleSolutionVectors[PIECE_TWO] =   gef::Vector4(gef::Vector4(-0.02f, -0.170f, -0.67f, +1.0f));
		puzzleSolutionVectors[PIECE_THREE] = gef::Vector4(gef::Vector4(+0.04f, -0.090f, -0.77f, +1.0f));

		scaleFactor[PIECE_ONE] =   0.60f;
		scaleFactor[PIECE_TWO] =   0.56f;
		scaleFactor[PIECE_THREE] = 0.65f;

		solutionHeight[PIECE_ONE] =   0.06f;
		solutionHeight[PIECE_TWO] =   0.10f;
		solutionHeight[PIECE_THREE] = 0.00f;

		solvedXOffset[0] = -0.094f;
		solvedYOffset[0] = 0.03f;
		solvedHeightOffset[0] = -0.03f;

		solvedXOffset[1] = 0.05f;
		solvedYOffset[1] = 0.059f;
		solvedHeightOffset[1] = -0.059;

		areOneAndTwoSolved = false;
	}
	
	//Set these to default amount, though will be updated later depending on difficulty level
	heightLeeway = 0.032;
	positionLeeway = 0.10;
}

Puzzle::~Puzzle()
{
	for (int c = 0; c < NUMBER_OF_PIECES; c++)
	{
		delete pieces[c];
		pieces[c] = 0;
	}
}

void Puzzle::update(gef::Matrix44 *marker, float height,  int markerNumber, bool &markerInView, int difficultyLevel)
{
		pieces[markerNumber]->update(marker, height, scaleFactor[markerNumber]);
		isMarkerInView[markerNumber] = markerInView;

		//Diffculty levels range from 1 to 3
		heightLeeway = 0.032/ difficultyLevel;
		positionLeeway = 0.10/ difficultyLevel;
}

void Puzzle::onRender(gef::Renderer3D* renderer, int currentPiece)
{
	pieces[currentPiece]->onRender(renderer);
}

bool Puzzle::hasThePuzzleBeenSolved()
{
	determineMarkerSolutions(pieces[PIECE_ONE]->getMarkerPosition(), 
							 pieces[PIECE_TWO]->getMarkerPosition(), 
							 pieces[PIECE_THREE]->getMarkerPosition());


	//Get the distances between marker two, and markers one and three
	inverseMarker.AffineInverse(pieces[PIECE_ONE]->getMarkerPosition());
	distanceFromMarkerOneToTwo   = pieces[PIECE_TWO]->getMarkerPosition() * inverseMarker;
	inverseMarker.AffineInverse(pieces[PIECE_TWO]->getMarkerPosition());
	distanceFromMarkerTwoToThree = pieces[PIECE_THREE]->getMarkerPosition() * inverseMarker;

	if (vectorCheck(distanceFromMarkerOneToTwo.GetTranslation(), solutionDisFromOneToTwo.GetTranslation()) &&
		areTheRelativeHeightsRightFor(pieces[PIECE_ONE]->getHeight(), pieces[PIECE_TWO]->getHeight(), solutionHeight[PIECE_ONE], solutionHeight[PIECE_TWO]) &&
		isMarkerInView[PIECE_ONE] == true && isMarkerInView[PIECE_TWO] == true)
			areOneAndTwoSolved = true;

	if (vectorCheck(distanceFromMarkerTwoToThree.GetTranslation(), solutionDisFromTwoToThree.GetTranslation()) &&
		areTheRelativeHeightsRightFor(pieces[PIECE_TWO]->getHeight(), pieces[PIECE_THREE]->getHeight(), solutionHeight[PIECE_TWO], solutionHeight[PIECE_THREE]) &&
		isMarkerInView[PIECE_TWO] == true && isMarkerInView[PIECE_THREE] == true)
			areTwoAndThreeSolved = true;
	

	if (areOneAndTwoSolved && areTwoAndThreeSolved)
		return true;

	return false;
}

void Puzzle::determineMarkerSolutions(gef::Matrix44 markerOne, gef::Matrix44 markerTwo, gef::Matrix44 markerThree)
{
	//Take existing marker matrix as the base matrix to be updated
	for (int c = 0; c < NUMBER_OF_PIECES; c++)
	{
		pieces[c]->setSolutionPosition(pieces[c]->getMarkerPosition());
		pieces[c]->setSolutionPositionTranslation(puzzleSolutionVectors[c]);
	}

	markerMatTest.AffineInverse(pieces[PIECE_ONE]->getSolutionPosition());
	solutionDisFromOneToTwo =   pieces[PIECE_TWO]->getSolutionPosition() * markerMatTest;

	markerMatTest.AffineInverse(pieces[PIECE_TWO]->getSolutionPosition());
	solutionDisFromTwoToThree = pieces[PIECE_THREE]->getSolutionPosition() * markerMatTest;

	
}

bool Puzzle::vectorCheck(gef::Vector4 marker, gef::Vector4 solution)
{
	//The Z value was omitted as it would be consistent for all of the markers anyway
	if (marker.x() > solution.x() - positionLeeway && marker.x() < solution.x() + positionLeeway &&
		marker.y() > solution.y() - positionLeeway && marker.y() < solution.y() + positionLeeway)
			return true;

	return false;
}

bool Puzzle::areTheRelativeHeightsRightFor(float firstPiece, float secondPiece, float firstSolution,  float secondSolution)
{
	float solutionDifference = secondSolution - firstSolution;
	float currentDifference = secondPiece - firstPiece;

	if (currentDifference < solutionDifference + heightLeeway && currentDifference > solutionDifference - heightLeeway)
		return true;

	return false;
}

void Puzzle::updateWhenSolved(gef::Matrix44 *marker, gef::Matrix44* markerTwo, float height, int markerNumber, int markerNumTwo)
{
	//start from new marker
	//have an offset from it
	gef::Matrix44 matrix = *markerTwo;

	gef::Vector4 xAxis = gef::Vector4(markerTwo->m(0, 0), markerTwo->m(0, 1), markerTwo->m(0, 2));
	gef::Vector4 yAxis = gef::Vector4(markerTwo->m(1, 0), markerTwo->m(1, 1), markerTwo->m(1, 2));
	gef::Vector4 position = matrix.GetTranslation() + (xAxis * solvedXOffset[markerNumber/2])  + (yAxis *solvedYOffset[markerNumber/2]);
	matrix.SetTranslation(position);

	pieces[markerNumber]->update(&matrix, solvedHeightOffset[markerNumber / 2] + height, scaleFactor[markerNumber]);
}

void Puzzle::reset()
{
	inverseMarker.SetIdentity();
	distanceFromMarkerOneToTwo.SetIdentity();
	distanceFromMarkerTwoToThree.SetIdentity();
	areOneAndTwoSolved = false;
	
	areTwoAndThreeSolved = false;
	for(int c = 0 ; c <3; c++)
		pieces[c]->update(&distanceFromMarkerOneToTwo, 0, scaleFactor[c]);
}