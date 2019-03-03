#include "Camera.h"

Camera::Camera(float fov)
{
	// initialise the camera settings
	camera_eye_ = gef::Vector4(-50.0f, 20.0f, 200.0f);
	camera_lookat_ = gef::Vector4(50.0f, 20.0f, 0.0f);
	camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 1000.f;


	//Initialise camera feed settings
	cameraImageAspectRatio = (float)CAMERA_WIDTH / (float)CAMERA_HEIGHT;
	displayAspectRatio = (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT;
	verticalScaleFactor = displayAspectRatio / cameraImageAspectRatio;

	//The vertical scale factor is to ensure the top and bottom are not clipped off
	//due to the different dimensions of the camera and screen
	cameraFeed.set_width(2.0f);
	cameraFeed.set_height(2.0f * verticalScaleFactor);
	cameraFeed.set_position(0, 0, 1);

	scaleMatrix.Scale(gef::Vector4(1, verticalScaleFactor, 1, 1));

	PerspectiveProjectionFov.PerspectiveFovGL(fov, cameraImageAspectRatio, near_plane_, far_plane_);

	finalProjectionMatrix = PerspectiveProjectionFov * scaleMatrix;
}

Camera::~Camera()
{
}

//Getters and Setters
void Camera::updateTexture(gef::TextureVita *tex)
{
	cameraFeed.set_texture(tex);
}
gef::Sprite Camera::getCameraSprite()
{
	return cameraFeed;
}
gef::Vector4 Camera::getCameraEye()
{
	return camera_eye_;
}
gef::Vector4 Camera::getCameraLookat()
{
	return camera_lookat_;
}
gef::Vector4 Camera::getCameraUp()
{
	return camera_up_;
}
float Camera::getCameraFOV()
{
	return camera_fov_;
}
float Camera::getNearPlane()
{
	return near_plane_;
}
float Camera::getFarPlane()
{
	return far_plane_;
}
gef::Matrix44 Camera::getProjectionMatrix()
{
	return finalProjectionMatrix;
}