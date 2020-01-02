#version 150

in vec3 vPosition;

out vec4 oColor;

uniform samplerCube uSkybox;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;

uniform float uSunFalloff;
uniform float uSunIntensity;

const vec3 skyColor = vec3(89.0f / 255.0f, 119.0f / 255.0f, 143.0f / 255.0f);

void main() {
	// oColor = texture(uSkybox, vPosition);
	
	float power = max(dot(normalize(vPosition), uLightDirection), 0.0f);
	vec3 sunColor = uLightColor * pow(power, uSunFalloff) / uSunIntensity;
	oColor = vec4(skyColor + sunColor, 1.0f);
}