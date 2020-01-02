// A filter shader.
struct FilterShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/filterVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/filterFragment.glsl";

	// Uniforms.
	GLuint uTexture;
	GLuint uSaturation;
	GLuint uTemperature;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
		bindAttribute(1, "iTexture");
	}

	// Default constructor.
	FilterShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uTexture);
		LOAD_UNIFORM(uSaturation);
		LOAD_UNIFORM(uTemperature);
	}

	// Set filters.
	void setFilters(float saturation, float temperature) {
		setUniformFloat(uSaturation, saturation);
		setUniformFloat(uTemperature, temperature);
	}
};