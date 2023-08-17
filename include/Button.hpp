#ifndef BUTTON_HEADER
#define BUTTON_HEADER

class Button {
private:
    unsigned int VBOS, EBOS;
public:
    unsigned int VAOS;
    Button(unsigned int vao, unsigned int vbo, unsigned int ebo);
    ~Button();
};
#endif