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
CMAKE_SOURCE_DIR = /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out

# Utility rule file for ascendc_bin_ascend310b_scatter_sub_1.

# Include any custom commands dependencies for this target.
include op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/compiler_depend.make

# Include the progress variables for this target.
include op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/progress.make

op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1:
	cd /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b && export HI_PYTHON=python3 && bash /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b/gen/ScatterSub-scatter_sub-1.sh /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b/src/ScatterSub.py /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b/bin/scatter_sub && echo $(MAKE)

ascendc_bin_ascend310b_scatter_sub_1: op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1
ascendc_bin_ascend310b_scatter_sub_1: op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/build.make
.PHONY : ascendc_bin_ascend310b_scatter_sub_1

# Rule to build all files generated by this target.
op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/build: ascendc_bin_ascend310b_scatter_sub_1
.PHONY : op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/build

op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/clean:
	cd /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel && $(CMAKE_COMMAND) -P CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/cmake_clean.cmake
.PHONY : op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/clean

op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/depend:
	cd /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/op_kernel /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel /home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : op_kernel/CMakeFiles/ascendc_bin_ascend310b_scatter_sub_1.dir/depend

