#version 400

in vec3 texcoords;
uniform samplerCube textureSampler;
out vec4 frag_colour;

void main() {
  frag_colour = texture(textureSampler, texcoords);
}