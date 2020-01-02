// A texture.
struct Texture {
	GLuint textureID;

	Texture() {
		return;
	}

	Texture(GLuint textureID) {
		this->textureID = textureID;
	}
};