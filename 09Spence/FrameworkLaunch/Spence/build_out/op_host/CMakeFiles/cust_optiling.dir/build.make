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
CMAKE_SOURCE_DIR = /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out

# Include any dependencies generated for this target.
include op_host/CMakeFiles/cust_optiling.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include op_host/CMakeFiles/cust_optiling.dir/compiler_depend.make

# Include the progress variables for this target.
include op_host/CMakeFiles/cust_optiling.dir/progress.make

# Include the compile flags for this target's objects.
include op_host/CMakeFiles/cust_optiling.dir/flags.make

op_host/CMakeFiles/cust_optiling.dir/spence.cpp.o: op_host/CMakeFiles/cust_optiling.dir/flags.make
op_host/CMakeFiles/cust_optiling.dir/spence.cpp.o: ../op_host/spence.cpp
op_host/CMakeFiles/cust_optiling.dir/spence.cpp.o: op_host/CMakeFiles/cust_optiling.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object op_host/CMakeFiles/cust_optiling.dir/spence.cpp.o"
	cd /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/op_host && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT op_host/CMakeFiles/cust_optiling.dir/spence.cpp.o -MF CMakeFiles/cust_optiling.dir/spence.cpp.o.d -o CMakeFiles/cust_optiling.dir/spence.cpp.o -c /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/op_host/spence.cpp

op_host/CMakeFiles/cust_optiling.dir/spence.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cust_optiling.dir/spence.cpp.i"
	cd /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/op_host && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/op_host/spence.cpp > CMakeFiles/cust_optiling.dir/spence.cpp.i

op_host/CMakeFiles/cust_optiling.dir/spence.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cust_optiling.dir/spence.cpp.s"
	cd /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/op_host && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/op_host/spence.cpp -o CMakeFiles/cust_optiling.dir/spence.cpp.s

# Object files for target cust_optiling
cust_optiling_OBJECTS = \
"CMakeFiles/cust_optiling.dir/spence.cpp.o"

# External object files for target cust_optiling
cust_optiling_EXTERNAL_OBJECTS =

op_host/libcust_opmaster_rt2.0.so: op_host/CMakeFiles/cust_optiling.dir/spence.cpp.o
op_host/libcust_opmaster_rt2.0.so: op_host/CMakeFiles/cust_optiling.dir/build.make
op_host/libcust_opmaster_rt2.0.so: op_host/CMakeFiles/cust_optiling.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libcust_opmaster_rt2.0.so"
	cd /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/op_host && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cust_optiling.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
op_host/CMakeFiles/cust_optiling.dir/build: op_host/libcust_opmaster_rt2.0.so
.PHONY : op_host/CMakeFiles/cust_optiling.dir/build

op_host/CMakeFiles/cust_optiling.dir/clean:
	cd /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/op_host && $(CMAKE_COMMAND) -P CMakeFiles/cust_optiling.dir/cmake_clean.cmake
.PHONY : op_host/CMakeFiles/cust_optiling.dir/clean

op_host/CMakeFiles/cust_optiling.dir/depend:
	cd /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/op_host /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/op_host /home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/op_host/CMakeFiles/cust_optiling.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : op_host/CMakeFiles/cust_optiling.dir/depend

