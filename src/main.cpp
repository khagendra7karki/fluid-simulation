#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cmath>
float vertices[] = {
// positions // colors
0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1 ) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";




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

    unsigned int vertexShader;
    vertexShader = glCreateShader( GL_VERTEX_SHADER );

    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );
    
    int success;
    char infoLog[512];
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success);

    if( !success ){
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog);
        std::cout<<"Erro::Shader::VERTEX::COMPILATION_FAILED \n"<<infoLog<< std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader( fragmentShader );
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success);

    if( !success ){
        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );

    glLinkProgram( shaderProgram );

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    unsigned int VBO , VAO;
    glGenVertexArrays( 1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray( VAO );

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices) , vertices, GL_STATIC_DRAW);

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( void * )0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( void* )( 3 * sizeof( float )));
    glEnableVertexAttribArray(1);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray(0);


    while( !glfwWindowShouldClose( window ) ){
        processInput( window );


        // float timeValue = glfwGetTime();
        // float greenValue = ( sin(timeValue ) / 2.0f ) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation( shaderProgram, "ourColor");

    
        
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram );
        
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray( VAO );

        glDrawArrays( GL_TRIANGLES, 0, 3 );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glDeleteVertexArrays( 1, &VAO);
    glDeleteBuffers( 1, &VBO );
    glDeleteProgram( shaderProgram );
    glfwTerminate();
    return 0;
    
    
}