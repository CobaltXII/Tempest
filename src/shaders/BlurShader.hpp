// A blur shader.
struct BlurShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/blurVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/blurFragment.glsl";

	// Uniforms.
	GLuint uTexture;
	GLuint uHorizontal;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
		bindAttribute(1, "iTexture");
	}

	// Default constructor.
	BlurShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uTexture);
		LOAD_UNIFORM(uHorizontal);
	}

	// Set the shader to horizontal mode.
	void modeHorizontal() {
		setUniformInt(uHorizontal, GL_TRUE);
	}

	// Set the shader to vertical mode.
	void modeVertical() {
		setUniformInt(uHorizontal, GL_FALSE);
	}
};