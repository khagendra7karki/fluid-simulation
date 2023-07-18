#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
int main(){
  if( !glfwInit()){
    std::cout<<"An error occurred"<<std::endl;
  }
  GLFWwindow* window = glfwCreateWindow( 640, 480 , "Fluid-Simulation", NULL, NULL);
  if(!window){
    std::cout<<"An error occurred"<<std::endl;
  }
  glfwDestroyWindow( window );
  glfwTerminate();
}