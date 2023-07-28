#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<shader.hpp>
#include<iostream>
#include<cmath>

float vertices[] = {
// positions // colors // texture coords
0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
};

unsigned int indices[] = {
    0, 1, 3, 
    1, 2, 3
};



void framebuffer_size_callback( GLFWwindow* window, int width, int height){
    glViewport( 0, 0, width , height);
}

void processInput( GLFWwindow *window ){
    if( glfwGetKey( window, GLFW_KEY_ESCAPE) == GLFW_PRESS ){
        glfwSetWindowShouldClose(window, true);
    }
}

int main(){ 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow( 800, 600, "SPH", NULL, NULL);

    if( window == NULL){
        std::cout<<"Failded to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent( window );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback);

    if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to initialize GLAD" << std :: endl;
        return -1;
    }

    //initialize texture
    int width, height, nrChannels, width2, height2, nrChannels2;
    unsigned char *data = stbi_load("./resources/container.jpg", &width, &height, &nrChannels, 0 );
    unsigned char * data2 = stbi_load( "./resources/awesomeface.png" ,&width2, &height2, &nrChannels2, 0 );

    unsigned int texture, texture2;

    glGenTextures(1, &texture);
    glGenTextures( 1, &texture2);
    glBindTexture( GL_TEXTURE_2D, texture);
    glBindTexture( GL_TEXTURE_2D, texture2);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if( data ){
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    
    }
    else{
        std::cout<<"Failed to load teture"<<std::endl;
    }

    if( data2 ){
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2 );
        glGenerateMipmap( GL_TEXTURE_2D);
    }

    //free the memory
    stbi_image_free( data );

    Shader shaderClass("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    unsigned int VBO , VAO;
    glGenVertexArrays( 1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray( VAO );

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices) , vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers( 1, &EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW);

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void * )0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* )( 3 * sizeof( float )));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void * )( 6 * sizeof( float )));
    glEnableVertexAttribArray( 2 );

    glBindVertexArray(0);

    shaderClass.use();
    shaderClass.setInt( "texture1", 0);
    shaderClass.setInt( "texture2", 1);

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

    while( !glfwWindowShouldClose( window ) ){
        processInput( window );


        // float timeValue = glfwGetTime();
        // float greenValue = ( sin(timeValue ) / 2.0f ) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation( shaderProgram, "ourColor");

    
        
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shaderClass.use();

        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glActiveTexture( GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture( GL_TEXTURE_2D, texture2 );
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray( VAO );

        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glDeleteVertexArrays( 1, &VAO);
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
    // glDeleteProgram( shaderProgram );
    shaderClass.close();
    glfwTerminate();
    return 0;
}