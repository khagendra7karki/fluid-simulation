#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<Shader.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<Camera.hpp>
#include<utility.hpp>
#include<Fluid.hpp>
#include<Button.hpp>

unsigned int WINDOW_WIDTH =  800;
unsigned int WINDOW_HEIGHT = 600;

bool switchReleased_S = true;
bool switchReleased_X = true;

Camera c( WINDOW_WIDTH, WINDOW_HEIGHT );
Fluid f;


void framebuffer_size_callback( GLFWwindow* window, int width, int height){
    glViewport( 0, 0, width , height);
    c.changeViewPort( width, height );          //change the projection window parameters after viewport change
    
}


void processInput( GLFWwindow *window ){
    
    // 'SPACE' key to exit the application
    if( glfwGetKey( window, GLFW_KEY_SPACE) == GLFW_PRESS ){
        glfwSetWindowShouldClose(window, true);
    }    

    // 'S' key to stop or resume the simulation
    if( switchReleased_S && (glfwGetKey( window, GLFW_KEY_S) == GLFW_PRESS)  ){
        switchReleased_S = false;
        f.startSimulation = !f.startSimulation;
        std::cout<<f.startSimulation<<std::endl;
    }
    else if( !switchReleased_S &&  glfwGetKey( window, GLFW_KEY_S ) == GLFW_RELEASE ){
        switchReleased_S = true;
    }

    // 'X' key to reset the simulation
    if(switchReleased_X && glfwGetKey( window, GLFW_KEY_X ) == GLFW_PRESS ){
        switchReleased_X = false;   
        f.resetSimulation();
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
    if ( !c.isMouseClicked && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        // f.addParticles( Vector3f( 0.0f, 0.2f + f.BOX_SIZE, 0.0f ));
        std::cout<<"Particles added"<<std::endl;
        c.isMouseClicked = true;
    }
    else if( c.isMouseClicked && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE  ) 
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

    unsigned int VBO[2] , VAO[2], EBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(2, EBO);


    glBindVertexArray( VAO[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData( GL_ARRAY_BUFFER, sizeOfVertices , vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_DYNAMIC_DRAW );


    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof( float ), ( void * )0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof( float ), ( void* )( 3 * sizeof( float )));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof( float ), ( void* )( 6 * sizeof( float )));
    glEnableVertexAttribArray(2);


    Button button(VAO[1],VBO[1],EBO[1]);

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
        
        // glBindBuffer( GL_ARRAY_BUFFER, 0 );
        
        shaderClass.use();

        shaderClass.setMat4( "model", model );
        shaderClass.setMat4( "view",model );
        shaderClass.setMat4( "projection", model); 
        glBindVertexArray(VAO[1]);
        glBindBuffer( GL_ARRAY_BUFFER, VBO[1] );
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

        shaderClass.setMat4( "view", c.view );
        shaderClass.setMat4( "projection", c.projection );
        glBindVertexArray( VAO[0] );
        glBindBuffer( GL_ARRAY_BUFFER,VBO[0] );
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
        button.Update( frameRate );
        std::cout<<"The instantaneos frame rate is "<<frameRate<<std::endl;
    }

    glDeleteVertexArrays( 2, VAO);
    glDeleteBuffers( 2 , VBO );
    glDeleteBuffers( 2, EBO );

    std::cout<<f.mParticles.size()<<std::endl;
    shaderClass.close();
    glfwTerminate();
    return 0;
}