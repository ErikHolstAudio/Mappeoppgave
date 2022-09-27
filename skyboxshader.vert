#version 400
layout (location = 0) in vec3 aPos;

uniform mat4 rMatrix; // rotation
uniform mat4 vMatrix;
uniform mat4 pMatrix;
out vec3 texcoords;

void main()
{
vec4 pos = pMatrix * vMatrix * rMatrix * vec4(aPos, 1.0);
  gl_Position = pos.xyww;
  texcoords = aPos.xyz;

}