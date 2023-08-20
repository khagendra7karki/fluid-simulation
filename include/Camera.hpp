#ifndef CAMERA

#define CAMERA

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<utility.hpp>

class Camera{
public:
    Camera(int width, int height);
    ~Camera();
    void change_angle(double xpos, double ypos);
    void change_magnification( double xoffset, double yoffset );
    void changeViewPort( unsigned int width , unsigned int height );
    
    glm::vec3 getArcballVector( double xpos, double ypos );

    glm::vec3 vStart, vEnd;

    glm::vec3 cameraFront, cameraPos, cameraUp;
    const float sensitivity;
    float zoom;
    bool isRightClicked, isLeftClicked;
    bool firstMouse;
    glm::mat4 projection; //camera saves the projection matrix
    glm::mat4 view;       // camera saves the view matrix 
private:
    unsigned int vHeight, vWidth;
    float radius;
    float lastX, lastY;
    float yaw , pitch;
};
#endif