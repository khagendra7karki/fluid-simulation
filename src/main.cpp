#define STB_IMAGE_IMPLEMENTATION

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<Shader.hpp>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<Camera.hpp>
#include<Sphere.h>
#include<utility.hpp>
#include<Fluid.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


float alphaValue = 0.1f; // Set the desired alpha value (transparency) here

const float fluidVolume = 1000 * MASS / REST_DENSITY;
const float particleDiameter = powf( fluidVolume, 1.0f / 3.0f ) / 10;
const float particleRadius = particleDiameter / 2.0f;



Camera c( WINDOW_WIDTH, WINDOW_HEIGHT );

void framebuffer_size_callback( GLFWwindow* window, int width, int height){
    glViewport( 0, 0, width , height);
}




void processInput( GLFWwindow *window ){
    if( glfwGetKey( window, GLFW_KEY_SPACE) == GLFW_PRESS ){
        glfwSetWindowShouldClose(window, true);
    }

    const float cameraSpeed = 0.05f;
    if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS)
        c.cameraPos += c.cameraSpeed * c.cameraFront;
    if( glfwGetKey ( window, GLFW_KEY_S) == GLFW_PRESS )
        c.cameraPos -= c.cameraSpeed * c.cameraFront ;
    
    if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
        c.cameraPos -= glm::normalize( glm::cross( c.cameraFront, c.cameraUp )) * c.cameraSpeed;
    
    if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
        c.cameraPos += glm::normalize( glm::cross( c.cameraFront, c.cameraUp )) * c.cameraSpeed;
    
}

void mouse_callback( GLFWwindow* window, double xpos, double ypos ){
    c.change_angle( xpos, ypos );
}


void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ){
    c.change_magnification( xoffset, yoffset );

}


int main(){ 

    Sphere s(particleRadius, 18, 9,  3, {0.0f, 0.0f, 1.0f, 1.5f }) ;
    const float* vertices = s.getInterleavedVertices();
    const unsigned int sizeOfVertices = s.getInterleavedVertexSize();

    const unsigned int* indices = s.getIndices();
    const unsigned int indicesSize = s.getIndexSize();
    const unsigned int indexCount = s.getIndexCount();
    

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glm::mat4 view;
    glm::mat4 model = glm::mat4( 1.0f );
    glm::mat4 projection;

    GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Fluid Simulation", NULL, NULL);

    if( window == NULL){
        std::cout<<"Failded to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent( window );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback);
    glfwSetCursorPosCallback( window, mouse_callback );

    // glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glfwSetScrollCallback( window, scroll_callback );

    if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to initialize GLAD" << std :: endl;
        return -1;
    }

    //initialize texture
    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("./resources/container.jpg", &width, &height, &nrChannels, 0 );




    Shader shaderClass("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    unsigned int VBO , VAO, EBO;
    glGenVertexArrays( 1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray( VAO );
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, sizeOfVertices , vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_DYNAMIC_DRAW );


    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof( float ), ( void * )0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof( float ), ( void* )( 3 * sizeof( float )));
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);


    glEnable( GL_DEPTH_TEST);
    glEnable( GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    

    double startTime = glfwGetTime();
    unsigned int frameRate= 0;
    while( !glfwWindowShouldClose( window ) ){
        processInput( window );
        

        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        
        shaderClass.use();

        view = c.lookat();

        shaderClass.setMat4( "view", view );

        projection = glm::perspective( glm::radians( c.zoom ) , 800.0f/ 600.0f, 0.1f, 100.0f );
        shaderClass.setMat4( "projection", projection );


        glBindVertexArray( VAO );
        // transformLoc = glGetUniformLocation( shaderClass.ID, "model");
        
        // glDrawElements( GL_TRIANGLES,indexCount, GL_UNSIGNED_INT,0 );
        // glDrawArrays( GL_TRIANGLES, 0, vertexCount );

        for( int i = 0 ; i < 10 ; i++ ){
            glm::mat4 i_newModel(1.0f);
            i_newModel = glm::translate( i_newModel, glm::vec3( i * particleDiameter, 0.0f, 0.0f ));
            for( int j = 0; j < 10; j++){
                glm::mat4 j_newModel;
                j_newModel = glm::translate( i_newModel, glm::vec3( 0.0f,j * particleDiameter, 0.0f ));
                for( int k = 0 ; k < 10; k++){
                    j_newModel = glm::translate( j_newModel, glm::vec3( 0.0f, 0.0f, particleDiameter ));
                    shaderClass.setMat4( "model", j_newModel );
                    glDrawElements( GL_TRIANGLES,indexCount, GL_UNSIGNED_INT,0 );
                }
            }
        }

        glfwSwapBuffers( window );
        glfwPollEvents();
        
        double endTime = glfwGetTime();
        double deltaTime = endTime - startTime;
        
        frameRate = (unsigned int)( 1.0 / deltaTime );

    }

    glDeleteVertexArrays( 1, &VAO);
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );


    shaderClass.close();
    glfwTerminate();
    return 0;
}