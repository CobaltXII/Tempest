#version 150

in vec3 iPosition;
in vec2 iTexture;
in vec3 iNormal;

out vec2 vTexture;
out vec3 vNormal;
out vec3 vToCamera;
out vec4 vClipSpace;

uniform mat4 uProjection;
uniform mat4 uView;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;
uniform vec3 uCamera;

uniform float uWaterLevel;

void main() {
	vec4 worldPosition = vec4(iPosition + vec3(0.0f, uWaterLevel, 0.0f), 1.0f);
	vec4 relativePosition = uView * worldPosition;
	vClipSpace = uProjection * relativePosition;
	gl_Position = vClipSpace;
	vTexture = iTexture;
	vNormal = iNormal;
	vToCamera = uCamera - worldPosition.xyz;
}