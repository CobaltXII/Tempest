// A renderer.
struct Renderer {
	const float FOV = 60.0f;
	const float NEAR_PLANE = 0.5f;
	const float FAR_PLANE = 5000.0f;

	Display display;
	glm::mat4 projection;

	int width;
	int height;

	Renderer() {
		return;
	}

	Renderer(std::string title, int width, int height) {
		display = Display(title, width, height);
		SDL_GL_GetDrawableSize(display.window, &this->width, &this->height);
		projection = glm::perspectiveFov(
			glm::radians(FOV),
			float(display.width),
			float(display.height),
			NEAR_PLANE,
			FAR_PLANE
		);
	}

	// Get the mouse ray.
	glm::vec3 getMouseRay(Camera camera, int mouseX, int mouseY) {
		float u = float(mouseX) * 2.0f / float(display.width) - 1.0f;
		float v = 1.0f - float(mouseY) * 2.0f / float(display.height);
		glm::vec4 b = glm::inverse(projection) * glm::vec4(u, v, -1.0f, 1.0f);
		return normalize(glm::inverse(camera.getView()) * glm::vec4(b.x, b.y, -1.0f, 0.0f));
	}

	// Get the time in seconds.
	float getTime() {
		return float(SDL_GetTicks()) / 1000.0f;
	}

	// Bind the default framebuffer.
	void bindDefaultFramebuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Unbind the default framebuffer.
	void unbindDefaultFramebuffer() {
		return;
	}

	// Prepare the frame.
	void prepare() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	// Render a textured model.
	template<typename T>
	void renderTexturedModel(TexturedModel& model, T& shader) {
		glBindVertexArray(model.model.vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		shader.setUniformSampler2D(shader.uTexture, GL_TEXTURE0, model.texture.textureID);

		glDrawArrays(GL_TRIANGLES, 0, model.model.vertexCount);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	// Render a terrain object.
	template<typename T>
	void renderTerrainObject(TerrainObject& model, T& shader) {
		glBindVertexArray(model.model.vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		shader.setUniformSampler2D(shader.uTexture1, GL_TEXTURE0, model.texture1.textureID);
		shader.setUniformSampler2D(shader.uTexture2, GL_TEXTURE1, model.texture2.textureID);
		shader.setUniformSampler2D(shader.uTexture3, GL_TEXTURE2, model.texture3.textureID);
		shader.setUniformSampler2D(shader.uHeightmap, GL_TEXTURE3, model.heightmap.textureID);

		glDrawArrays(GL_TRIANGLES, 0, model.model.vertexCount);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	// Render an entity.
	template<typename T>
	void renderEntity(Entity& entity, T& shader) {
		glBindVertexArray(entity.model.model.vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		shader.setUniformSampler2D(shader.uTexture, GL_TEXTURE0, entity.model.texture.textureID);

		glm::mat4 transformation = createTransformation(
			entity.position,
			entity.rotation,
			entity.scale
		);
		shader.setModel(transformation);

		glDrawArrays(GL_TRIANGLES, 0, entity.model.model.vertexCount);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	// Render entities.
	template<typename T>
	void renderEntities(std::vector<Entity> entities, T& shader) {
		if (entities.empty()) {
			return;
		}

		glBindVertexArray(entities[0].model.model.vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		shader.setUniformSampler2D(shader.uTexture, GL_TEXTURE0, entities[0].model.texture.textureID);

		for (auto& entity: entities) {
			glm::mat4 transformation = createTransformation(
				entity.position,
				entity.rotation,
				entity.scale
			);
			shader.setModel(transformation);

			glDrawArrays(GL_TRIANGLES, 0, entity.model.model.vertexCount);
		}

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	// Render a cubemap.
	template<typename T>
	void renderCubemap(Cubemap& cubemap, T& shader) {
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(cubemap.modelID);
		glEnableVertexAttribArray(0);
		shader.setUniformSamplerCube(cubemap.cubemapID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
	}

	// Render a quad.
	template<typename T>
	void renderQuad(Model& model, GLuint textureID, T& shader) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBindVertexArray(model.vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		shader.setUniformSampler2D(shader.uTexture, GL_TEXTURE0, textureID);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}

	// Render an untextured quad.
	void renderUntexturedQuad(Model& model) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBindVertexArray(model.vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}

	// Render water.
	template<typename T>
	void renderWaterObject(WaterObject& model, T& shader) {
		glBindVertexArray(model.model.vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		shader.setUniformSampler2D(shader.uReflectionTexture, GL_TEXTURE0, model.reflectionTextureID);
		shader.setUniformSampler2D(shader.uRefractionTexture, GL_TEXTURE1, model.refractionTextureID);
		shader.setUniformSampler2D(shader.uDepthTexture, GL_TEXTURE2, model.refractionDepthTextureID);
		shader.setUniformSampler2D(shader.uWaterDuDv, GL_TEXTURE3, model.waterDuDv.textureID);
		shader.setUniformSampler2D(shader.uWaterNormal, GL_TEXTURE4, model.waterNormal.textureID);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_TRIANGLES, 0, model.model.vertexCount);
		glDisable(GL_BLEND);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	// Prepare ImGui.
	void prepareImGui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(display.window);
		ImGui::NewFrame();
	}

	// Render ImGui.
	void renderImGui() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// Update the frame.
	void update(int fps) {
		display.update(fps);
	}

	// Destroy the renderer.
	void destroy() {
		display.destroy();
	}
};