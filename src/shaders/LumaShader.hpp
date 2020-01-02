// A luma shader.
struct LumaShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/lumaVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/lumaFragment.glsl";

	// Uniforms.
	GLuint uTexture;
	GLuint uThreshold;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
		bindAttribute(1, "iTexture");
	}

	// Default constructor.
	LumaShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uTexture);
		LOAD_UNIFORM(uThreshold);
	}

	// Set threshold.
	void setThreshold(float threshold) {
		setUniformFloat(uThreshold, threshold);
	}
};