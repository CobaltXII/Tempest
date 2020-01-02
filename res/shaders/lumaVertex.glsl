#version 150

in vec2 iPosition;
in vec2 iTexture;

out vec2 vTexture;

void main() {
	gl_Position = vec4(iPosition, 0.0f, 1.0f);
	vTexture = iTexture;
}