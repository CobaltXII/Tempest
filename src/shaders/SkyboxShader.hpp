// A skybox shader.
struct SkyboxShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/skyboxVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/skyboxFragment.glsl";

	// Uniforms.
	GLuint uProjection;
	GLuint uView;
	GLuint uLightDirection;
	GLuint uLightColor;
	GLuint uSunFalloff;
	GLuint uSunIntensity;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
	}

	// Default constructor.
	SkyboxShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uProjection);
		LOAD_UNIFORM(uView);
		LOAD_UNIFORM(uLightDirection);
		LOAD_UNIFORM(uLightColor);
		LOAD_UNIFORM(uSunFalloff);
		LOAD_UNIFORM(uSunIntensity);
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

	// Set the sun.
	void setSun(float falloff, float intensity) {
		setUniformFloat(uSunFalloff, falloff);
		setUniformFloat(uSunIntensity, intensity);
	}
};