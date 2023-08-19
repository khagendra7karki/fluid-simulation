#ifndef BUTTON_HEADER
#define BUTTON_HEADER

class Button {
private:
    unsigned int VBOS, EBOS;
public:
    unsigned int VAOS;
    float vertices[28];
    unsigned int indices[6];
    Button(unsigned int vao, unsigned int vbo, unsigned int ebo);
    void Update(float val);
    // ~Button();
};

#endif