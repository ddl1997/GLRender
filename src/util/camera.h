#ifndef CAMERA_H
#define CAMERA_H

#include "global.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float MINFOV = 1.0f;
const float MAXFOV = 60.0f;
const float NEAR = 0.1f;
const float FAR = 1000.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Near;
    float Far;
    // euler Angles
    float Yaw; // Æ«º½½Ç£¨ÈÆyÖá£©
    float Pitch; // ¸©Ñö½Ç£¨ÈÆxÖá£©
    // camera options
    float MovementSpeed; // ÒÆ¶¯ËÙ¶È
    float MouseSensitivity; // Êó±ê¿ØÖÆÊÓ½ÇÒÆ¶¯µÄÁéÃô¶È
    float Zoom;

    float width, height;

public:
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float near = NEAR, float far = FAR) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Near = near;
        Far = far;
        updateCameraVectors();
        initScreenSize();
    }

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float near = NEAR, float far = FAR) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
        initScreenSize();
    }

    glm::vec3 getPosition()
    {
        return Position;
    }

    void setWidth(float width) { this->width = width; }

    void setHeight(float height) { this->height = height; }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, WorldUp);
    }

    glm::mat4 GetPerspectiveProjectionMatrix()
    {
        return glm::perspective(glm::radians(Zoom), width / height, Near, Far);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    glm::mat4 getViewMatrix()
    {
        return calculate_lookAt_matrix(Position, Position + Front, WorldUp);
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

    glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);

    void initScreenSize()
    {
        width = WIDTH;
        height = HEIGHT;
    }
};
#endif