# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build

# Include any dependencies generated for this target.
include cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/compiler_depend.make

# Include the progress variables for this target.
include cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/progress.make

# Include the compile flags for this target's objects.
include cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/flags.make

cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o: cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/flags.make
cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o: ../main.cpp
cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o: cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o"
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o -MF CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o.d -o CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o -c /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/main.cpp

cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.i"
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/main.cpp > CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.i

cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.s"
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/main.cpp -o CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.s

cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o: cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/flags.make
cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o: ../mseloss.cpp
cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o: cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o"
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o -MF CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o.d -o CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o -c /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/mseloss.cpp

cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.i"
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/mseloss.cpp > CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.i

cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.s"
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/mseloss.cpp -o CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.s

# Object files for target addcdiv_custom_cpu
addcdiv_custom_cpu_OBJECTS = \
"CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o" \
"CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o"

# External object files for target addcdiv_custom_cpu
addcdiv_custom_cpu_EXTERNAL_OBJECTS =

../addcdiv_custom_cpu: cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/main.cpp.o
../addcdiv_custom_cpu: cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/__/__/mseloss.cpp.o
../addcdiv_custom_cpu: cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/build.make
../addcdiv_custom_cpu: /usr/local/Ascend/ascend-toolkit/latest/tools/tikicpulib/lib/libtikicpulib_cceprint.so
../addcdiv_custom_cpu: /usr/local/Ascend/ascend-toolkit/latest/tools/tikicpulib/lib/libtikicpulib_npuchk.so
../addcdiv_custom_cpu: /usr/local/Ascend/ascend-toolkit/latest/tools/tikicpulib/lib/libtikicpulib_stubreg.so
../addcdiv_custom_cpu: cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../addcdiv_custom_cpu"
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/addcdiv_custom_cpu.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/build: ../addcdiv_custom_cpu
.PHONY : cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/build

cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/clean:
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu && $(CMAKE_COMMAND) -P CMakeFiles/addcdiv_custom_cpu.dir/cmake_clean.cmake
.PHONY : cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/clean

cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/depend:
	cd /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/cmake/cpu /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu /home/wanghs/example/samples/operator/MseLoss/KernelLaunch/MseLossKernelInvocation/build/cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cmake/cpu/CMakeFiles/addcdiv_custom_cpu.dir/depend

