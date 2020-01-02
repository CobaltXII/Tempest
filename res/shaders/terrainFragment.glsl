#version 150

in vec3 vPosition;
in vec2 vTexture;
in vec3 vNormal;
in float vVisibility;

out vec4 oColor;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;

uniform vec2 uCursorPosition;
uniform float uCursorSize;

const vec3 skyColor = vec3(89.0f / 255.0f, 119.0f / 255.0f, 143.0f / 255.0f);

float hexagramSDF(vec2 p, float r) {
	const vec4 k = vec4(-0.5f, 0.8660254038f, 0.5773502692f, 1.7320508076f);
	p = abs(p);
	p -= 2.0f * min(dot(k.xy, p), 0.0f) * k.xy;
	p -= 2.0f * min(dot(k.yx, p), 0.0f) * k.yx;
	p -= vec2(clamp(p.x, r * k.z, r * k.w), r);
	return length(p) * sign(p.y);
}

void main() {
	vec3 unitNormal = normalize(vNormal);
	float diffuseBrightness = max(dot(unitNormal, uLightDirection), 0.3f);
	vec3 diffuse = diffuseBrightness * uLightColor;
	vec2 coordinates = vTexture * 100.0f;
	vec3 color = texture(uTexture2, coordinates).rgb;
	color = mix(color, texture(uTexture1, coordinates).rgb, clamp(vPosition.y / 10.0f, 0.0f, 1.0f));
	color = mix(color, texture(uTexture3, coordinates).rgb, max(0.0f, sqrt(1.0f - dot(unitNormal, vec3(0.0f, 1.0f, 0.0f)))));
	oColor = vec4(diffuse * color, 1.0f);
	oColor = mix(vec4(skyColor, 1.0f), oColor, vVisibility);
	float cursorDistance = abs(length(vPosition.xz - uCursorPosition) - uCursorSize);
	if (cursorDistance < 3.0f) {
		oColor /= cursorDistance / 3.0f;
	}
}