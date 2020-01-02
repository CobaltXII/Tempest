#version 150

in vec3 iPosition;

out vec3 vPosition;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
	gl_Position = uProjection * uView * vec4(iPosition, 1.0f);
	vPosition = iPosition;
}