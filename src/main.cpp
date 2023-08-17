#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<Shader.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<Camera.hpp>
#include<utility.hpp>
#include<Fluid.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600




Camera c( WINDOW_WIDTH, WINDOW_HEIGHT );
Fluid f;

void framebuffer_size_callback( GLFWwindow* window, int width, int height){
    glViewport( 0, 0, width , height);
    
}


void processInput( GLFWwindow *window ){
    if( glfwGetKey( window, GLFW_KEY_SPACE) == GLFW_PRESS ){
        glfwSetWindowShouldClose(window, true);
    }    
    if( glfwGetKey( window, GLFW_KEY_S) == GLFW_PRESS ){
        f.startSimulation = !f.startSimulation;
        std::cout<<f.startSimulation<<std::endl;
    }

}

void mouse_callback( GLFWwindow* window, double xpos, double ypos ){
    if( c.isMouseClicked )
        c.change_angle( xpos, ypos );
}


void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ){
    c.change_magnification( xoffset, yoffset );
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        c.isMouseClicked = true;
    }
    else if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE ) 
        c.isMouseClicked = false;
}


int main(){ 
    
    const float* vertices = f.sphere.getInterleavedVertices();
    const unsigned int sizeOfVertices = f.sphere.getInterleavedVertexSize();

    const unsigned int* indices = f.sphere.getIndices();
    const unsigned int indicesSize = f.sphere.getIndexSize();
    const unsigned int indexCount = f.sphere.getIndexCount();
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glm::mat4 model = glm::mat4( 1.0f );

    GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Fluid Simulation", NULL, NULL);

    if( window == NULL){
        std::cout<<"Failded to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //make the window the current context of the thread
    glfwMakeContextCurrent( window );

    //set callbacks
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback);
    glfwSetCursorPosCallback( window, mouse_callback );
    glfwSetMouseButtonCallback(window, mouse_button_callback);
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


    //enable the requiried features
    glEnable( GL_DEPTH_TEST);
    glEnable( GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    

    double startTime = glfwGetTime();
    float frameRate= 0;
    while( !glfwWindowShouldClose( window ) ){
        processInput( window );
        

        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        
        shaderClass.use();

        shaderClass.setMat4( "view", c.view );
        shaderClass.setMat4( "projection", c.projection );


        glBindVertexArray( VAO );

        shaderClass.setMat4( "model", model );
        glDrawArrays(GL_LINES, 0, 24);


        //calculate the next position of the fluid
        f.simulate();

        //based on the position of the particles render the particle
        for( int i = 0 ; i < f.mParticles.size() ; i++ ){
            shaderClass.setMat4( "model", glm::translate( model, {f.mParticles[i].mPosition.x, f.mParticles[i].mPosition.y, f.mParticles[i].mPosition.z} ));
            glDrawElements( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0 );
            
        }

        glfwSwapBuffers( window );
        glfwPollEvents();
        
        double endTime = glfwGetTime();
        double deltaTime = endTime - startTime;
        startTime = endTime;
        frameRate = ( 1.0 / deltaTime );
        std::cout<<"The instantaneos frame rate is "<<frameRate<<std::endl;
    }

    glDeleteVertexArrays( 1, &VAO);
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );

    std::cout<<f.mParticles.size()<<std::endl;
    shaderClass.close();
    glfwTerminate();
    return 0;
}