# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build"

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/lib/glad/glad.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/lib/glad/glad.c.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/lib/glad/glad.c
CMakeFiles/client.dir/lib/glad/glad.c.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/client.dir/lib/glad/glad.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/client.dir/lib/glad/glad.c.o -MF CMakeFiles/client.dir/lib/glad/glad.c.o.d -o CMakeFiles/client.dir/lib/glad/glad.c.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/lib/glad/glad.c"

CMakeFiles/client.dir/lib/glad/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/client.dir/lib/glad/glad.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/lib/glad/glad.c" > CMakeFiles/client.dir/lib/glad/glad.c.i

CMakeFiles/client.dir/lib/glad/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/client.dir/lib/glad/glad.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/lib/glad/glad.c" -o CMakeFiles/client.dir/lib/glad/glad.c.s

CMakeFiles/client.dir/src/core/camera.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/camera.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/camera.cpp
CMakeFiles/client.dir/src/core/camera.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client.dir/src/core/camera.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/camera.cpp.o -MF CMakeFiles/client.dir/src/core/camera.cpp.o.d -o CMakeFiles/client.dir/src/core/camera.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/camera.cpp"

CMakeFiles/client.dir/src/core/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/camera.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/camera.cpp" > CMakeFiles/client.dir/src/core/camera.cpp.i

CMakeFiles/client.dir/src/core/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/camera.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/camera.cpp" -o CMakeFiles/client.dir/src/core/camera.cpp.s

CMakeFiles/client.dir/src/core/chunk.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/chunk.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/chunk.cpp
CMakeFiles/client.dir/src/core/chunk.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client.dir/src/core/chunk.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/chunk.cpp.o -MF CMakeFiles/client.dir/src/core/chunk.cpp.o.d -o CMakeFiles/client.dir/src/core/chunk.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/chunk.cpp"

CMakeFiles/client.dir/src/core/chunk.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/chunk.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/chunk.cpp" > CMakeFiles/client.dir/src/core/chunk.cpp.i

CMakeFiles/client.dir/src/core/chunk.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/chunk.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/chunk.cpp" -o CMakeFiles/client.dir/src/core/chunk.cpp.s

CMakeFiles/client.dir/src/core/indexBuffer.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/indexBuffer.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/indexBuffer.cpp
CMakeFiles/client.dir/src/core/indexBuffer.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/client.dir/src/core/indexBuffer.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/indexBuffer.cpp.o -MF CMakeFiles/client.dir/src/core/indexBuffer.cpp.o.d -o CMakeFiles/client.dir/src/core/indexBuffer.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/indexBuffer.cpp"

CMakeFiles/client.dir/src/core/indexBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/indexBuffer.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/indexBuffer.cpp" > CMakeFiles/client.dir/src/core/indexBuffer.cpp.i

CMakeFiles/client.dir/src/core/indexBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/indexBuffer.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/indexBuffer.cpp" -o CMakeFiles/client.dir/src/core/indexBuffer.cpp.s

CMakeFiles/client.dir/src/core/player.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/player.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/player.cpp
CMakeFiles/client.dir/src/core/player.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/client.dir/src/core/player.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/player.cpp.o -MF CMakeFiles/client.dir/src/core/player.cpp.o.d -o CMakeFiles/client.dir/src/core/player.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/player.cpp"

CMakeFiles/client.dir/src/core/player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/player.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/player.cpp" > CMakeFiles/client.dir/src/core/player.cpp.i

CMakeFiles/client.dir/src/core/player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/player.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/player.cpp" -o CMakeFiles/client.dir/src/core/player.cpp.s

CMakeFiles/client.dir/src/core/random.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/random.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/random.cpp
CMakeFiles/client.dir/src/core/random.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/client.dir/src/core/random.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/random.cpp.o -MF CMakeFiles/client.dir/src/core/random.cpp.o.d -o CMakeFiles/client.dir/src/core/random.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/random.cpp"

CMakeFiles/client.dir/src/core/random.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/random.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/random.cpp" > CMakeFiles/client.dir/src/core/random.cpp.i

CMakeFiles/client.dir/src/core/random.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/random.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/random.cpp" -o CMakeFiles/client.dir/src/core/random.cpp.s

CMakeFiles/client.dir/src/core/renderer.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/renderer.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/renderer.cpp
CMakeFiles/client.dir/src/core/renderer.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/client.dir/src/core/renderer.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/renderer.cpp.o -MF CMakeFiles/client.dir/src/core/renderer.cpp.o.d -o CMakeFiles/client.dir/src/core/renderer.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/renderer.cpp"

CMakeFiles/client.dir/src/core/renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/renderer.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/renderer.cpp" > CMakeFiles/client.dir/src/core/renderer.cpp.i

CMakeFiles/client.dir/src/core/renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/renderer.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/renderer.cpp" -o CMakeFiles/client.dir/src/core/renderer.cpp.s

CMakeFiles/client.dir/src/core/shader.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/shader.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/shader.cpp
CMakeFiles/client.dir/src/core/shader.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/client.dir/src/core/shader.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/shader.cpp.o -MF CMakeFiles/client.dir/src/core/shader.cpp.o.d -o CMakeFiles/client.dir/src/core/shader.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/shader.cpp"

CMakeFiles/client.dir/src/core/shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/shader.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/shader.cpp" > CMakeFiles/client.dir/src/core/shader.cpp.i

CMakeFiles/client.dir/src/core/shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/shader.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/shader.cpp" -o CMakeFiles/client.dir/src/core/shader.cpp.s

CMakeFiles/client.dir/src/core/stb_image.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/stb_image.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/stb_image.cpp
CMakeFiles/client.dir/src/core/stb_image.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/client.dir/src/core/stb_image.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/stb_image.cpp.o -MF CMakeFiles/client.dir/src/core/stb_image.cpp.o.d -o CMakeFiles/client.dir/src/core/stb_image.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/stb_image.cpp"

CMakeFiles/client.dir/src/core/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/stb_image.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/stb_image.cpp" > CMakeFiles/client.dir/src/core/stb_image.cpp.i

CMakeFiles/client.dir/src/core/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/stb_image.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/stb_image.cpp" -o CMakeFiles/client.dir/src/core/stb_image.cpp.s

CMakeFiles/client.dir/src/core/texture.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/texture.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/texture.cpp
CMakeFiles/client.dir/src/core/texture.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/client.dir/src/core/texture.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/texture.cpp.o -MF CMakeFiles/client.dir/src/core/texture.cpp.o.d -o CMakeFiles/client.dir/src/core/texture.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/texture.cpp"

CMakeFiles/client.dir/src/core/texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/texture.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/texture.cpp" > CMakeFiles/client.dir/src/core/texture.cpp.i

CMakeFiles/client.dir/src/core/texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/texture.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/texture.cpp" -o CMakeFiles/client.dir/src/core/texture.cpp.s

CMakeFiles/client.dir/src/core/vertexArray.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/vertexArray.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/vertexArray.cpp
CMakeFiles/client.dir/src/core/vertexArray.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/client.dir/src/core/vertexArray.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/vertexArray.cpp.o -MF CMakeFiles/client.dir/src/core/vertexArray.cpp.o.d -o CMakeFiles/client.dir/src/core/vertexArray.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/vertexArray.cpp"

CMakeFiles/client.dir/src/core/vertexArray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/vertexArray.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/vertexArray.cpp" > CMakeFiles/client.dir/src/core/vertexArray.cpp.i

CMakeFiles/client.dir/src/core/vertexArray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/vertexArray.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/vertexArray.cpp" -o CMakeFiles/client.dir/src/core/vertexArray.cpp.s

CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/vertexBuffer.cpp
CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o -MF CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o.d -o CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/vertexBuffer.cpp"

CMakeFiles/client.dir/src/core/vertexBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/vertexBuffer.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/vertexBuffer.cpp" > CMakeFiles/client.dir/src/core/vertexBuffer.cpp.i

CMakeFiles/client.dir/src/core/vertexBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/vertexBuffer.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/vertexBuffer.cpp" -o CMakeFiles/client.dir/src/core/vertexBuffer.cpp.s

CMakeFiles/client.dir/src/core/world.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/core/world.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/core/world.cpp
CMakeFiles/client.dir/src/core/world.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/client.dir/src/core/world.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/core/world.cpp.o -MF CMakeFiles/client.dir/src/core/world.cpp.o.d -o CMakeFiles/client.dir/src/core/world.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/world.cpp"

CMakeFiles/client.dir/src/core/world.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/core/world.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/world.cpp" > CMakeFiles/client.dir/src/core/world.cpp.i

CMakeFiles/client.dir/src/core/world.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/core/world.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/core/world.cpp" -o CMakeFiles/client.dir/src/core/world.cpp.s

CMakeFiles/client.dir/src/client/client.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/client/client.cpp.o: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/src/client/client.cpp
CMakeFiles/client.dir/src/client/client.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/client.dir/src/client/client.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/client/client.cpp.o -MF CMakeFiles/client.dir/src/client/client.cpp.o.d -o CMakeFiles/client.dir/src/client/client.cpp.o -c "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/client/client.cpp"

CMakeFiles/client.dir/src/client/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/src/client/client.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/client/client.cpp" > CMakeFiles/client.dir/src/client/client.cpp.i

CMakeFiles/client.dir/src/client/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/client/client.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/src/client/client.cpp" -o CMakeFiles/client.dir/src/client/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/lib/glad/glad.c.o" \
"CMakeFiles/client.dir/src/core/camera.cpp.o" \
"CMakeFiles/client.dir/src/core/chunk.cpp.o" \
"CMakeFiles/client.dir/src/core/indexBuffer.cpp.o" \
"CMakeFiles/client.dir/src/core/player.cpp.o" \
"CMakeFiles/client.dir/src/core/random.cpp.o" \
"CMakeFiles/client.dir/src/core/renderer.cpp.o" \
"CMakeFiles/client.dir/src/core/shader.cpp.o" \
"CMakeFiles/client.dir/src/core/stb_image.cpp.o" \
"CMakeFiles/client.dir/src/core/texture.cpp.o" \
"CMakeFiles/client.dir/src/core/vertexArray.cpp.o" \
"CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o" \
"CMakeFiles/client.dir/src/core/world.cpp.o" \
"CMakeFiles/client.dir/src/client/client.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/lib/glad/glad.c.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/camera.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/chunk.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/indexBuffer.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/player.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/random.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/renderer.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/shader.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/stb_image.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/texture.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/vertexArray.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/vertexBuffer.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/core/world.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/src/client/client.cpp.o
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/build.make
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: /usr/lib/x86_64-linux-gnu/libSDL2.so
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/lib/enet-1.3.18/libenet.a
/media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX executable \"/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/bin/Linux/Release/client\""
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: /media/bertie-cartwright/External\ 4TB\ HDD/sunst/Documents/C++/Minecraft/Minecraft\ VS\ Code/bin/Linux/Release/client
.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code" "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code" "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build" "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build" "/media/bertie-cartwright/External 4TB HDD/sunst/Documents/C++/Minecraft/Minecraft VS Code/build/CMakeFiles/client.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/client.dir/depend

