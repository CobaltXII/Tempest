#version 150

in vec2 vTexture;

out vec4 oColor;

uniform sampler2D uSourceTexture;
uniform sampler2D uBlurTexture;

void main() {
	oColor = texture(uSourceTexture, vTexture) + texture(uBlurTexture, vTexture);
}