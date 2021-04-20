#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

const float Camera::YAW = 3.14f;
const float Camera::PITCH = 0.0f;
const float Camera::SPEED = 2.5f;
const float Camera::SENSITIVITY = 0.005f;
const float Camera::ZOOM = 45.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: MovementSpeed(SPEED),
	  MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Front = glm::vec3(0.0f, 0.0f, 1.0f);
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}
float Camera::getZoom()
{
	return Zoom;
}

void Camera::reset()
{
	Front = glm::vec3(0.0f, 0.0f, 1.0f);
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = YAW;
	Pitch = PITCH;
	updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX,
	   float upY, float upZ, float yaw, float pitch)
	: MovementSpeed(SPEED),
	  MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}
void Camera::ProcessMouseMovement(float xoffset, float yoffset, float W, float H, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw   += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		/*if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;*/
		if (Pitch >= 3.14)
			Pitch = 3.14;
		if (Pitch <= -3.14)
			Pitch = -3.14;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}
void Camera::ProcessKeyboard(Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}
float Camera::getYaw() const
{
	return Yaw;
}

void Camera::setYaw(float value)
{
	Yaw = value;
}

float Camera::getPitch() const
{
	return Pitch;
}

void Camera::setPitch(float value)
{
	Pitch = value;
}

float Camera::getMovementSpeed() const
{
	return MovementSpeed;
}

void Camera::setMovementSpeed(float value)
{
	MovementSpeed = value;
}

float Camera::getMouseSensitivity() const
{
	return MouseSensitivity;
}

void Camera::setMouseSensitivity(float value)
{
	MouseSensitivity = value;
}

void Camera::updateCameraVectors()
{
	Front = glm::vec3(
		cos(Pitch) * sin(Yaw),
		sin(Pitch),
		cos(Pitch) * cos(Yaw)
	);
	Front = glm::normalize(Front);
	Right = glm::vec3(
				sin(Yaw - 3.14f/2.0f),
				0.0f,
				cos(Yaw - 3.14f/2.0f)
			);
	Right = glm::normalize(Right);
	Up = glm::cross( Right, Front );
	Up = glm::normalize(Up);
}
