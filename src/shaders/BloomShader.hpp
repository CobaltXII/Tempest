// A bloom shader.
struct BloomShader: Shader {
	const std::string VERTEX_FILE = "res/shaders/bloomVertex.glsl";
	const std::string FRAGMENT_FILE = "res/shaders/bloomFragment.glsl";

	// Uniforms.
	GLuint uSourceTexture;
	GLuint uBlurTexture;

	// Bind all used attributes.
	void bindAttributes() override {
		bindAttribute(0, "iPosition");
		bindAttribute(1, "iTexture");
	}

	// Default constructor.
	BloomShader() {
		loadFrom(VERTEX_FILE, FRAGMENT_FILE);
		LOAD_UNIFORM(uSourceTexture);
		LOAD_UNIFORM(uBlurTexture);
	}

	// Set the textures.
	void setTextures(GLuint source, GLuint blur) {
		setUniformSampler2D(uSourceTexture, GL_TEXTURE0, source);
		setUniformSampler2D(uBlurTexture, GL_TEXTURE1, blur);
	}
};