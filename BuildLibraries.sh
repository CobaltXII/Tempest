mkdir bin
echo "Building glad"
clang -c src/glad/glad.c -o bin/glad.o -O3
echo "Building stb_image"
clang -c src/stb/stb_image.c -o bin/stb_image.o -O3
echo "Building stb_image_write"
clang -c src/stb/stb_image_write.c -o bin/stb_image_write.o -O3
echo "Building tiny_obj_loader"
clang++ -std=c++11 -c src/tinyobjloader/tiny_obj_loader.cc -o bin/tiny_obj_loader.o -O3
echo "Building FastNoise"
clang++ -std=c++11 -c src/FastNoise/FastNoise.cpp -o bin/FastNoise.o -O3
echo "Building ImGui"
clang++ -std=c++11 -c src/imgui/imgui.cpp -o bin/imgui.o -O3
clang++ -std=c++11 -c src/imgui/imgui_draw.cpp -o bin/imgui_draw.o -O3
clang++ -std=c++11 -c src/imgui/imgui_widgets.cpp -o bin/imgui_widgets.o -O3
clang++ -std=c++11 -c src/imgui/imgui_impl_sdl.cpp -o bin/imgui_impl_sdl.o -O3 `sdl2-config --cflags`
clang++ -std=c++11 -c src/imgui/imgui_impl_opengl3.cpp -o bin/imgui_impl_opengl3.o -O3