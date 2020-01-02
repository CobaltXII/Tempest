#version 150

in vec2 vTexture;

out vec4 oColor;

uniform sampler2D uTexture;
uniform float uSaturation;
uniform float uTemperature;

void main() {
	vec3 color = texture(uTexture, vTexture).rgb;

	color.r += uTemperature;
	color.b -= uTemperature;
	color = mix(color, vec3(dot(vec3(0.3f, 0.6f, 0.1f), color)), -uSaturation);

	oColor = vec4(color, 1.0f);
}