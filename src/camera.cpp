#include"../include/camera.hpp"
#include<GLFW/glfw3.h>
Camera::Camera(int width, int height):
                                    lastX( width / 2), 
                                    lastY( height / 2 ), 
                                    yaw( -90.0f ),
                                    zoom( 45.0f ),
                                    cameraFront(0.0f, 0.0f, -1.0f),
                                    cameraPos(0.0f, 0.0f, 3.0f),
                                    cameraUp(0.0f, 1.0f, 0.0f),
                                    cameraSpeed(0.03f),
                                    sensitivity(0.08f),
                                    firstMouse( true ){}
void Camera::change_angle(double xpos, double ypos){
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    if( firstMouse ){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    lastX = xpos;
    lastY = ypos;


    xoffset += sensitivity;
    yoffset += sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if( pitch > 89.0f ) pitch = 89.0f;
    if( pitch < -89.0f ) pitch = -89.0f;


    glm::vec3 direction;

    direction.x = cos( glm::radians( yaw ) * cos( glm::radians( pitch )));
    direction.y = sin( glm::radians( pitch ));
    direction.z = sin( glm::radians( yaw  )) * cos( glm::radians( pitch ));

    cameraFront = glm::normalize( direction);
}
void Camera::change_magnification(double xoffset , double yoffset ){
    zoom -= ( float ) yoffset;
    if( zoom < 1.0f ) zoom = 1.0f;
    if( zoom > 45.0f ) zoom = 45.0f;
}

void Camera::change_position( glm::vec3 change ){

}
glm::mat4 Camera::lookat(){
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp );
}

Camera::~Camera(){}