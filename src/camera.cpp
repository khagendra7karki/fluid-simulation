// Purpose: Camera class implementation'
#include<Camera.hpp>
#include<GLFW/glfw3.h>
Camera::Camera(int width, int height):radius(2.0f),
                                    lastX( width / 2), 
                                    lastY( height / 2 ), 
                                    yaw( -90.0f ),
                                    zoom( 30.0f ),
                                    cameraFront(0.0f, 0.0f, -1.0f),
                                    cameraPos(0.0f, 0.0f, 2.0f),
                                    cameraUp(0.0f, 1.0f, 0.0f),
                                    cameraSpeed(0.03f),
                                    sensitivity(0.1f),
                                    firstMouse( true ){}
void Camera::change_angle(double xpos, double ypos){

    if( firstMouse ){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;


    xoffset *= sensitivity;
    yoffset *= sensitivity;

    pitch -= yoffset;
    yaw += xoffset;

    if( pitch > 89.0f ) pitch = 89.0f;
    if( pitch < -89.0f ) pitch = -89.0f;



    cameraPos.x = radius * cos( glm::radians( yaw ) * cos( glm::radians( pitch )));
    cameraPos.y = radius * sin( glm::radians( pitch ));
    cameraPos.z = radius * sin( glm::radians( yaw  )) * cos( glm::radians( pitch ));

}
void Camera::change_magnification(double xoffset , double yoffset ){
    zoom -= ( float ) yoffset;
    if( zoom < 1.0f ) zoom = 1.0f;
    if( zoom > 45.0f ) zoom = 45.0f;
}


glm::mat4 Camera::lookat(){
    return glm::lookAt(cameraPos, { 0.0f, 0.0f, 0.0f}, cameraUp );
}

Camera::~Camera(){}