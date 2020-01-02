// A water object.
struct WaterObject {
	Model model;
	Texture waterDuDv;
	Texture waterNormal;

	WaterObject() {
		return;
	}

	WaterObject(Model model, int width, int height, Texture textureDuDv, Texture textureNormal) {
		this->model = model;
		this->waterDuDv = textureDuDv;
		this->waterNormal = textureNormal;
		initializeReflection(width, height);
		initializeRefraction(width, height);
	}

	// Create a framebuffer.
	GLuint createFramebuffer() {
		GLuint framebufferID = Manager::createFramebuffer();
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		return framebufferID;
	}

	// Bind a framebuffer.
	void bindFramebuffer(GLuint framebufferID, int width, int height) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CLIP_DISTANCE0);
	}

	// Unbind any framebuffer.
	void unbindFramebuffer() {
		glDisable(GL_CLIP_DISTANCE0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Create a texture attachment.
	GLuint createTextureAttachment(int width, int height) {
		GLuint textureID = Manager::createTexture();
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);
		return textureID;
	}

	// Create a depth texture attachment.
	GLuint createDepthTextureAttachment(int width, int height) {
		GLuint textureID = Manager::createTexture();
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0);
		return textureID;
	}

	// Initialize all reflection resources.
	int reflectionWidth;
	int reflectionHeight;
	GLuint reflectionFramebufferID;
	GLuint reflectionTextureID;
	GLuint reflectionDepthTextureID;
	void initializeReflection(int width, int height) {
		const int scale = 4;
		reflectionWidth = width / scale;
		reflectionHeight = height / scale;
		reflectionFramebufferID = createFramebuffer();
		reflectionTextureID = createTextureAttachment(reflectionWidth, reflectionHeight);
		reflectionDepthTextureID = createDepthTextureAttachment(reflectionWidth, reflectionHeight);
		unbindFramebuffer();
	}

	// Initialize all refraction resources.
	int refractionWidth;
	int refractionHeight;
	GLuint refractionFramebufferID;
	GLuint refractionTextureID;
	GLuint refractionDepthTextureID;
	void initializeRefraction(int width, int height) {
		const int scale = 4;
		refractionWidth = width / scale;
		refractionHeight = height / scale;
		refractionFramebufferID = createFramebuffer();
		refractionTextureID = createTextureAttachment(refractionWidth, refractionHeight);
		refractionDepthTextureID = createDepthTextureAttachment(refractionWidth, refractionHeight);
		unbindFramebuffer();
	}

	// Bind the reflection framebuffer.
	void bindReflectionFramebuffer() {
		bindFramebuffer(reflectionFramebufferID, reflectionWidth, reflectionHeight);
	}

	// Bind the refraction framebuffer.
	void bindRefractionFramebuffer() {
		bindFramebuffer(refractionFramebufferID, refractionWidth, refractionHeight);
	}

	// Unbind the reflection framebuffer.
	void unbindReflectionFramebuffer() {
		unbindFramebuffer();
	}

	// Unbind the refraction framebuffer.
	void unbindRefractionFramebuffer() {
		unbindFramebuffer();
	}
};

// Water library.
namespace Water {
	// Generate water.
	WaterObject generateWater(float size, int width, int height, Texture textureDuDv, Texture textureNormal) {
		float vertices[] = {
			-size, 0.0f, -size,
			-size, 0.0f, +size,
			+size, 0.0f, -size,
			+size, 0.0f, -size,
			-size, 0.0f, +size,
			+size, 0.0f, +size,
		};

		float textures[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
		};

		float normals[] = {
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
		};

		Model out = Manager::createModel(vertices, textures, normals, 6);
		return WaterObject(out, width, height, textureDuDv, textureNormal);
	}
};