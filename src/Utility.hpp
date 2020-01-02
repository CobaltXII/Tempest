// Fatal error.
void fatalError(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t bufsz = vsnprintf(NULL, 0, fmt, args) + 1;
	va_end(args);
	char* buf = (char*)malloc(bufsz);
	if (!buf) {
		return;
	}
	va_start(args, fmt);
	vsnprintf(buf, bufsz, fmt, args);
	va_end(args);
	fprintf(stderr, "Fatal error at %s\n", buf);
	exit(EXIT_FAILURE);
}

// Format string.
char* formatString(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t bufsz = vsnprintf(NULL, 0, fmt, args) + 1;
	va_end(args);
	char* buf = (char*)malloc(bufsz);
	if (!buf) {
		return NULL;
	}
	va_start(args, fmt);
	vsnprintf(buf, bufsz, fmt, args);
	va_end(args);
	return buf;
}

// Load a file as a string.
std::string loadFileAsString(std::string filePath) {
	std::ifstream t(filePath);
	if (!t) {
		fatalError("std::ifstream(\"%s\")", filePath.c_str());
	}
	std::stringstream buffer;
	buffer << t.rdbuf() << "\0";
	return buffer.str();
}

// Create a transformation matrix.
glm::mat4 createTransformation(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, translation);
	matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::scale(matrix, scale);
	return matrix;
}

// Random unit float.
float randomFloat() {
	return float(rand()) / float(RAND_MAX);
}

// Random signed unit float.
float signedRandomFloat() {
	return float(rand()) / float(RAND_MAX) * 2.0f - 1.0f;
}