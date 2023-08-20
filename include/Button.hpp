#ifndef BUTTON_HEADER
#define BUTTON_HEADER
#include<vector>
#include<iostream>
class Button {
private:
    unsigned int VBOS, EBOS;
public:
    unsigned int VAOS;
    std::vector<float> vertices;
    // float vertices[40];
    unsigned int indices[6];
    Button(unsigned int vao, unsigned int vbo, unsigned int ebo);
    void Update(float val);
    // ~Button();
};

#endif