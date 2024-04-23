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
CMAKE_SOURCE_DIR = /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out

# Include any dependencies generated for this target.
include framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/compiler_depend.make

# Include the progress variables for this target.
include framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/progress.make

# Include the compile flags for this target's objects.
include framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/flags.make

framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o: framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/flags.make
framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o: ../framework/tf_plugin/tensorflow_fast_gelu_grad_plugin.cc
framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o: framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o"
	cd /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/framework/tf_plugin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o -MF CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o.d -o CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o -c /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/framework/tf_plugin/tensorflow_fast_gelu_grad_plugin.cc

framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.i"
	cd /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/framework/tf_plugin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/framework/tf_plugin/tensorflow_fast_gelu_grad_plugin.cc > CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.i

framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.s"
	cd /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/framework/tf_plugin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/framework/tf_plugin/tensorflow_fast_gelu_grad_plugin.cc -o CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.s

# Object files for target cust_tf_parsers
cust_tf_parsers_OBJECTS = \
"CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o"

# External object files for target cust_tf_parsers
cust_tf_parsers_EXTERNAL_OBJECTS =

framework/tf_plugin/libcust_tf_parsers.so: framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/tensorflow_fast_gelu_grad_plugin.cc.o
framework/tf_plugin/libcust_tf_parsers.so: framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/build.make
framework/tf_plugin/libcust_tf_parsers.so: framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libcust_tf_parsers.so"
	cd /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/framework/tf_plugin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cust_tf_parsers.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/build: framework/tf_plugin/libcust_tf_parsers.so
.PHONY : framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/build

framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/clean:
	cd /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/framework/tf_plugin && $(CMAKE_COMMAND) -P CMakeFiles/cust_tf_parsers.dir/cmake_clean.cmake
.PHONY : framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/clean

framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/depend:
	cd /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/framework/tf_plugin /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/framework/tf_plugin /home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : framework/tf_plugin/CMakeFiles/cust_tf_parsers.dir/depend
