// The manager.
namespace Manager {
	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;
	std::vector<GLuint> textures;
	std::vector<GLuint> framebuffers;

	// Create a VAO.
	GLuint createVAO() {
		GLuint vaoID;
		glGenVertexArrays(1, &vaoID);
		vaos.push_back(vaoID);
		return vaoID;
	}

	// Create a VAO.
	GLuint createVBO() {
		GLuint vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		return vboID;
	}

	// Create a texture.
	GLuint createTexture() {
		GLuint textureID;
		glGenTextures(1, &textureID);
		textures.push_back(textureID);
		return textureID;
	}

	// Create a framebuffer.
	GLuint createFramebuffer() {
		GLuint framebufferID;
		glGenFramebuffers(1, &framebufferID);
		framebuffers.push_back(framebufferID);
		return framebufferID;
	}

	// Create a framebuffer.
	Framebuffer createFramebuffer(int width, int height, GLenum interpolation = GL_LINEAR, GLenum wrap = GL_REPEAT) {
		GLuint framebufferID = createFramebuffer();
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		GLuint textureID = createTexture();
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);

		GLuint depthTextureID = createTexture();
		glBindTexture(GL_TEXTURE_2D, depthTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureID, 0);

		return Framebuffer(framebufferID, textureID, depthTextureID, width, height);
	}

	// Store a list of data in an attribute list.
	void storeDataInAttributeList(int attributeIndex, int elementWidth, float* data, int dataLength) {
		GLuint vboID = createVBO();
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		glBufferData(GL_ARRAY_BUFFER, dataLength * elementWidth * sizeof(float), data, GL_STATIC_DRAW);
		glVertexAttribPointer(attributeIndex, elementWidth, GL_FLOAT, GL_FALSE, elementWidth * sizeof(float), 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Create a model from raw data.
	Model createModel(float* vertices, float* textures, float* normals, int vertexCount) {
		GLuint vaoID = createVAO();
		glBindVertexArray(vaoID);

		storeDataInAttributeList(0, 3, vertices, vertexCount);
		storeDataInAttributeList(1, 2, textures, vertexCount);
		storeDataInAttributeList(2, 3, normals, vertexCount);

		glBindVertexArray(0);
		return Model(vaoID, vertexCount);
	}

	// Create a model from raw data.
	Model createModelWithoutNormals(float* vertices, float* textures, int vertexCount) {
		GLuint vaoID = createVAO();
		glBindVertexArray(vaoID);

		storeDataInAttributeList(0, 3, vertices, vertexCount);
		storeDataInAttributeList(1, 2, textures, vertexCount);

		glBindVertexArray(0);
		return Model(vaoID, vertexCount);
	}

	// Create a quad.
	Model createQuad(float x, float y, float w, float h) {
		float xf = x;
		float yf = y;
		float wf = w;
		float hf = h;
		float vertices[] = {
			xf, yf,
			xf, yf + hf,
			xf + wf, yf,
			xf + wf, yf,
			xf, yf + hf,
			xf + wf, yf + hf,
		};
		float textures[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f
		};

		GLuint vaoID = createVAO();
		glBindVertexArray(vaoID);
		storeDataInAttributeList(0, 2, vertices, 6);
		storeDataInAttributeList(1, 2, textures, 6);
		glBindVertexArray(0);
		return Model(vaoID, 6);
	}

	// Load a model from a file.
	Model loadModel(std::string path) {
		std::vector<float> vertices;
		std::vector<float> textures;
		std::vector<float> normals;

		loadWavefront(path, vertices, textures, normals);

		float* rawVertices = vertices.data();
		float* rawTextures = textures.data();
		float* rawNormals = normals.data();
		return createModel(rawVertices, rawTextures, rawNormals, vertices.size() / 3);
	}

	// Load a texture from a file.
	Texture loadTexture(std::string path) {
		GLuint textureID = createTexture();
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width;
		int height;
		int channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data) {
			fatalError("stbi_load(\"%s\")", path.c_str());
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		return Texture(textureID);
	}

	// Load a cubemap from many files.
	Cubemap loadCubemap(std::vector<std::string> paths) {
		GLuint cubemapID = createTexture();
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
			for (unsigned int i = 0; i < paths.size(); i++) {
				int width;
				int height;
				int channels;
				stbi_set_flip_vertically_on_load(false);
				unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb);
				if (!data) {
					fatalError("stbi_load(\"%s\")", paths[i].c_str());
				}
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		GLuint modelID = createVAO();
		{
			glBindVertexArray(modelID);
			storeDataInAttributeList(0, 3, cubemapVertices, 36);
			glBindVertexArray(0);
		}

		return Cubemap(cubemapID, modelID);
	}

	// Clean everything up.
	void cleanUp() {
		for (auto vao: vaos) {
			glDeleteVertexArrays(1, &vao);
		}
		for (auto vbo: vbos) {
			glDeleteBuffers(1, &vbo);
		}
		for (auto texture: textures) {
			glDeleteTextures(1, &texture);
		}
		for (auto framebuffer: framebuffers) {
			glDeleteFramebuffers(1, &framebuffer);
		}
	}
};