#version 150

in vec2 vTexture;

out vec4 oColor;

uniform sampler2D uTexture;

void main() {
	oColor = texture(uTexture, vTexture);
}