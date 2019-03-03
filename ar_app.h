#pragma

#include "build\vs2017\Camera.h"
#include "build\vs2017\Menu.h"
#include "build\vs2017\Audio.h"
#include "build\vs2017\Controls.h"
#include "build\vs2017\PuzzlePieces.h"
#include "build\vs2017\Puzzle.h"
#include "build\vs2017\animatedMenuItem.h"
#include "build\vs2017\ModelLoader.h"
#include "build\vs2017\StaticMenuItem.h"

#include <system/application.h>
#include <gxm.h>
#include <libdbg.h>
#include <libsmart.h>
#include <graphics/font.h>
#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>
#include <sony_sample_framework.h>
#include <sony_tracking.h>


#define MARKER1 0
#define MARKER2 1
#define MARKER3 2

#define TIME_OUT 2

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class RenderTarget;
}


class ARApp : public gef::Application
{
public:
	ARApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void RenderOverlay();
	void SetupLights();
	void markerFound(int, gef::Matrix44&, Puzzle*, bool&);

	gef::InputManager* input_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;
	Camera *myCamera;
	Audio *audioControl;
	Controls *myControls;
	Puzzle *monsterPuzzle, *tutorialPuzzle;
	Menu *background;
	Menu *buttons[3];
	Menu *buttonHighlight;


	float fps_;
	float counter;
	float markerY[NUMBER_OF_PIECES];
	
	gef::TextureVita cameraTexture;

	gef::Matrix44 orthoProjMat;
	gef::Matrix44 viewMatrix;
	gef::Matrix44 marker_transform, finalMatrix;
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;

	AppData* dat;

	enum GameStates { SPLASH_SCREEN, MAIN_MENU, OPTIONS, LEVEL_ONE, TUTORIAL_LEVEL , WIN_SCREEN, LOSE_SCREEN};
	GameStates currentGameState;

	bool isMarkerVisable[NUMBER_OF_PIECES];
	bool markerInSight[NUMBER_OF_PIECES];

	int difficultyLevel;
};
