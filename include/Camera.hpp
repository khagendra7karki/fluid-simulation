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

    glm::vec3 cameraFront, cameraPos, cameraUp;
    const float sensitivity;
    float zoom;
    bool isMouseClicked;
    
    glm::mat4 projection; //camera saves the projection matrix
    glm::mat4 view;       // camera saves the view matrix 
private:
    float radius;
    float lastX, lastY;
    float yaw , pitch;
    bool firstMouse;
};
#endif