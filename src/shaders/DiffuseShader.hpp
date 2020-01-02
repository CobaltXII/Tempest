// A diffuse shader.
struct DiffuseShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/diffuseVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/diffuseFragment.glsl";

	// Uniforms.
	GLuint uProjection;
	GLuint uView;
	GLuint uModel;
	GLuint uLightDirection;
	GLuint uLightColor;
	GLuint uFogDensity;
	GLuint uFogGradient;
	GLuint uClipPlane;
	GLuint uTexture;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
		bindAttribute(1, "iTexture");
		bindAttribute(2, "iNormal");
	}

	// Default constructor.
	DiffuseShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uProjection);
		LOAD_UNIFORM(uView);
		LOAD_UNIFORM(uModel);
		LOAD_UNIFORM(uLightDirection);
		LOAD_UNIFORM(uLightColor);
		LOAD_UNIFORM(uFogDensity);
		LOAD_UNIFORM(uFogGradient);
		LOAD_UNIFORM(uClipPlane);
		LOAD_UNIFORM(uTexture);
	}

	// Set the projection matrix.
	void setProjection(glm::mat4 m) {
		setUniformMat4(uProjection, m);
	}

	// Set the view matrix.
	void setView(glm::mat4 m) {
		setUniformMat4(uView, m);
	}

	// Set the model matrix.
	void setModel(glm::mat4 m) {
		setUniformMat4(uModel, m);
	}

	// Set the light.
	void setLight(Light& light) {
		setUniformVec3(uLightDirection, light.direction);
		setUniformVec3(uLightColor, light.color);
	}

	// Set the fog.
	void setFog(float fogDensity, float fogGradient) {
		setUniformFloat(uFogDensity, fogDensity);
		setUniformFloat(uFogGradient, fogGradient);
	}

	// Set the clipping plane.
	void setClipPlane(glm::vec4 clipPlane) {
		setUniformVec4(uClipPlane, clipPlane);
	}
};