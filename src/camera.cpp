// Purpose: Camera class implementation'
#include<Camera.hpp>
#include<iostream>
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
                                    isRightClicked( false ),
                                    firstMouse( true ),
                                    isLeftClicked( false ){
    projection = glm::perspective( glm::radians( zoom ) , float( vWidth )/ vHeight, 0.1f, 10000.0f );
    view = glm::lookAt(cameraPos, { 0.0f, 0.0f, 0.0f}, cameraUp );
}

void Camera::change_angle(double xpos, double ypos){

    if( firstMouse ){
        vStart = getArcballVector(xpos, ypos);
        firstMouse = false;
    } else {
        glm::vec3 vEnd = getArcballVector(xpos, ypos);
        float angle = 2.0f * acos(std::min(1.0f, glm::dot(vStart, vEnd))); // angle of rotation around rotation axis
        glm::vec3 rotAxis = glm::cross(vStart, vEnd);
        glm::mat3 camera2object = glm::inverse(glm::mat3(view)); // Assuming 'view' is your view matrix
        glm::vec3 rotAxisObj = camera2object * rotAxis;
        view = glm::rotate(view, angle, rotAxisObj);
        vStart = vEnd;
    }

    lastX = xpos;
    lastY = ypos;
}



glm::vec3 Camera::getArcballVector(double xpos, double ypos) {
    glm::vec3 P = glm::vec3(1.0*xpos/vWidth*2 - 1.0,
                            1.0*ypos/vHeight*2 - 1.0,
                            0);
    P.y = -P.y;
    float OP_squared = P.x*P.x + P.y*P.y;
    if (OP_squared <= 1*1)
        P.z = sqrt(1*1 - OP_squared);  // Pythagore
    else
        P = glm::normalize(P);  // nearest point
    return P;
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