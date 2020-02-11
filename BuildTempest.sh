clang++ -std=c++11 src/Tempest.cpp \
bin/glad.o \
bin/stb_image.o \
bin/stb_image_write.o \
bin/tiny_obj_loader.o \
bin/FastNoise.o \
bin/imgui.o \
bin/imgui_draw.o \
bin/imgui_widgets.o \
bin/imgui_impl_sdl.o \
bin/imgui_impl_opengl3.o \
-o Tempest.o -Wall `sdl2-config --cflags --libs` -framework OpenGL -framework Cocoa -O3 && ./Tempest.o