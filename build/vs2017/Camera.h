#pragma
#define DISPLAY_WIDTH 960
#define DISPLAY_HEIGHT 544

#include <maths/vector4.h>
#include <maths/math_utils.h>
#include "graphics\sprite.h"
#include <maths\matrix44.h>
#include <platform/vita/graphics/texture_vita.h>
#include <sony_sample_framework.h>

namespace gef
{
	class Sprite;
	class TextureVita;
}

class Camera
{
public:
	Camera(float);
	~Camera();

	gef::Vector4 getCameraEye();
	gef::Vector4 getCameraLookat();
	gef::Vector4 getCameraUp();

	float getCameraFOV();
	float getNearPlane();
	float getFarPlane();

	gef::Sprite getCameraSprite();
	gef::Matrix44 getProjectionMatrix();

	void updateTexture(gef::TextureVita*);

private:
	gef::Vector4 camera_eye_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;

	float camera_fov_, near_plane_, far_plane_;
	float verticalScaleFactor, displayAspectRatio, cameraImageAspectRatio;

	gef::Sprite cameraFeed;

	gef::Matrix44 PerspectiveProjectionFov, scaleMatrix, finalProjectionMatrix;

};


