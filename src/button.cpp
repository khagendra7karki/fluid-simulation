#include<Button.hpp> 
#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

Button::Button(unsigned int vao, unsigned int vbo, unsigned int ebo):vertices{
        -0.8f,  0.8f, 0.0f, 1.0f, 0.5f, 0.2f, 1.0f, // top right
        -0.8f, 0.75f, 0.0f,  1.0f, 0.5f, 0.2f, 1.0f,// bottom right
        -0.95f, 0.75f, 0.0f,  1.0f, 0.5f, 0.2f, 1.0f,// bottom left
        -0.95f,  0.8f, 0.0f,  1.0f, 0.5f, 0.2f, 1.0f,// top left 


    },indices{  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3,   // second Triangle

       
    },VAOS(vao),VBOS(vbo),EBOS(ebo){
    

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOS);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOS);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray( 0 );
}
void Button::Update(float val){
    vertices[0] = vertices[7]=(0.95-0.8)*val/150.0; 
     glBindVertexArray(VAOS);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOS);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray( 0 );
}
// Button::~Button(){}