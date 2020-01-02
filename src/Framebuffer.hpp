// A framebuffer.
struct Framebuffer {
	GLuint framebufferID;
	GLuint textureID;
	GLuint depthTextureID;
	int width;
	int height;

	Framebuffer() {
		return;
	}

	Framebuffer(GLuint framebufferID, GLuint textureID, GLuint depthTextureID, int width, int height) {
		this->framebufferID = framebufferID;
		this->textureID = textureID;
		this->depthTextureID = depthTextureID;
		this->width = width;
		this->height = height;
	}

	// Bind this framebuffer.
	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glViewport(0, 0, width, height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Unbind this framebuffer.
	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};