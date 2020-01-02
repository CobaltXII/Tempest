#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <SDL.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "tinyobjloader/tiny_obj_loader.h"
#include "FastNoise/FastNoise.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Utility.hpp"
#include "Display.hpp"
#include "Model.hpp"
#include "Texture.hpp"
#include "Cubemap.hpp"
#include "Framebuffer.hpp"
#include "TexturedModel.hpp"
#include "Entity.hpp"
#include "Heightmap.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "shaders/DiffuseShader.hpp"
#include "shaders/SkyboxShader.hpp"
#include "shaders/TerrainShader.hpp"
#include "shaders/WaterShader.hpp"
#include "shaders/QuadShader.hpp"
#include "shaders/FilterShader.hpp"
#include "shaders/LumaShader.hpp"
#include "shaders/BlurShader.hpp"
#include "shaders/BloomShader.hpp"
#include "Wavefront.hpp"
#include "Manager.hpp"
#include "Terrain.hpp"
#include "Water.hpp"
#include "Renderer.hpp"

int main() {
	// Create a renderer.
	const int displayWidth = 1280;
	const int displayHeight = 720;
	Renderer renderer("OpenGL", displayWidth, displayHeight);

	// Load shaders.
	DiffuseShader diffuseShader;
	SkyboxShader skyboxShader;
	TerrainShader terrainShader;
	WaterShader waterShader;
	QuadShader quadShader;
	FilterShader filterShader;
	LumaShader lumaShader;
	BlurShader blurShader;
	BloomShader bloomShader;

	// Load the skybox.
	Cubemap skybox = Manager::loadCubemap({
		"res/skybox/bluecloud_ft.jpg",
		"res/skybox/bluecloud_bk.jpg",
		"res/skybox/bluecloud_up.jpg",
		"res/skybox/bluecloud_dn.jpg",
		"res/skybox/bluecloud_rt.jpg",
		"res/skybox/bluecloud_lf.jpg"
	});

	// Load textures.
	Texture treeTexture = Manager::loadTexture("res/models/Tree.png");
	Texture houseTexture = Manager::loadTexture("res/models/House.png");
	Texture terrainTexture1 = Manager::loadTexture("res/Terrain1.png");
	Texture terrainTexture2 = Manager::loadTexture("res/Terrain2.png");
	Texture terrainTexture3 = Manager::loadTexture("res/Terrain3.png");
	Texture waterDuDvTexture = Manager::loadTexture("res/WaterDuDv.png");
	Texture waterNormalTexture = Manager::loadTexture("res/WaterNormal.png");

	// Load models.
	Model treeModel = Manager::loadModel("res/models/Tree.obj");
	Model houseModel = Manager::loadModel("res/models/House.obj");

	// Link models with their textures.
	TexturedModel treeEntity(treeModel, treeTexture);
	TexturedModel houseEntity(houseModel, houseTexture);

	// Create terrain.
	const float terrainSize = 500.0f;
	const int terrainResolution = 384;
	TerrainObject terrainObject = Terrain::generateTerrain(
		terrainSize,
		terrainResolution,
		terrainTexture1,
		terrainTexture2,
		terrainTexture3
	);

	// Create water.
	float waterLevel = 0.0f;
	WaterObject waterObject = Water::generateWater(
		terrainSize,
		renderer.width,
		renderer.height,
		waterDuDvTexture,
		waterNormalTexture
	);

	// // Generate trees.
	std::vector<Entity> treeEntities;
	for (int i = 0; i < 1000; i++) {
		int attempts = 0;
		tree:
		attempts++;
		float x = signedRandomFloat() * terrainSize;
		float z = signedRandomFloat() * terrainSize;
		float y = terrainObject.sample(x, z);
		if (y < waterLevel + 10.0f) {
			if (attempts >= 10) {
				continue;
			}
			goto tree;
		}
		glm::vec3 position = glm::vec3(x, y, z);
		glm::vec3 rotation = glm::vec3(0.0f, glm::radians(randomFloat() * 360.0f), 0.0f);
		glm::vec3 scale = glm::vec3(randomFloat() + 1.0f);
		treeEntities.push_back(Entity(treeEntity, position, rotation, scale));
	}

	// // Generate houses.
	std::vector<Entity> houseEntities;
	for (int i = 0; i < 10; i++) {
		int attempts = 0;
		house:
		attempts++;
		float x = signedRandomFloat() * terrainSize;
		float z = signedRandomFloat() * terrainSize;
		float y = terrainObject.sample(x, z);
		if (y < waterLevel + 10.0f) {
			if (attempts >= 10) {
				continue;
			}
			goto house;
		}
		glm::vec3 position = glm::vec3(x, y, z);
		glm::vec3 rotation = glm::vec3(0.0f, glm::radians(randomFloat() * 360.0f), 0.0f);
		glm::vec3 scale = glm::vec3(10.0f);
		houseEntities.push_back(Entity(houseEntity, position, rotation, scale));
	}

	// Create framebuffers for post-processing.
	Framebuffer postProcessingSource = Manager::createFramebuffer(renderer.width / 2, renderer.height / 2, GL_NEAREST);
	Framebuffer postProcessingLuma = Manager::createFramebuffer(renderer.width / 4, renderer.height / 4, GL_LINEAR, GL_CLAMP_TO_EDGE);
	Framebuffer postProcessingBlurX = Manager::createFramebuffer(renderer.width / 4, renderer.height / 4, GL_LINEAR, GL_CLAMP_TO_EDGE);
	Framebuffer postProcessingBlurY = Manager::createFramebuffer(renderer.width / 4, renderer.height / 4, GL_LINEAR, GL_CLAMP_TO_EDGE);
	Framebuffer postProcessingBloom = Manager::createFramebuffer(renderer.width / 2, renderer.height / 2, GL_NEAREST);

	// Create a full-screen quad for post-processing.
	Model fullScreenQuad = Manager::createQuad(-1.0f, -1.0f, 2.0f, 2.0f);

	// Create a light.
	Light light(
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	// Create a camera.
	Camera camera(glm::vec3(0.0f, 100.0f, 100.0f), 0.0f, 0.0f, 0.0f);

	// Draw state.
	bool drawTrees = false;
	bool drawHouses = false;
	bool drawImGui = true;

	// Render the scene.
	auto RenderScene = [&](glm::vec4 clipPlane = glm::vec4(0.0f)) {
		// Render the skybox.
		{
			// Enable the shader.
			skyboxShader.enable();
			skyboxShader.setProjection(renderer.projection);
			skyboxShader.setView(camera.getCubemapView());
			skyboxShader.setLight(light);

			// Render the skybox.
			renderer.renderCubemap(skybox, skyboxShader);

			// Disable the shader.
			skyboxShader.disable();
		}

		// Render the diffuse objects.
		{
			// Enable the shader.
			diffuseShader.enable();
			diffuseShader.setProjection(renderer.projection);
			diffuseShader.setView(camera.getView());
			diffuseShader.setLight(light);
			diffuseShader.setClipPlane(clipPlane);

			if (drawTrees) {
				// Render the trees.
				renderer.renderEntities(treeEntities, diffuseShader);
			}
			if (drawHouses) {
				// Render the houses.
				renderer.renderEntities(houseEntities, diffuseShader);
			}

			// Disable the shader.
			diffuseShader.disable();
		}
	};

	// Render the terrain.
	auto RenderTerrain = [&](glm::vec4 clipPlane = glm::vec4(0.0f)) {
		// Enable the shader.
		terrainShader.enable();
		terrainShader.setProjection(renderer.projection);
		terrainShader.setView(camera.getView());
		terrainShader.setLight(light);
		terrainShader.setClipPlane(clipPlane);

		// Render the terrain.
		renderer.renderTerrainObject(terrainObject, terrainShader);

		// Disable the shader.
		terrainShader.disable();
	};

	// Render the water.
	auto RenderWater = [&]() {
		// Enable the shader.
		waterShader.enable();
		waterShader.setProjection(renderer.projection);
		waterShader.setView(camera.getView());
		waterShader.setLight(light);
		waterShader.setCamera(camera);
		waterShader.setTime(renderer.getTime());

		// Render the water.
		renderer.renderWaterObject(waterObject, waterShader);

		// Disable the shader.
		waterShader.disable();
	};

	// Keyboard state.
	bool keyW = false;
	bool keyA = false;
	bool keyS = false;
	bool keyD = false;
	bool keyZ = false;
	bool keyX = false;
	bool keyLeftShift = false;
	bool keyRightShift = false;

	// Mouse state.
	int mouseX = 0;
	int mouseY = 0;
	int mouseDragX = 0;
	int mouseDragY = 0;
	bool mouseButtonLeft = false;
	bool mouseButtonMiddle = false;
	bool mouseButtonRight = false;

	// Cursor state.
	float cursorSize = 10.0f;

	// Game loop.
	bool running = true;
	while (running) {
		// Poll events.
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);

			if (e.type == SDL_QUIT) {
				running = false;
			} else if (e.type == SDL_KEYDOWN) {
				SDL_Keycode key = e.key.keysym.sym;
				if (key == SDLK_w) keyW = true;
				if (key == SDLK_a) keyA = true;
				if (key == SDLK_s) keyS = true;
				if (key == SDLK_d) keyD = true;
				if (key == SDLK_z) keyZ = true;
				if (key == SDLK_x) keyX = true;
				if (key == SDLK_LSHIFT) keyLeftShift = true;
				if (key == SDLK_RSHIFT) keyRightShift = true;
				if (key == SDLK_i) drawImGui = !drawImGui;
			} else if (e.type == SDL_KEYUP) {
				SDL_Keycode key = e.key.keysym.sym;
				if (key == SDLK_w) keyW = false;
				if (key == SDLK_a) keyA = false;
				if (key == SDLK_s) keyS = false;
				if (key == SDLK_d) keyD = false;
				if (key == SDLK_z) keyZ = false;
				if (key == SDLK_x) keyX = false;
				if (key == SDLK_LSHIFT) keyLeftShift = false;
				if (key == SDLK_RSHIFT) keyRightShift = false;
			} else if (e.type == SDL_MOUSEMOTION) {
				mouseX = e.motion.x;
				mouseY = e.motion.y;
				if (mouseButtonMiddle) {
					mouseDragX += e.motion.xrel;
					mouseDragY += e.motion.yrel;
				}
			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				Uint8 button = e.button.button;
				if (button == SDL_BUTTON_LEFT) mouseButtonLeft = true;
				else if (button == SDL_BUTTON_MIDDLE) mouseButtonMiddle = true;
				else if (button == SDL_BUTTON_RIGHT) mouseButtonRight = true;
			} else if (e.type == SDL_MOUSEBUTTONUP) {
				Uint8 button = e.button.button;
				if (button == SDL_BUTTON_LEFT) mouseButtonLeft = false;
				else if (button == SDL_BUTTON_MIDDLE) mouseButtonMiddle = false;
				else if (button == SDL_BUTTON_RIGHT) mouseButtonRight = false;
			} else if (e.type == SDL_MOUSEWHEEL) {
				float cursorSizeDelta = 1.0f;
				if (e.wheel.y > 0) {
					cursorSize += cursorSizeDelta;
				} else if (e.wheel.y < 0) {
					cursorSize -= cursorSizeDelta;
				}
			}
		}

		// Point the camera.
		{
			float u = float(mouseDragX) / float(displayWidth);
			float v = float(mouseDragY) / float(displayHeight);
			camera.pitch = glm::radians((v * 2.0f - 1.0f) * 90.0f);
			camera.yaw = glm::radians((u * 2.0f - 1.0f) * 180.0f + 180.0f);
		}

		// Move the camera.
		{
			float cameraSpeed = 3.0f;
			if (keyLeftShift) {
				cameraSpeed = 10.0f;
			} else if (keyRightShift) {
				cameraSpeed = 0.3f;
			}
			if (keyW) {
				camera.position.x += std::sin(-camera.yaw) * -cameraSpeed;
				camera.position.z += std::cos(-camera.yaw) * -cameraSpeed;
			} else if (keyS) {
				camera.position.x += std::sin(-camera.yaw) * cameraSpeed;
				camera.position.z += std::cos(-camera.yaw) * cameraSpeed;
			}
			if (keyA) {
				camera.position.x += std::sin(-camera.yaw + glm::radians(90.0f)) * -cameraSpeed;
				camera.position.z += std::cos(-camera.yaw + glm::radians(90.0f)) * -cameraSpeed;
			} else if (keyD) {
				camera.position.x += std::sin(-camera.yaw + glm::radians(90.0f)) * cameraSpeed;
				camera.position.z += std::cos(-camera.yaw + glm::radians(90.0f)) * cameraSpeed;
			}
			if (keyZ) {
				camera.position.y += -cameraSpeed;
			} else if (keyX) {
				camera.position.y += cameraSpeed;
			}
		}

		// Prepare the scene.
		renderer.prepare();

		// Render the scene.
		{
			// Do the reflection pass.
			{
				waterObject.bindReflectionFramebuffer();
				float D = 2.0f * (camera.position.y - waterLevel);
				camera.position.y -= D;
				camera.pitch *= -1.0f;
				RenderScene(glm::vec4(0.0f, 1.0f, 0.0f, -waterLevel));
				RenderTerrain(glm::vec4(0.0f, 1.0f, 0.0f, -waterLevel));
				camera.pitch *= -1.0f;
				camera.position.y += D;
				waterObject.unbindReflectionFramebuffer();
			}

			// Do the refraction pass.
			{
				waterObject.bindRefractionFramebuffer();
				RenderScene(glm::vec4(0.0f, -1.0f, 0.0f, waterLevel + 10.0f));
				RenderTerrain(glm::vec4(0.0f, -1.0f, 0.0f, waterLevel + 10.0f));
				waterObject.unbindRefractionFramebuffer();
			}

			// Render to the source post-processing buffer.
			postProcessingSource.bind();
			RenderScene();
			RenderTerrain();
			RenderWater();
			postProcessingSource.unbind();

			// Do the luma pass.
			lumaShader.enable();
			postProcessingLuma.bind();
			renderer.renderQuad(fullScreenQuad, postProcessingSource.textureID, lumaShader);
			postProcessingLuma.unbind();
			lumaShader.disable();

			// Do the blur pass.
			blurShader.enable();
			blurShader.modeHorizontal();
			postProcessingBlurX.bind();
			renderer.renderQuad(fullScreenQuad, postProcessingLuma.textureID, blurShader);
			postProcessingBlurX.unbind();
			blurShader.modeVertical();
			postProcessingBlurY.bind();
			renderer.renderQuad(fullScreenQuad, postProcessingBlurX.textureID, blurShader);
			postProcessingBlurY.unbind();
			blurShader.disable();

			// Do the bloom pass.
			bloomShader.enable();
			bloomShader.setTextures(postProcessingSource.textureID, postProcessingBlurY.textureID);
			postProcessingBloom.bind();
			renderer.renderUntexturedQuad(fullScreenQuad);
			postProcessingBloom.unbind();
			bloomShader.disable();

			// Do post-processing.
			filterShader.enable();
			renderer.bindDefaultFramebuffer();
			renderer.renderQuad(fullScreenQuad, postProcessingBloom.textureID, lumaShader);
			renderer.unbindDefaultFramebuffer();
			filterShader.disable();
		}

		// Render the GUI.
		if (drawImGui) {
			bool takeScreenshotWithImGui = false;

			renderer.prepareImGui();

			ImGui::Begin("Light"); {
				ImGui::PushItemWidth(150);

				static float azimuth = 220.0f;
				static float altitude = 70.0f;
				ImGui::SliderFloat("Light Azimuth", &azimuth, 0.0f, 360.0f);
				ImGui::SliderFloat("Light Altitude", &altitude, 0.0f, 360.0f);

				light.direction = glm::normalize(glm::vec3(
					std::cos(glm::radians(azimuth)) * std::cos(glm::radians(altitude)),
					std::sin(glm::radians(altitude)),
					std::sin(glm::radians(azimuth)) * std::cos(glm::radians(altitude))
				));

				static float color[3] = {1.0f, 1.0f, 1.0f};
				ImGui::ColorPicker3("Light Color", color);

				light.color = glm::vec3(
					color[0],
					color[1],
					color[2]
				);

				static float sunFalloff = 1000.0f;
				static float sunIntensity = 1.0f;
				ImGui::SliderFloat("Sun Falloff", &sunFalloff, 1.0f, 1000.0f);
				ImGui::SliderFloat("Sun Intensity", &sunIntensity, 1.0f, 100.0f);

				skyboxShader.enable();
				skyboxShader.setSun(sunFalloff, sunIntensity);
				skyboxShader.disable();

				ImGui::PopItemWidth();
			}
			ImGui::End();

			ImGui::Begin("Effects"); {
				ImGui::PushItemWidth(200);

				// Fog settings.
				ImGui::Text("Fog Settings"); {
					static float fogDensity = 0.009f;
					static float fogGradient = 10.0f;
					ImGui::SliderFloat("Fog Density", &fogDensity, 0.0f, 0.1f);
					ImGui::SliderFloat("Fog Gradient", &fogGradient, 0.0f, 10.0f);

					diffuseShader.enable();
					diffuseShader.setFog(fogDensity / 10.0f, fogGradient);
					diffuseShader.disable();

					terrainShader.enable();
					terrainShader.setFog(fogDensity / 10.0f, fogGradient);
					terrainShader.disable();
				}

				// Filter settings.
				ImGui::Text("Filter Settings"); {
					static float saturation = 0.194f;
					static float temperature = 0.011f;
					ImGui::SliderFloat("Saturation", &saturation, -1.0f, 1.0f);
					ImGui::SliderFloat("Temperature", &temperature, -0.2f, 0.2f);

					filterShader.enable();
					filterShader.setFilters(saturation, temperature);
					filterShader.disable();
				}

				// Bloom settings.
				ImGui::Text("Bloom Settings"); {
					static float threshold = 0.55f;
					ImGui::SliderFloat("Threshold", &threshold, 0.0f, 1.0f);

					lumaShader.enable();
					lumaShader.setThreshold(threshold);
					lumaShader.disable();
				}

				ImGui::PopItemWidth();
			}
			ImGui::End();

			ImGui::Begin("Water"); {
				ImGui::PushItemWidth(150);

				static float waterScale = 2.2f;
				static float waterStrength = 0.02f;
				static float waterSpeed = 0.006f;
				static float waterShinyness = 0.75f;
				static float waterSpecularity = 100.0f;
				static float waterColor[4] = {0.0f, 0.714f, 1.0f, 0.2f};
				static float waterMurky[3] = {0.0f, 0.026f, 0.183f};
				ImGui::SliderFloat("Water Level", &waterLevel, -50.0f, 50.0f);
				ImGui::SliderFloat("Water Scale", &waterScale, 0.0f, 100.0f);
				ImGui::SliderFloat("Water Strength", &waterStrength, 0.0f, 0.1f);
				ImGui::SliderFloat("Water Speed", &waterSpeed, 0.0f, 0.01f);
				ImGui::SliderFloat("Water Shinyness", &waterShinyness, 0.0f, 1.0f);
				ImGui::SliderFloat("Water Specularity", &waterSpecularity, 0.0f, 100.0f);
				ImGui::ColorPicker4("Water Color", waterColor);
				ImGui::SliderFloat("Water Alpha", &waterColor[3], 0.0f, 1.0f);
				ImGui::ColorPicker3("Water Murky", waterMurky);

				ImGui::PopItemWidth();

				waterShader.enable();
				waterShader.setWater(waterLevel, waterScale, waterStrength, waterSpeed, waterShinyness, waterSpecularity, glm::make_vec4(waterColor), glm::make_vec3(waterMurky));
				waterShader.disable();
			}
			ImGui::End();

			ImGui::Begin("Debug"); {
				ImGui::Text("Camera X: %f", camera.position.x);
				ImGui::Text("Camera Y: %f", camera.position.y);
				ImGui::Text("Camera Z: %f", camera.position.z);
				ImGui::Text("Camera Pitch: %f", glm::degrees(camera.pitch));
				ImGui::Text("Camera Yaw: %f", glm::degrees(camera.yaw));
				ImGui::Text("Camera Roll: %f", glm::degrees(camera.roll));

				if (ImGui::Button("Go To Origin")) {
					camera.position.x = 0.0f;
					camera.position.z = 0.0f;
					mouseDragX = 0.0f;
					mouseDragY = displayHeight;
				}

				if (ImGui::Button("Take Screenshot")) {
					unsigned char* data = new unsigned char[renderer.width * renderer.height * 3];
					glReadPixels(0, 0, renderer.width, renderer.height, GL_RGB, GL_UNSIGNED_BYTE, data);
					stbi_flip_vertically_on_write(true);
					stbi_write_png(formatString("screenshot_%d.png", time(NULL)), renderer.width, renderer.height, 3, data, renderer.width * 3);
					delete[] data;
				}

				if (ImGui::Button("Take Screenshot With ImGui")) {
					takeScreenshotWithImGui = true;
				}

				if (ImGui::Button("Regenerate Terrain")) {
					terrainObject.heightmap.regenerate();
				}

				ImGui::Checkbox("Draw Trees", &drawTrees);
				ImGui::Checkbox("Draw Houses", &drawHouses);
			}
			ImGui::End();

			ImGui::Begin("Editor"); {
				ImGui::PushItemWidth(150);

				static const char* modes[] = {
					"Sculpt",
					"Average"
				};
				static int mode = 0;
				ImGui::Combo("Mode", &mode, modes, IM_ARRAYSIZE(modes));

				static float power = 0.5f;
				ImGui::SliderFloat("Power", &power, 0.0f, 1.0f);

				ImGui::PopItemWidth();

				glm::vec3 mouseRay = renderer.getMouseRay(camera, mouseX, mouseY);
				float timeOfIntersection = terrainObject.raycast(camera.position, mouseRay);
				if (timeOfIntersection > 0.0f) {
					glm::vec3 pointOfIntersection = camera.position + mouseRay * timeOfIntersection;
					terrainShader.enable();
					terrainShader.setCursor(glm::vec2(pointOfIntersection.x, pointOfIntersection.z), cursorSize);
					terrainShader.disable();

					if (!ImGui::GetIO().WantCaptureMouse) {
						if (mode == 0) {
							if (mouseButtonLeft) {
								terrainObject.addHeight(pointOfIntersection.x, pointOfIntersection.z, cursorSize, power);
								terrainObject.update();
							} else if (mouseButtonRight) {
								terrainObject.addHeight(pointOfIntersection.x, pointOfIntersection.z, cursorSize, -power);
								terrainObject.update();
							}
						} else if (mode == 1) {
							if (mouseButtonLeft) {
								terrainObject.averageHeight(pointOfIntersection.x, pointOfIntersection.z, cursorSize, power);
								terrainObject.update();
							}
						}
					}

					for (auto& entity: treeEntities) {
						float y = terrainObject.sample(entity.position.x, entity.position.z);
						if (y < waterLevel + 10.0f) {
							y = -10000.0f;
						}
						entity.position.y = y;
					}
					for (auto& entity: houseEntities) {
						float y = terrainObject.sample(entity.position.x, entity.position.z);
						if (y < waterLevel + 10.0f) {
							y = -10000.0f;
						}
						entity.position.y = y;
					}
				}
			}
			ImGui::End();

			renderer.renderImGui();

			if (takeScreenshotWithImGui) {
				unsigned char* data = new unsigned char[renderer.width * renderer.height * 3];
				glReadPixels(0, 0, renderer.width, renderer.height, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_flip_vertically_on_write(true);
				stbi_write_png(formatString("screenshot_%d.png", time(NULL)), renderer.width, renderer.height, 3, data, renderer.width * 3);
				delete[] data;
			}
		}

		// Update the renderer.
		renderer.update(8008135);
	}

	// Destroy unmanaged resources.
	diffuseShader.destroy();
	skyboxShader.destroy();
	terrainShader.destroy();
	waterShader.destroy();
	quadShader.destroy();
	filterShader.destroy();
	lumaShader.destroy();
	blurShader.destroy();
	bloomShader.destroy();
	renderer.destroy();

	// Clean up and exit.
	Manager::cleanUp();
	return 0;
}