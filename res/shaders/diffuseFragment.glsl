#version 150

in vec2 vTexture;
in vec3 vNormal;
in float vVisibility;

out vec4 oColor;

uniform sampler2D uTexture;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;

const vec3 skyColor = vec3(89.0f / 255.0f, 119.0f / 255.0f, 143.0f / 255.0f);

void main() {
	vec3 unitNormal = normalize(vNormal);
	float diffuseBrightness = max(dot(unitNormal, uLightDirection), 0.3f);
	vec3 diffuse = diffuseBrightness * uLightColor;
	vec4 textureColor = texture(uTexture, vTexture);
	if (textureColor.w < 0.5f) {
		discard;
	}
	oColor = vec4(diffuse, 1.0f) * textureColor;
	oColor = mix(vec4(skyColor, 1.0f), oColor, vVisibility);
}