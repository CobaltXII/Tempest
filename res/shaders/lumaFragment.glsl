#version 150

in vec2 vTexture;

out vec4 oColor;

uniform sampler2D uTexture;

uniform float uThreshold;

void main() {
	vec3 color = texture(uTexture, vTexture).rgb;

	if (dot(color, vec3(0.2126f, 0.7152f, 0.0722f)) < uThreshold) {
		color = vec3(0.0f);
	}

	oColor = vec4(color, 1.0f);
}