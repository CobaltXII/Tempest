// A terrain shader.
struct TerrainShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/terrainVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/terrainFragment.glsl";

	// Uniforms.
	GLuint uProjection;
	GLuint uView;
	GLuint uLightDirection;
	GLuint uLightColor;
	GLuint uFogDensity;
	GLuint uFogGradient;
	GLuint uCursorPosition;
	GLuint uCursorSize;
	GLuint uClipPlane;
	GLuint uTexture1;
	GLuint uTexture2;
	GLuint uTexture3;
	GLuint uHeightmap;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
		bindAttribute(1, "iTexture");
	}

	// Default constructor.
	TerrainShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uProjection);
		LOAD_UNIFORM(uView);
		LOAD_UNIFORM(uLightDirection);
		LOAD_UNIFORM(uLightColor);
		LOAD_UNIFORM(uFogDensity);
		LOAD_UNIFORM(uFogGradient);
		LOAD_UNIFORM(uCursorPosition);
		LOAD_UNIFORM(uCursorSize);
		LOAD_UNIFORM(uClipPlane);
		LOAD_UNIFORM(uTexture1);
		LOAD_UNIFORM(uTexture2);
		LOAD_UNIFORM(uTexture3);
		LOAD_UNIFORM(uHeightmap);
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

	// Set the fog.
	void setFog(float fogDensity, float fogGradient) {
		setUniformFloat(uFogDensity, fogDensity);
		setUniformFloat(uFogGradient, fogGradient);
	}

	// Set the cursor.
	void setCursor(glm::vec2 cursorPosition, float cursorSize) {
		setUniformVec2(uCursorPosition, cursorPosition);
		setUniformFloat(uCursorSize, cursorSize);
	}

	// Set the clipping plane.
	void setClipPlane(glm::vec4 clipPlane) {
		setUniformVec4(uClipPlane, clipPlane);
	}
};