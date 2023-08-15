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
}

void mouse_callback( GLFWwindow* window, double xpos, double ypos ){
    c.change_angle( xpos, ypos );
}


void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ){
    c.change_magnification( xoffset, yoffset );
}


int main(){ 

    Sphere s(particleRadius, 36, 18,   {0.0f, 0.0f, 1.0f, 1.5f }) ;
    s.printSelf();
    const float* vertices = s.getInterleavedVertices();
    const unsigned int sizeOfVertices = s.getInterleavedVertexSize();

    const unsigned int* indices = s.getIndices();
    const unsigned int indicesSize = s.getIndexSize();
    const unsigned int indexCount = s.getIndexCount();
    
    Fluid f;

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


    Shader shaderClass("./shaders/sphere_vertex.glsl", "./shaders/sphere_fragment.glsl");

    unsigned int VBO , VAO, EBO;
    glGenVertexArrays( 1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray( VAO );
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, sizeOfVertices , vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_DYNAMIC_DRAW );


    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof( float ), ( void * )0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof( float ), ( void* )( 3 * sizeof( float )));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof( float ), ( void* )( 6 * sizeof( float )));
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);


    glEnable( GL_DEPTH_TEST);
    glEnable( GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    

    double startTime = glfwGetTime();
    double frameStartTime = startTime;
    float frameRate= 0;
    unsigned int frames=0;
    while( !glfwWindowShouldClose( window ) ){
        processInput( window );
        

        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        
        shaderClass.use();

        view = c.lookat();

        shaderClass.setMat4( "view", view );

        projection = glm::perspective( glm::radians( c.zoom ) , 800.0f/ 600.0f, 1.0f, 100.0f );
        shaderClass.setMat4( "projection", projection );


        glBindVertexArray( VAO );

        f.simulate();
        for( int i = 0 ; i < f.mParticles.size() ; i++ ){
            shaderClass.setMat4( "model", glm::translate( model, {f.mParticles[i].mPosition.x, f.mParticles[i].mPosition.y, f.mParticles[i].mPosition.z} ));
            glDrawElements( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0 );
            
        }

        glfwSwapBuffers( window );
        glfwPollEvents();
        
        double endTime = glfwGetTime();
        double deltaTime = endTime - startTime;
        startTime = endTime;
        frames++;
        if( endTime - frameStartTime  > 60.0 ){
            std::cout<<"The current frame rate is "<<frames <<std::endl;
            frames = 0;
            frameStartTime = endTime;

        }
        frameRate = ( 1.0 / deltaTime );
        std::cout<<"The instantaneos frame rate is "<<frameRate<<std::endl;
        std::cout<<"Time elapsed since last render "<<deltaTime <<std::endl;
    }

    glDeleteVertexArrays( 1, &VAO);
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );


    shaderClass.close();
    glfwTerminate();
    return 0;
}