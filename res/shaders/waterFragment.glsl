#version 150

in vec2 vTexture;
in vec3 vNormal;
in vec3 vToCamera;
in vec4 vClipSpace;

out vec4 oColor;

uniform sampler2D uReflectionTexture;
uniform sampler2D uRefractionTexture;
uniform sampler2D uDepthTexture;
uniform sampler2D uWaterDuDv;
uniform sampler2D uWaterNormal;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;

uniform float uTime;

uniform float uWaterScale;
uniform float uWaterStrength;
uniform float uWaterSpeed;
uniform float uWaterSpecularity;
uniform float uWaterShinyness;
uniform vec4 uWaterColor;
uniform vec3 uWaterMurky;

void main() {
	vec2 ndc = (vClipSpace.xy / vClipSpace.w) / 2.0f + 0.5f;

	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);

	const float near = 0.5f;
	const float far = 5000.0f;
	float floorZ = texture(uDepthTexture, ndc).x;
	float floorDistance = 2.0f * near * far / (far + near - (2.0f * floorZ - 1.0f) * (far - near));

	float waterZ = gl_FragCoord.z;
	float waterDistance = 2.0f * near * far / (far + near - (2.0f * waterZ - 1.0f) * (far - near));
	float waterDepth = floorDistance - waterDistance;

	float distortion = uTime * uWaterSpeed;
	vec2 distortion1 = texture(uWaterDuDv, vec2(vTexture.x + distortion, vTexture.y) * uWaterScale).rg * 0.1f;
	vec2 distortion2 = texture(uWaterDuDv, vec2(-vTexture.x + distortion, vTexture.y + distortion) * uWaterScale).rg * 0.1f;
	vec2 distortedTexCoords = distortion1 + distortion2;
	vec2 totalDistortion = (texture(uWaterDuDv, distortedTexCoords).rg * 2.0f - 1.0f) * uWaterStrength;

	reflectTexCoords += totalDistortion;
	refractTexCoords += totalDistortion;

	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.01f, 0.99f);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.99f, -0.01f);

	refractTexCoords = clamp(refractTexCoords, 0.01f, 0.99f);

	vec4 reflectColor = texture(uReflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(uRefractionTexture, refractTexCoords);
	refractColor = mix(refractColor, vec4(uWaterMurky, 1.0f), clamp(waterDepth / 30.0f, 0.0f, 1.0f));

	vec3 viewVector = normalize(vToCamera);
	float refractiveFactor = dot(viewVector, vNormal);

	vec3 normalColor = texture(uWaterNormal, distortedTexCoords).xyz;
	vec3 normal = normalize(vec3(normalColor.r * 2.0f - 1.0f, normalColor.b, normalColor.g * 2.0f - 1.0f));

	vec3 reflectedLight = reflect(uLightDirection, normal);
	float specularFactor = pow(max(dot(reflectedLight, -viewVector), 0.0f), uWaterSpecularity);
	vec3 specularHighlights = vec3(specularFactor * uWaterShinyness);

	oColor = mix(reflectColor, refractColor, refractiveFactor);
	oColor = mix(oColor, vec4(uWaterColor.xyz, 1.0f), uWaterColor.w) + vec4(specularHighlights, 1.0f);
	oColor.a = clamp(waterDepth / 10.0f, 0.0f, 1.0f);
}