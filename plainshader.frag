#version 410 core

in vec4 color;              //color received from the pipeline (and vertex shader)
in vec2 UV;
out vec4 fragmentColor;     //color sent to fragment on screen

void main() {
   fragmentColor = vec4(color.xyz, 1.0);   //color given to current fragment (pixel)
   //fragmentColor = vec4(1,0,0,0);

}
