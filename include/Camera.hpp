#ifndef CAMERA

#define CAMERA

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Camera{
public:
    Camera(int width, int height);
    ~Camera();
    void change_angle(double xpos, double ypos);
    void change_magnification( double xoffset, double yoffset );
    glm::mat4 lookat();
    glm::vec3 cameraFront, cameraPos, cameraUp;
    const float cameraSpeed;
    const float sensitivity;
    float zoom;
private:
    float radius;
    float lastX, lastY;
    float yaw , pitch;
    bool firstMouse;
};
#endif