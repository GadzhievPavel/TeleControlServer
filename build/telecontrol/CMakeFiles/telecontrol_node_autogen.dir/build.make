# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pavel/TeleControlServer/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pavel/TeleControlServer/build

# Utility rule file for telecontrol_node_autogen.

# Include the progress variables for this target.
include telecontrol/CMakeFiles/telecontrol_node_autogen.dir/progress.make

telecontrol/CMakeFiles/telecontrol_node_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pavel/TeleControlServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target telecontrol_node"
	cd /home/pavel/TeleControlServer/build/telecontrol && /usr/bin/cmake -E cmake_autogen /home/pavel/TeleControlServer/build/telecontrol/CMakeFiles/telecontrol_node_autogen.dir/AutogenInfo.json Debug

telecontrol_node_autogen: telecontrol/CMakeFiles/telecontrol_node_autogen
telecontrol_node_autogen: telecontrol/CMakeFiles/telecontrol_node_autogen.dir/build.make

.PHONY : telecontrol_node_autogen

# Rule to build all files generated by this target.
telecontrol/CMakeFiles/telecontrol_node_autogen.dir/build: telecontrol_node_autogen

.PHONY : telecontrol/CMakeFiles/telecontrol_node_autogen.dir/build

telecontrol/CMakeFiles/telecontrol_node_autogen.dir/clean:
	cd /home/pavel/TeleControlServer/build/telecontrol && $(CMAKE_COMMAND) -P CMakeFiles/telecontrol_node_autogen.dir/cmake_clean.cmake
.PHONY : telecontrol/CMakeFiles/telecontrol_node_autogen.dir/clean

telecontrol/CMakeFiles/telecontrol_node_autogen.dir/depend:
	cd /home/pavel/TeleControlServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pavel/TeleControlServer/src /home/pavel/TeleControlServer/src/telecontrol /home/pavel/TeleControlServer/build /home/pavel/TeleControlServer/build/telecontrol /home/pavel/TeleControlServer/build/telecontrol/CMakeFiles/telecontrol_node_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : telecontrol/CMakeFiles/telecontrol_node_autogen.dir/depend
