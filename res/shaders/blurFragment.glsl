#version 150

in vec2 vTexture;

out vec4 oColor;

uniform sampler2D uTexture;

uniform bool uHorizontal;

const float weights[5] = float[5](
	0.2270270270f,
	0.1945945946f,
	0.1216216216f,
	0.0540540541f,
	0.0162162162f
);

void main() {
	vec2 pixel = 1.0f / textureSize(uTexture, 0);

	vec3 result = texture(uTexture, vTexture).rgb * weights[0];

	if (uHorizontal) {
		for (int i = 1; i < 5; i++) {
			result += texture(uTexture, vTexture + vec2(pixel.x * i, 0.0f)).rgb * weights[i];
			result += texture(uTexture, vTexture - vec2(pixel.x * i, 0.0f)).rgb * weights[i];
		}
	} else {
		for (int i = 1; i < 5; i++) {
			result += texture(uTexture, vTexture + vec2(0.0f, pixel.y * i)).rgb * weights[i];
			result += texture(uTexture, vTexture - vec2(0.0f, pixel.y * i)).rgb * weights[i];
		}
	}

	oColor = vec4(result, 1.0f);
}