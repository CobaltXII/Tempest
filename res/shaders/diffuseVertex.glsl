#version 150

in vec3 iPosition;
in vec2 iTexture;
in vec3 iNormal;

out vec2 vTexture;
out vec3 vNormal;
out float vVisibility;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;

uniform float uFogDensity;
uniform float uFogGradient;

uniform vec4 uClipPlane;

void main() {
	vec4 worldPosition = uModel * vec4(iPosition, 1.0f);
	gl_ClipDistance[0] = dot(worldPosition, uClipPlane);
	vec4 relativePosition = uView * worldPosition;
	gl_Position = uProjection * relativePosition;
	vTexture = iTexture;
	vNormal = (uModel * vec4(iNormal, 0.0f)).xyz;
	float distance = length(relativePosition.xyz);
	vVisibility = exp(-pow(distance * uFogDensity, uFogGradient));
	vVisibility = clamp(vVisibility, 0.0f, 1.0f);
}