// Purpose: Camera class implementation'
#include<Camera.hpp>
Camera::Camera(int width, int height): vWidth( width ),
                                    vHeight( height ),
                                    radius(1.0f),
                                    lastX( width / 2), 
                                    lastY( height / 2 ), 
                                    yaw( -90.0f ),
                                    zoom( 30.0f ),
                                    cameraFront(0.0f, 0.0f, -1.0f),
                                    cameraPos(0.0f, 0.0f, 1.0f),
                                    cameraUp(0.0f, 1.0f, 0.0f),
                                    sensitivity(0.28f),
                                    isMouseClicked( false ){
    projection = glm::perspective( glm::radians( zoom ) , float( vWidth )/ vHeight, 0.1f, 10000.0f );
    view = glm::lookAt(cameraPos, { 0.0f, 0.0f, 0.0f}, cameraUp );
}


void Camera::change_angle(double xpos, double ypos){

    
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


    //create the view matrix based on the new camera position
    view = glm::lookAt(cameraPos, { 0.0f, 0.0f, 0.0f}, cameraUp );

}
void Camera::change_magnification(double xoffset , double yoffset ){
    zoom -= ( float ) yoffset;
    if( zoom < 1.0f ) zoom = 1.0f;
    if( zoom > 45.0f ) zoom = 45.0f;
    
    //create the view matrix based on the new camera position
    projection = glm::perspective( glm::radians( zoom ) , float( vWidth)/ vHeight, 0.1f, 10000.0f );
}

void Camera::changeViewPort( unsigned int width, unsigned int height){
    vHeight = height;
    vWidth = width;
}


Camera::~Camera(){}