#include "ar_app.h"


ARApp::ARApp(gef::Platform& platform) :
	Application(platform),
	input_manager_(NULL),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	myControls(NULL)
{
}

void ARApp::Init()
{
	input_manager_ = gef::InputManager::Create(platform_);
	myControls = new Controls(input_manager_->controller_input(), platform_);
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	audioControl = new Audio(platform_);

	//Current Puzzles
	tutorialPuzzle = new Puzzle(&platform_, 0);
	monsterPuzzle =  new Puzzle(&platform_, 1);

	//Menu Items
	background = new MenuItem(platform_, "background.png", 0.0f, 0.f,   3.5f, 3.5f,   2.f,  2.4f);
	buttons[0] = new StaticMenuItem(platform_, "UIBox.png", 00.0f, -00.7f, 1.0f, 1.0f, 0.75f, 0.45f);
	buttons[1] = new StaticMenuItem(platform_, "UIBox.png", 00.0f, -00.0f, 1.0f, 1.0f, 0.75f, 0.45f);
	buttons[2] = new StaticMenuItem(platform_, "UIBox.png", 00.0f, 00.7f, 1.0f, 1.0f, 0.75f, 0.45f);

	myCamera = new Camera(SCE_SMART_IMAGE_FOV);

	InitFont();
	SetupLights();

	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);

	currentGameState = SPLASH_SCREEN;

	for (int c = 0; c < NUMBER_OF_PIECES; c++)
		markerY[c] = 0;

	//Difficulty ranges from 1 to 3 (1 easiest, 3 hardest)
	difficultyLevel = 1;

	//Music is only for one puzzle, so at start should be off
	audioControl->setMusicPlaying(false);
}

void ARApp::CleanUp()
{
	smartRelease();
	sampleRelease();

	CleanUpFont();

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

	delete audioControl;
	audioControl = NULL;
}

bool ARApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	input_manager_->Update();
	myControls->update();
	
	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);
	
	for (int c = 0; c < NUMBER_OF_PIECES; c++)
		isMarkerVisable[c] = false;
	

	switch (currentGameState)
	{
	case SPLASH_SCREEN:

		//For the animation to move it needs to know how much time has elapsed between frames
		background->update(fps_);

		if (myControls->crossPressed())
			currentGameState = MAIN_MENU;

		break;


	case MAIN_MENU:

		background->update(fps_);
		
		if (myControls->crossPressed())
		{
			//If the variables aren't reset, puzzle might auto complete sections
			tutorialPuzzle->reset();
			myControls->resetDifferences();

			for (int c = 0; c < NUMBER_OF_PIECES; c++)
				markerY[c] = 0;

			currentGameState = TUTORIAL_LEVEL;
		}

		if (myControls->circlePressed())
		{
			monsterPuzzle->reset();
			myControls->resetDifferences();

			for (int c = 0; c < NUMBER_OF_PIECES; c++)
				markerY[c] = 0;

			currentGameState = LEVEL_ONE;

			audioControl->setMusicPlaying(true);
		}

		if (myControls->triangleButtonPressed())
		{
			difficultyLevel += 1;

			if (difficultyLevel > 3)
				difficultyLevel = 1;
		}


		break;

	case TUTORIAL_LEVEL:

		markerY[myControls->getCurrentPiece()] = myControls->getDifference();

		//To make sure it will only map object to original marker, check if the pieces are
		//still to be solved
		if (sampleIsMarkerFound(MARKER1) && tutorialPuzzle->areOneAndTwoSolved != true)
			markerFound(MARKER1, marker_transform, tutorialPuzzle, isMarkerVisable[MARKER1]);

		if (sampleIsMarkerFound(MARKER3) && tutorialPuzzle->areTwoAndThreeSolved != true)
			markerFound(MARKER3, marker_transform, tutorialPuzzle, isMarkerVisable[MARKER3]);

		if (sampleIsMarkerFound(MARKER2))
		{
			markerFound(MARKER2, marker_transform, tutorialPuzzle, isMarkerVisable[MARKER2]);

			//To ensure the pieces stick to new marker, new update type is called, and
			//bool that determines if it should be rendered is set to true
			if (tutorialPuzzle->areOneAndTwoSolved == true)
			{
				sampleGetTransform(MARKER2, &marker_transform);
				tutorialPuzzle->updateWhenSolved(&marker_transform, &marker_transform, markerY[MARKER2], MARKER1, MARKER2);
				isMarkerVisable[MARKER1] = true;
			}

			if (tutorialPuzzle->areTwoAndThreeSolved == true)
			{
				sampleGetTransform(MARKER2, &marker_transform);
				tutorialPuzzle->updateWhenSolved(&marker_transform, &marker_transform, markerY[MARKER2], MARKER3, MARKER2);
				isMarkerVisable[MARKER3] = true;
			}

			//One both pieces are attatched, puzzle is solved and win screen is shown
			if (tutorialPuzzle->areOneAndTwoSolved == true && 
				tutorialPuzzle->areTwoAndThreeSolved == true)
					currentGameState = WIN_SCREEN;

			//Test new positions and heights
			tutorialPuzzle->hasThePuzzleBeenSolved();
		}

		if (!sampleIsMarkerFound(MARKER1) && !sampleIsMarkerFound(MARKER2) && !sampleIsMarkerFound(MARKER3))
		{
			counter += 1 / fps_;

			//If no marker is in view for set time, game over occurs
			if (counter > TIME_OUT)
			{
				currentGameState = LOSE_SCREEN;
				counter = 0;
			}
		}
		else
			counter = 0; //Reset timer if marker is found

		//return to main menu
		if (myControls->startPressed())
			currentGameState = MAIN_MENU;
	

		break;

	case LEVEL_ONE:
		markerY[myControls->getCurrentPiece()] = myControls->getDifference();

		if (sampleIsMarkerFound(MARKER1) && monsterPuzzle->areOneAndTwoSolved != true)
				markerFound(MARKER1, marker_transform, monsterPuzzle, isMarkerVisable[MARKER1]);

		if (sampleIsMarkerFound(MARKER3) && monsterPuzzle->areTwoAndThreeSolved != true)
			markerFound(MARKER3, marker_transform, monsterPuzzle, isMarkerVisable[MARKER3]);

		if (sampleIsMarkerFound(MARKER2))
		{
			markerFound(MARKER2, marker_transform, monsterPuzzle, isMarkerVisable[MARKER2]);
			if (monsterPuzzle->areOneAndTwoSolved == true)
			{
				sampleGetTransform(MARKER2, &marker_transform);
				monsterPuzzle->updateWhenSolved(&marker_transform, &marker_transform, markerY[MARKER2], MARKER1, MARKER2);
				isMarkerVisable[MARKER1] = true;
			}

			if (monsterPuzzle->areTwoAndThreeSolved == true)
			{
				sampleGetTransform(MARKER2, &marker_transform);
				monsterPuzzle->updateWhenSolved(&marker_transform, &marker_transform, markerY[MARKER2], MARKER3, MARKER2);
				isMarkerVisable[MARKER3] = true;
			}


			if (monsterPuzzle->areOneAndTwoSolved == true && 
				monsterPuzzle->areTwoAndThreeSolved == true)
					currentGameState = WIN_SCREEN;

			monsterPuzzle->hasThePuzzleBeenSolved();
		}

		if (!sampleIsMarkerFound(MARKER1) && !sampleIsMarkerFound(MARKER2) && !sampleIsMarkerFound(MARKER3))
		{
			counter += 1/fps_;

			if (counter > TIME_OUT)
			{
				currentGameState = LOSE_SCREEN;
				counter = 0;
			}
		}
		else
			counter = 0;

		if (myControls->startPressed())
			currentGameState = MAIN_MENU;

		
		break;


	case WIN_SCREEN:
		
		background->update(fps_);

		if (myControls->crossPressed())
			currentGameState = MAIN_MENU;

		break;


	case LOSE_SCREEN:
		
		background->update(fps_);

		if (myControls->crossPressed())
			currentGameState = MAIN_MENU;

		break;

	default:
		break;
	}

	sampleUpdateEnd(dat);
	return true;
}

void ARApp::Render()
{
	dat = sampleRenderBegin();

	projection_matrix = platform_.PerspectiveProjectionFov(myCamera->getCameraFOV(), (float)platform_.width() / (float)platform_.height(), myCamera->getNearPlane(), myCamera->getFarPlane());
	view_matrix.LookAt(myCamera->getCameraEye(), myCamera->getCameraLookat(), myCamera->getCameraUp());

	// REMEMBER AND SET THE PROJECTION MATRIX HERE
	orthoProjMat.OrthographicFrustumGL(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(orthoProjMat);

	switch (currentGameState)
	{
	case SPLASH_SCREEN:
		sprite_renderer_->Begin(false);

			background->onRender(sprite_renderer_);

		sprite_renderer_->End();

		break;

	case MAIN_MENU:
		sprite_renderer_->Begin(false);
		
			background->onRender(sprite_renderer_);
			buttons[0]->onRender(sprite_renderer_);
			buttons[1]->onRender(sprite_renderer_);
			buttons[2]->onRender(sprite_renderer_);

		sprite_renderer_->End();

		break;

	case TUTORIAL_LEVEL:

		sprite_renderer_->Begin(false);

		//Render the camera view first
		if (dat->currentImage)
		{
			cameraTexture.set_texture(dat->currentImage->tex_yuv);

			myCamera->updateTexture(&cameraTexture);
			sprite_renderer_->DrawSprite(myCamera->getCameraSprite());

		}

		sprite_renderer_->End();

		//Reset view matrix
		viewMatrix.SetIdentity();
		// SET VIEW AND PROJECTION MATRIX HERE
		renderer_3d_->set_projection_matrix(myCamera->getProjectionMatrix());
		renderer_3d_->set_view_matrix(viewMatrix);


		//// Begin rendering 3D meshes, don't clear the frame buffer
		renderer_3d_->Begin(false);

		//Check if the markers are in view, and only render their pieces if they are
		for (int c = 0; c < NUMBER_OF_PIECES; c++)
		{
			if (isMarkerVisable[c])
				tutorialPuzzle->onRender(renderer_3d_, c);
		}

		renderer_3d_->End();

		break;

	case LEVEL_ONE:

		sprite_renderer_->Begin(false);
			//// DRAW CAMERA FEED SPRITE HERE
			if (dat->currentImage)
			{
				cameraTexture.set_texture(dat->currentImage->tex_yuv);

				myCamera->updateTexture(&cameraTexture);
				sprite_renderer_->DrawSprite(myCamera->getCameraSprite());
			
			}

		sprite_renderer_->End();

		viewMatrix.SetIdentity();
		// SET VIEW AND PROJECTION MATRIX HERE
		renderer_3d_->set_projection_matrix(myCamera->getProjectionMatrix());
		renderer_3d_->set_view_matrix(viewMatrix);


		//Begin rendering 3D meshes, don't clear the frame buffer
		renderer_3d_->Begin(false);
		
			for (int c = 0; c < NUMBER_OF_PIECES; c++)
			{
				if (isMarkerVisable[c])
					monsterPuzzle->onRender(renderer_3d_, c);
			}
			
		renderer_3d_->End();

		break;




	case WIN_SCREEN:
		sprite_renderer_->Begin(false);

			background->onRender(sprite_renderer_);

		sprite_renderer_->End();


		break;


	case LOSE_SCREEN:
		sprite_renderer_->Begin(false);

			background->onRender(sprite_renderer_);

		sprite_renderer_->End();


		break;

	default:
		break;
	}

	RenderOverlay();
	sampleRenderEnd();
}


void ARApp::RenderOverlay()
{
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_.OrthographicFrustum(0.0f, platform_.width(), 0.0f, platform_.height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}


void ARApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void ARApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void ARApp::DrawHUD()
{
	if(font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);


		switch (currentGameState)
		{
		case SPLASH_SCREEN:
			if (font_)
				font_->RenderText(sprite_renderer_, gef::Vector4(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, -0.9f), 5.0f, 0xff000000, gef::TJ_CENTRE, "PERSPECTIVE");

			break;


		case MAIN_MENU:
			if (font_)
			{
				font_->RenderText(sprite_renderer_, gef::Vector4(DISPLAY_WIDTH / 2, 100, -0.9f), 0.7f, 0xff000000, gef::TJ_CENTRE, "Press X for Level One");
				font_->RenderText(sprite_renderer_, gef::Vector4(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, -0.9f), 0.7f, 0xff000000, gef::TJ_CENTRE, "Press O for Level Two");
				font_->RenderText(sprite_renderer_, gef::Vector4(DISPLAY_WIDTH / 2, 450, -0.9f), 0.7f, 0xff000000, gef::TJ_CENTRE, "Press Triangle to Change Difficulty Level: %i", difficultyLevel);

			}
			break;

		case TUTORIAL_LEVEL:
			if (font_)
			{
				font_->RenderText(sprite_renderer_, gef::Vector4(10, 10, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Current Piece: %i", myControls->getCurrentPiece());
				font_->RenderText(sprite_renderer_, gef::Vector4(10, 30, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Press Circle To Cycle Between Objects", myControls->getCurrentPiece());
				font_->RenderText(sprite_renderer_, gef::Vector4(10, 50, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Use left joystick to move selected object up and down", myControls->getCurrentPiece());
				font_->RenderText(sprite_renderer_, gef::Vector4(10, 70, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Move markers by Hand till puzzles are solved", myControls->getCurrentPiece());
			}

			break;

		case LEVEL_ONE:
			if (font_)
			{
				font_->RenderText(sprite_renderer_, gef::Vector4(10, 10, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Current Piece: %i", myControls->getCurrentPiece());
				font_->RenderText(sprite_renderer_, gef::Vector4(10, 30, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Press Circle To Cycle Between Objects", myControls->getCurrentPiece());
				font_->RenderText(sprite_renderer_, gef::Vector4(10, 50, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Use left joystick to move selected object up and down", myControls->getCurrentPiece());
				font_->RenderText(sprite_renderer_, gef::Vector4(10, 70, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Move markers by Hand till puzzles are solved", myControls->getCurrentPiece());
			}

			break;


		case WIN_SCREEN:
			if (font_)
				font_->RenderText(sprite_renderer_, gef::Vector4(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, -0.9f), 5.0f, 0xff000000, gef::TJ_CENTRE, "YOU WIN");
			break;


		case LOSE_SCREEN:
			if (font_)
				font_->RenderText(sprite_renderer_, gef::Vector4(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, -0.9f), 5.0f, 0xff000000, gef::TJ_CENTRE, "YOU LOSE");

			break;

		default:


			break;
		}
	
	}
}

void ARApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}


void ARApp::markerFound(int marker, gef::Matrix44 &markerTransform, Puzzle *currentPuzzle, bool &confirmMarkerFound)
{
	sampleGetTransform(marker, &markerTransform);
	confirmMarkerFound = true;
	currentPuzzle->update(&markerTransform, markerY[marker], marker, confirmMarkerFound, difficultyLevel);
}