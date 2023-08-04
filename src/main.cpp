#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<shader.hpp>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<camera.hpp>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


float alphaValue = 0.1f; // Set the desired alpha value (transparency) here

float vertices[] = {
    //vertex         //color (blue in RGB)       //alpha value
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 1
    0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 2
    0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 3
    0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 4
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 5
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 6

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 7
    0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 8
    0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 9
    0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 10
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 11
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 12

    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 13
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 14
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 15
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 16
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 17
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 18

    0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 19
    0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 20
    0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 21
    0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 22
    0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    alphaValue, // Vertex 23
    0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    alphaValue  // Vertex 24
};


unsigned int indices[] = {
    0, 1, 3, 
    1, 2, 3
};


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

    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glfwSetScrollCallback( window, scroll_callback );

    if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to initialize GLAD" << std :: endl;
        return -1;
    }

    //initialize texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./resources/container.jpg", &width, &height, &nrChannels, 0 );


    // unsigned int texture, texture2;

    // glGenTextures(1, &texture);
    // glGenTextures( 1, &texture2);

    // glBindTexture( GL_TEXTURE_2D, texture);

    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // if( data ){
    //     glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    
    // }
    // else{
    //     std::cout<<"Failed to load teture"<<std::endl;
    // }

    // glBindTexture( GL_TEXTURE_2D, texture2);

    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // unsigned char * data2 = stbi_load( "./resources/awesomeface.png" ,&width, &height, &nrChannels, 0 );
    // if( data2 ){
    //     glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2 );
    //     glGenerateMipmap( GL_TEXTURE_2D);
    // }

    // //free the memory
    // stbi_image_free( data );
    // stbi_image_free( data2 );

    Shader shaderClass("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    unsigned int VBO , VAO;
    glGenVertexArrays( 1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray( VAO );

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices) , vertices, GL_STATIC_DRAW);

    // unsigned int EBO;
    // glGenBuffers( 1, &EBO );
    // glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW);

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof( float ), ( void * )0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof( float ), ( void* )( 3 * sizeof( float )));
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);

    // shaderClass.use();
    // shaderClass.setInt( "texture1", 0);
    // shaderClass.setInt( "texture2", 1);

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

    glEnable( GL_DEPTH_TEST);
    glEnable( GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    while( !glfwWindowShouldClose( window ) ){
        processInput( window );
        

        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        // glActiveTexture( GL_TEXTURE0);
        // glBindTexture( GL_TEXTURE_2D, texture);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture( GL_TEXTURE_2D, texture2 );
        
        shaderClass.use();
        unsigned int transformLoc;

        view = c.lookat();

        transformLoc = glGetUniformLocation( shaderClass.ID, "view");
        glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr( view ));

        transformLoc = glGetUniformLocation( shaderClass.ID, "projection");

        projection = glm::perspective( glm::radians( c.zoom ) , 800.0f/ 600.0f, 0.1f, 100.0f );
        glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr(projection));


        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray( VAO );
        // float time = glfwGetTime();
        // for( int i = 0 ; i < 10; i++){
            transformLoc = glGetUniformLocation( shaderClass.ID, "model");
            // glm::mat4 newModel = glm::mat4(1.0f );

            // newModel = glm::translate( model, cubePositions[i]);
            // newModel = glm::rotate( newModel, time, cubePositions[i]);
            
            glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr(model) );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        // }
        // glDrawArrays( GL_TRIANGLES, 0, 36 );

        // glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glDeleteVertexArrays( 1, &VAO);
    glDeleteBuffers( 1, &VBO );
    // glDeleteBuffers( 1, &EBO );
    // glDeleteProgram( shaderProgram );
    shaderClass.close();
    glfwTerminate();
    return 0;
}