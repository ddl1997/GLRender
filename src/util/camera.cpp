#include "camera.h"

// 处理键盘按键事件
void Camera::ProcessKeyboard(Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == Movement::FORWARD)
        Position += Front * velocity;
    if (direction == Movement::BACKWARD)
        Position -= Front * velocity;
    if (direction == Movement::LEFT)
        Position -= Right * velocity;
    if (direction == Movement::RIGHT)
        Position += Right * velocity;
    if (direction == Movement::UP)
        Position += WorldUp * velocity; // 在世界坐标系下的上升
    if (direction == Movement::DOWN)
        Position -= WorldUp * velocity; // 在世界坐标系下的下降
    //Position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)
}

// 处理鼠标移动事件
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// 处理鼠标滚轮事件
void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < MINFOV)
        Zoom = MINFOV;
    if (Zoom > MAXFOV)
        Zoom = MAXFOV;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

// 计算lookAt矩阵
glm::mat4 Camera::calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
    // 1. Position = known
    // 2. Calculate cameraDirection
    glm::vec3 zaxis = glm::normalize(position - target);
    // 3. Get positive right axis vector
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
    // 4. Calculate camera up vector
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    // Create translation and rotation matrix
    // In glm we access elements as mat[col][row] due to column-major layout
    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    translation[3][0] = -position.x; // Third column, first row
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x; // First column, first row
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; // First column, second row
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; // First column, third row
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;
    // C * a = b，b为世界坐标系下的坐标，a为摄像机坐标系下的坐标，C=[xaxis, yaxis, zaxis]。因为C为正交矩阵，因此可以使用C的转置矩阵(C * CT = E)实现b到a的计算，即a = CT * b
    // Return lookAt matrix as combination of translation and rotation matrix
    return rotation * translation; // Remember to read from right to left (first translation then rotation)
}