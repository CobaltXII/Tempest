// A water shader.
struct WaterShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/waterVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/waterFragment.glsl";

	// Uniforms.
	GLuint uProjection;
	GLuint uView;
	GLuint uLightDirection;
	GLuint uLightColor;
	GLuint uFogDensity;
	GLuint uFogGradient;
	GLuint uCamera;
	GLuint uReflectionTexture;
	GLuint uRefractionTexture;
	GLuint uDepthTexture;
	GLuint uWaterDuDv;
	GLuint uWaterNormal;
	GLuint uTime;
	GLuint uWaterLevel;
	GLuint uWaterScale;
	GLuint uWaterStrength;
	GLuint uWaterSpeed;
	GLuint uWaterShinyness;
	GLuint uWaterSpecularity;
	GLuint uWaterColor;
	GLuint uWaterMurky;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
		bindAttribute(1, "iTexture");
		bindAttribute(2, "iNormal");
	}

	// Default constructor.
	WaterShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uProjection);
		LOAD_UNIFORM(uView);
		LOAD_UNIFORM(uLightDirection);
		LOAD_UNIFORM(uLightColor);
		LOAD_UNIFORM(uFogDensity);
		LOAD_UNIFORM(uFogGradient);
		LOAD_UNIFORM(uCamera);
		LOAD_UNIFORM(uReflectionTexture);
		LOAD_UNIFORM(uRefractionTexture);
		LOAD_UNIFORM(uDepthTexture);
		LOAD_UNIFORM(uWaterDuDv);
		LOAD_UNIFORM(uWaterNormal);
		LOAD_UNIFORM(uTime);
		LOAD_UNIFORM(uWaterLevel);
		LOAD_UNIFORM(uWaterScale);
		LOAD_UNIFORM(uWaterStrength);
		LOAD_UNIFORM(uWaterSpeed);
		LOAD_UNIFORM(uWaterShinyness);
		LOAD_UNIFORM(uWaterSpecularity);
		LOAD_UNIFORM(uWaterColor);
		LOAD_UNIFORM(uWaterMurky);
	}

	// Set the projection matrix.
	void setProjection(glm::mat4 m) {
		setUniformMat4(uProjection, m);
	}

	// Set the view matrix.
	void setView(glm::mat4 m) {
		setUniformMat4(uView, m);
	}

	// Set the light.
	void setLight(Light& light) {
		setUniformVec3(uLightDirection, light.direction);
		setUniformVec3(uLightColor, light.color);
	}

	// Set the camera.
	void setCamera(Camera& camera) {
		setUniformVec3(uCamera, camera.position);
	}

	// Set the time.
	void setTime(float time) {
		setUniformFloat(uTime, time);
	}

	// Set the water.
	void setWater(float level, float scale, float strength, float speed, float shinyness, float specularity, glm::vec4 color, glm::vec3 murky) {
		setUniformFloat(uWaterLevel, level);
		setUniformFloat(uWaterScale, scale);
		setUniformFloat(uWaterStrength, strength);
		setUniformFloat(uWaterSpeed, speed);
		setUniformFloat(uWaterShinyness, shinyness);
		setUniformFloat(uWaterSpecularity, specularity);
		setUniformVec4(uWaterColor, color);
		setUniformVec3(uWaterMurky, murky);
	}

	// Set the fog.
	void setFog(float fogDensity, float fogGradient) {
		setUniformFloat(uFogDensity, fogDensity);
		setUniformFloat(uFogGradient, fogGradient);
	}
};