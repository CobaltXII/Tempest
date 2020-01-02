// A camera.
struct Camera {
	glm::vec3 position;
	float pitch;
	float yaw;
	float roll;

	Camera() {
		return;
	}

	Camera(glm::vec3 position, float pitch, float yaw, float roll) {
		this->position = position;
		this->pitch = pitch;
		this->yaw = yaw;
		this->roll = roll;
	}

	// Get the camera's view matrix.
	glm::mat4 getView() {
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, roll, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix = glm::translate(matrix, -position);
		return matrix;
	}

	// Get the camera's view matrix (for cubemaps).
	glm::mat4 getCubemapView() {
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, roll, glm::vec3(0.0f, 0.0f, 1.0f));
		return matrix;
	}
};