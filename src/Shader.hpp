// A shader program.
struct Shader {
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	// Load this shader program from two shaders.
	void loadFrom(std::string vertexFile, std::string fragmentFile) {
		vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
		fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		bindAttributes();
		glLinkProgram(programID);
		GLint success;
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			int length;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
			char* error = new char[length];
			glGetProgramInfoLog(programID, length, NULL, error);
			std::cerr << error << std::endl;
			fatalError("glLinkProgram");
		}
	}

	// Load a shader.
	GLuint loadShader(std::string filePath, GLenum type) {
		std::string shaderString = loadFileAsString(filePath);
		const char* shaderSource = shaderString.c_str();
		int length = strlen(shaderSource);
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &shaderSource, &length);
		glCompileShader(shader);
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* error = new char[length];
			glGetShaderInfoLog(shader, length, NULL, error);
			std::cerr << error << std::endl;
			fatalError("glCompileShader");
		}
		return shader;
	}

	// Get the location of a uniform variable in this shader.
	GLuint getUniformLocation(std::string uniformName) {
		return glGetUniformLocation(programID, uniformName.c_str());
	}

	// Set a uniform float.
	void setUniformFloat(GLuint location, float value) {
		glUniform1f(location, value);
	}

	// Set a uniform int.
	void setUniformInt(GLuint location, int value) {
		glUniform1i(location, value);
	}

	// Set a uniform vec2.
	void setUniformVec2(GLuint location, glm::vec2& value) {
		glUniform2f(location, value.x, value.y);
	}

	// Set a uniform vec3.
	void setUniformVec3(GLuint location, glm::vec3& value) {
		glUniform3f(location, value.x, value.y, value.z);
	}

	// Set a uniform vec4.
	void setUniformVec4(GLuint location, glm::vec4& value) {
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	// Set a uniform mat4.
	void setUniformMat4(GLuint location, glm::mat4& value) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}

	// Set a uniform sampler2D.
	void setUniformSampler2D(GLuint location, GLenum texture, GLuint textureID) {
		glActiveTexture(texture);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(location, texture - GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	// Set a uniform samplerCube.
	void setUniformSamplerCube(GLuint cubemapID) {
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
	}

	// Bind all used attributes.
	virtual void bindAttributes() {
		return;
	}

	// Bind an attribute to a shader variable.
	void bindAttribute(int attribute, std::string variableName) {
		glBindAttribLocation(programID, attribute, variableName.c_str());
	}

	// Enable this shader.
	void enable() {
		glUseProgram(programID);
	}

	// Disable this shader.
	void disable() {
		glUseProgram(0);
	}

	// Destroy the shader.
	void destroy() {
		disable();
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		glDeleteProgram(programID);
	}
};

// This macro makes loading uniforms less repetitive.
#define LOAD_UNIFORM(uniform) uniform = getUniformLocation(#uniform)