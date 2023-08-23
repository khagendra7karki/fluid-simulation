#version 330 core
out vec4 FragColor;

in vec4 color;
in vec2 textpos;

uniform sampler2D texture;
uniform int ntext;

void main(){
    if(ntext==1){
        FragColor=texture(texture,textpos);
    }else
    FragColor = color;
}
