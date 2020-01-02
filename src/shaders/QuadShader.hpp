// A quad shader.
struct QuadShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/quadVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/quadFragment.glsl";

	// Uniforms.
	GLuint uTexture;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
		bindAttribute(1, "iTexture");
	}

	// Default constructor.
	QuadShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uTexture);
	}
};