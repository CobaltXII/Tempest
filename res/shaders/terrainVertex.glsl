#version 150

in vec3 iPosition;
in vec2 iTexture;

out vec3 vPosition;
out vec2 vTexture;
out vec3 vNormal;
out float vVisibility;

uniform mat4 uProjection;
uniform mat4 uView;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;

uniform float uFogDensity;
uniform float uFogGradient;

uniform vec4 uClipPlane;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uHeightmap;

vec3 normal(vec2 uv) {
	vec2 epsilon = vec2(1.0f / textureSize(uHeightmap, 0).x, 0.0f);
	float l = texture(uHeightmap, uv - epsilon.xy).r;
	float r = texture(uHeightmap, uv + epsilon.xy).r;
	float d = texture(uHeightmap, uv - epsilon.yx).r;
	float u = texture(uHeightmap, uv + epsilon.yx).r;
	return normalize(vec3(l - r, 2.0f, d - u));
}

void main() {
	vec4 worldPosition = vec4(iPosition + vec3(0.0f, texture(uHeightmap, iTexture).r, 0.0f), 1.0f);
	gl_ClipDistance[0] = dot(worldPosition, uClipPlane);
	vec4 relativePosition = uView * worldPosition;
	gl_Position = uProjection * relativePosition;
	vPosition = worldPosition.xyz;
	vTexture = iTexture;
	vNormal = normal(iTexture);
	float distance = length(relativePosition.xyz);
	vVisibility = exp(-pow(distance * uFogDensity, uFogGradient));
	vVisibility = clamp(vVisibility, 0.0f, 1.0f);
}