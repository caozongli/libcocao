# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/libcocao/workspace/libcocao

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/libcocao/workspace/libcocao/build

# Include any dependencies generated for this target.
include CMakeFiles/libcocao.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/libcocao.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/libcocao.dir/flags.make

CMakeFiles/libcocao.dir/libcocao/fiber.cc.o: CMakeFiles/libcocao.dir/flags.make
CMakeFiles/libcocao.dir/libcocao/fiber.cc.o: ../libcocao/fiber.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/libcocao/workspace/libcocao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/libcocao.dir/libcocao/fiber.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) -D__FILE__=\"libcocao/fiber.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcocao.dir/libcocao/fiber.cc.o -c /home/libcocao/workspace/libcocao/libcocao/fiber.cc

CMakeFiles/libcocao.dir/libcocao/fiber.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcocao.dir/libcocao/fiber.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/fiber.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/libcocao/workspace/libcocao/libcocao/fiber.cc > CMakeFiles/libcocao.dir/libcocao/fiber.cc.i

CMakeFiles/libcocao.dir/libcocao/fiber.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcocao.dir/libcocao/fiber.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/fiber.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/libcocao/workspace/libcocao/libcocao/fiber.cc -o CMakeFiles/libcocao.dir/libcocao/fiber.cc.s

CMakeFiles/libcocao.dir/libcocao/fiber.cc.o.requires:

.PHONY : CMakeFiles/libcocao.dir/libcocao/fiber.cc.o.requires

CMakeFiles/libcocao.dir/libcocao/fiber.cc.o.provides: CMakeFiles/libcocao.dir/libcocao/fiber.cc.o.requires
	$(MAKE) -f CMakeFiles/libcocao.dir/build.make CMakeFiles/libcocao.dir/libcocao/fiber.cc.o.provides.build
.PHONY : CMakeFiles/libcocao.dir/libcocao/fiber.cc.o.provides

CMakeFiles/libcocao.dir/libcocao/fiber.cc.o.provides.build: CMakeFiles/libcocao.dir/libcocao/fiber.cc.o


CMakeFiles/libcocao.dir/libcocao/log.cc.o: CMakeFiles/libcocao.dir/flags.make
CMakeFiles/libcocao.dir/libcocao/log.cc.o: ../libcocao/log.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/libcocao/workspace/libcocao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/libcocao.dir/libcocao/log.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) -D__FILE__=\"libcocao/log.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcocao.dir/libcocao/log.cc.o -c /home/libcocao/workspace/libcocao/libcocao/log.cc

CMakeFiles/libcocao.dir/libcocao/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcocao.dir/libcocao/log.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/log.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/libcocao/workspace/libcocao/libcocao/log.cc > CMakeFiles/libcocao.dir/libcocao/log.cc.i

CMakeFiles/libcocao.dir/libcocao/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcocao.dir/libcocao/log.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/log.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/libcocao/workspace/libcocao/libcocao/log.cc -o CMakeFiles/libcocao.dir/libcocao/log.cc.s

CMakeFiles/libcocao.dir/libcocao/log.cc.o.requires:

.PHONY : CMakeFiles/libcocao.dir/libcocao/log.cc.o.requires

CMakeFiles/libcocao.dir/libcocao/log.cc.o.provides: CMakeFiles/libcocao.dir/libcocao/log.cc.o.requires
	$(MAKE) -f CMakeFiles/libcocao.dir/build.make CMakeFiles/libcocao.dir/libcocao/log.cc.o.provides.build
.PHONY : CMakeFiles/libcocao.dir/libcocao/log.cc.o.provides

CMakeFiles/libcocao.dir/libcocao/log.cc.o.provides.build: CMakeFiles/libcocao.dir/libcocao/log.cc.o


CMakeFiles/libcocao.dir/libcocao/schedule.cc.o: CMakeFiles/libcocao.dir/flags.make
CMakeFiles/libcocao.dir/libcocao/schedule.cc.o: ../libcocao/schedule.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/libcocao/workspace/libcocao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/libcocao.dir/libcocao/schedule.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) -D__FILE__=\"libcocao/schedule.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcocao.dir/libcocao/schedule.cc.o -c /home/libcocao/workspace/libcocao/libcocao/schedule.cc

CMakeFiles/libcocao.dir/libcocao/schedule.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcocao.dir/libcocao/schedule.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/schedule.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/libcocao/workspace/libcocao/libcocao/schedule.cc > CMakeFiles/libcocao.dir/libcocao/schedule.cc.i

CMakeFiles/libcocao.dir/libcocao/schedule.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcocao.dir/libcocao/schedule.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/schedule.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/libcocao/workspace/libcocao/libcocao/schedule.cc -o CMakeFiles/libcocao.dir/libcocao/schedule.cc.s

CMakeFiles/libcocao.dir/libcocao/schedule.cc.o.requires:

.PHONY : CMakeFiles/libcocao.dir/libcocao/schedule.cc.o.requires

CMakeFiles/libcocao.dir/libcocao/schedule.cc.o.provides: CMakeFiles/libcocao.dir/libcocao/schedule.cc.o.requires
	$(MAKE) -f CMakeFiles/libcocao.dir/build.make CMakeFiles/libcocao.dir/libcocao/schedule.cc.o.provides.build
.PHONY : CMakeFiles/libcocao.dir/libcocao/schedule.cc.o.provides

CMakeFiles/libcocao.dir/libcocao/schedule.cc.o.provides.build: CMakeFiles/libcocao.dir/libcocao/schedule.cc.o


CMakeFiles/libcocao.dir/libcocao/thread.cc.o: CMakeFiles/libcocao.dir/flags.make
CMakeFiles/libcocao.dir/libcocao/thread.cc.o: ../libcocao/thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/libcocao/workspace/libcocao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/libcocao.dir/libcocao/thread.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) -D__FILE__=\"libcocao/thread.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcocao.dir/libcocao/thread.cc.o -c /home/libcocao/workspace/libcocao/libcocao/thread.cc

CMakeFiles/libcocao.dir/libcocao/thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcocao.dir/libcocao/thread.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/thread.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/libcocao/workspace/libcocao/libcocao/thread.cc > CMakeFiles/libcocao.dir/libcocao/thread.cc.i

CMakeFiles/libcocao.dir/libcocao/thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcocao.dir/libcocao/thread.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/thread.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/libcocao/workspace/libcocao/libcocao/thread.cc -o CMakeFiles/libcocao.dir/libcocao/thread.cc.s

CMakeFiles/libcocao.dir/libcocao/thread.cc.o.requires:

.PHONY : CMakeFiles/libcocao.dir/libcocao/thread.cc.o.requires

CMakeFiles/libcocao.dir/libcocao/thread.cc.o.provides: CMakeFiles/libcocao.dir/libcocao/thread.cc.o.requires
	$(MAKE) -f CMakeFiles/libcocao.dir/build.make CMakeFiles/libcocao.dir/libcocao/thread.cc.o.provides.build
.PHONY : CMakeFiles/libcocao.dir/libcocao/thread.cc.o.provides

CMakeFiles/libcocao.dir/libcocao/thread.cc.o.provides.build: CMakeFiles/libcocao.dir/libcocao/thread.cc.o


CMakeFiles/libcocao.dir/libcocao/utils.cc.o: CMakeFiles/libcocao.dir/flags.make
CMakeFiles/libcocao.dir/libcocao/utils.cc.o: ../libcocao/utils.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/libcocao/workspace/libcocao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/libcocao.dir/libcocao/utils.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) -D__FILE__=\"libcocao/utils.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcocao.dir/libcocao/utils.cc.o -c /home/libcocao/workspace/libcocao/libcocao/utils.cc

CMakeFiles/libcocao.dir/libcocao/utils.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcocao.dir/libcocao/utils.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/utils.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/libcocao/workspace/libcocao/libcocao/utils.cc > CMakeFiles/libcocao.dir/libcocao/utils.cc.i

CMakeFiles/libcocao.dir/libcocao/utils.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcocao.dir/libcocao/utils.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"libcocao/utils.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/libcocao/workspace/libcocao/libcocao/utils.cc -o CMakeFiles/libcocao.dir/libcocao/utils.cc.s

CMakeFiles/libcocao.dir/libcocao/utils.cc.o.requires:

.PHONY : CMakeFiles/libcocao.dir/libcocao/utils.cc.o.requires

CMakeFiles/libcocao.dir/libcocao/utils.cc.o.provides: CMakeFiles/libcocao.dir/libcocao/utils.cc.o.requires
	$(MAKE) -f CMakeFiles/libcocao.dir/build.make CMakeFiles/libcocao.dir/libcocao/utils.cc.o.provides.build
.PHONY : CMakeFiles/libcocao.dir/libcocao/utils.cc.o.provides

CMakeFiles/libcocao.dir/libcocao/utils.cc.o.provides.build: CMakeFiles/libcocao.dir/libcocao/utils.cc.o


# Object files for target libcocao
libcocao_OBJECTS = \
"CMakeFiles/libcocao.dir/libcocao/fiber.cc.o" \
"CMakeFiles/libcocao.dir/libcocao/log.cc.o" \
"CMakeFiles/libcocao.dir/libcocao/schedule.cc.o" \
"CMakeFiles/libcocao.dir/libcocao/thread.cc.o" \
"CMakeFiles/libcocao.dir/libcocao/utils.cc.o"

# External object files for target libcocao
libcocao_EXTERNAL_OBJECTS =

../bin/liblibcocao.so: CMakeFiles/libcocao.dir/libcocao/fiber.cc.o
../bin/liblibcocao.so: CMakeFiles/libcocao.dir/libcocao/log.cc.o
../bin/liblibcocao.so: CMakeFiles/libcocao.dir/libcocao/schedule.cc.o
../bin/liblibcocao.so: CMakeFiles/libcocao.dir/libcocao/thread.cc.o
../bin/liblibcocao.so: CMakeFiles/libcocao.dir/libcocao/utils.cc.o
../bin/liblibcocao.so: CMakeFiles/libcocao.dir/build.make
../bin/liblibcocao.so: CMakeFiles/libcocao.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/libcocao/workspace/libcocao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library ../bin/liblibcocao.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libcocao.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libcocao.dir/build: ../bin/liblibcocao.so

.PHONY : CMakeFiles/libcocao.dir/build

CMakeFiles/libcocao.dir/requires: CMakeFiles/libcocao.dir/libcocao/fiber.cc.o.requires
CMakeFiles/libcocao.dir/requires: CMakeFiles/libcocao.dir/libcocao/log.cc.o.requires
CMakeFiles/libcocao.dir/requires: CMakeFiles/libcocao.dir/libcocao/schedule.cc.o.requires
CMakeFiles/libcocao.dir/requires: CMakeFiles/libcocao.dir/libcocao/thread.cc.o.requires
CMakeFiles/libcocao.dir/requires: CMakeFiles/libcocao.dir/libcocao/utils.cc.o.requires

.PHONY : CMakeFiles/libcocao.dir/requires

CMakeFiles/libcocao.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libcocao.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libcocao.dir/clean

CMakeFiles/libcocao.dir/depend:
	cd /home/libcocao/workspace/libcocao/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/libcocao/workspace/libcocao /home/libcocao/workspace/libcocao /home/libcocao/workspace/libcocao/build /home/libcocao/workspace/libcocao/build /home/libcocao/workspace/libcocao/build/CMakeFiles/libcocao.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libcocao.dir/depend

