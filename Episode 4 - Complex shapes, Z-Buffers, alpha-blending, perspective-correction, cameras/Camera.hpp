#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>

class Camera
{
public:
	enum Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
private:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
protected:
	void updateCameraVectors();
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX,
		   float upY, float upZ, float yaw, float pitch);
	void ProcessMouseMovement(float xoffset, float yoffset, float W, float H, bool constrainPitch = true);
	void ProcessKeyboard(Movement direction, float deltaTime);
	glm::mat4 GetViewMatrix();
	float getZoom();
	void reset();

	static const float YAW;
	static const float PITCH;
	static const float SPEED;
	static const float SENSITIVITY;
	static const float ZOOM;
	float getYaw() const;
	void setYaw(float value);
	float getPitch() const;
	void setPitch(float value);
	float getMovementSpeed() const;
	void setMovementSpeed(float value);
	float getMouseSensitivity() const;
	void setMouseSensitivity(float value);
};

#endif // CAMERA_HPP
