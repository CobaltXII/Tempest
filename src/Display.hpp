// A display.
struct Display {
	int width;
	int height;

	SDL_Window* window;
	SDL_GLContext context;

	Display() {
		return;
	}

	Display(std::string title, int width, int height) {
		this->width = width;
		this->height = height;

		// Initialize SDL.
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			fatalError("SDL_Init: %s", SDL_GetError());
		}

		// Set up attributes.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

		// Create the SDL_Window*.
		window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			SDL_WINDOW_OPENGL |
			SDL_WINDOW_ALLOW_HIGHDPI
		);
		if (!window) {
			fatalError("SDL_CreateWindow: %s", SDL_GetError());
		}

		// Create the SDL_GLContext.
		context = SDL_GL_CreateContext(window);
		if (!context) {
			fatalError("SDL_GL_CreateContext: %s", SDL_GetError());
		}

		// Load OpenGL functions using GLAD.
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		// Set up ImGui.
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Use ImGui dark theme.
		ImGui::StyleColorsClassic();

		// Set up ImGui bindings.
		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init("#version 150");
	}

	// Update the display.
	void update(int fps) {
		// Swap buffers.
		SDL_GL_SwapWindow(window);
	}

	// Destroy the display.
	void destroy() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
};