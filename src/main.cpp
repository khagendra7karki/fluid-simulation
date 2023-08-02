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


float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    if( glfwGetKey( window, GLFW_KEY_ESCAPE) == GLFW_PRESS ){
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
        c.cameraFront += glm::normalize( glm::cross( c.cameraFront, c.cameraUp )) * c.cameraSpeed;
    
}

void mouse_callback( GLFWwindow* window, double xpos, double ypos ){
    c.change_angle( xpos, ypos );
}


void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ){
    c.change_magnification( xoffset, yoffset );

}


int main(){ 
    // Camera c( 800, 600 );
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f ),
        glm::vec3( -1.5f, -2.2f, -2.5f),
        glm::vec3( -3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3( -1.7f, 3.0f, 7.5f ),
        glm::vec3( 1.3f, -2.0f, -2.75f ),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, -0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, 01.5f )
    };


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glm::mat4 view;
    glm::mat4 model = glm::mat4( 1.0f );
    glm::mat4 projection;

    GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "SPH", NULL, NULL);

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
    int width, height, nrChannels, width2, height2, nrChannels2;
    unsigned char *data = stbi_load("./resources/container.jpg", &width, &height, &nrChannels, 0 );

    unsigned char * data2 = stbi_load( "./resources/awesomeface.png" ,&width2, &height2, &nrChannels2, 0 );

    unsigned int texture, texture2;

    glGenTextures(1, &texture);
    glGenTextures( 1, &texture2);

    glBindTexture( GL_TEXTURE_2D, texture);

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

    glBindTexture( GL_TEXTURE_2D, texture2);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if( data2 ){
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2 );
        glGenerateMipmap( GL_TEXTURE_2D);
    }

    //free the memory
    stbi_image_free( data );
    stbi_image_free( data2 );

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

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void * )0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* )( 3 * sizeof( float )));
    glEnableVertexAttribArray(1);

    // glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void * )( 6 * sizeof( float )));
    // glEnableVertexAttribArray( 2 );

    glBindVertexArray(0);

    shaderClass.use();
    shaderClass.setInt( "texture1", 0);
    shaderClass.setInt( "texture2", 1);

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

    glEnable( GL_DEPTH_TEST);

    const float radius =  10.0f; //directin of the camera from the world
    

    while( !glfwWindowShouldClose( window ) ){
        processInput( window );


        // float timeValue = glfwGetTime();
        // float greenValue = ( sin(timeValue ) / 2.0f ) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation( shaderProgram, "ourColor");

        

        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glActiveTexture( GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture( GL_TEXTURE_2D, texture2 );
        
        shaderClass.use();
        unsigned int transformLoc;
        // unsigned int transformLoc = glGetUniformLocation( shaderClass.ID, "model");
        // glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr(model));

        float camX = sin( glfwGetTime() ) * radius;
        float camZ = cos( glfwGetTime() ) * radius;

        view = c.lookat();

        transformLoc = glGetUniformLocation( shaderClass.ID, "view");
        glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr( view ));

        transformLoc = glGetUniformLocation( shaderClass.ID, "projection");

        projection = glm::perspective( glm::radians( c.zoom ) , 800.0f/ 600.0f, 0.1f, 100.0f );
        glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr(projection));


        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray( VAO );
        float time = glfwGetTime();
        for( int i = 0 ; i < 10; i++){
            transformLoc = glGetUniformLocation( shaderClass.ID, "model");
            glm::mat4 newModel = glm::mat4(1.0f );

            newModel = glm::translate( model, cubePositions[i]);
            newModel = glm::rotate( newModel, time, cubePositions[i]);
            
            glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr(newModel) );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }
        // glDrawArrays( GL_TRIANGLES, 0, 36 );

        // glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

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