# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/phra/tesi/mih/802_21/myODTONE

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/phra/tesi/mih/802_21/myODTONE

# Include any dependencies generated for this target.
include app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/depend.make

# Include the progress variables for this target.
include app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/progress.make

# Include the compile flags for this target's objects.
include app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/flags.make

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/flags.make
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o: app/dhcp_usr/client/clparse.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/phra/tesi/mih/802_21/myODTONE/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o   -c /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/clparse.c

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/odtone-dhcp_usr.dir/clparse.c.i"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/clparse.c > CMakeFiles/odtone-dhcp_usr.dir/clparse.c.i

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/odtone-dhcp_usr.dir/clparse.c.s"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/clparse.c -o CMakeFiles/odtone-dhcp_usr.dir/clparse.c.s

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o.requires:
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o.requires

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o.provides: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o.requires
	$(MAKE) -f app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build.make app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o.provides.build
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o.provides

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o.provides.build: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/flags.make
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o: app/dhcp_usr/client/dhc6.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/phra/tesi/mih/802_21/myODTONE/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o   -c /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhc6.c

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.i"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhc6.c > CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.i

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.s"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhc6.c -o CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.s

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o.requires:
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o.requires

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o.provides: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o.requires
	$(MAKE) -f app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build.make app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o.provides.build
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o.provides

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o.provides.build: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/flags.make
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o: app/dhcp_usr/client/dhclient.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/phra/tesi/mih/802_21/myODTONE/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o   -c /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhclient.c

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.i"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhclient.c > CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.i

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.s"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhclient.c -o CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.s

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o.requires:
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o.requires

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o.provides: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o.requires
	$(MAKE) -f app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build.make app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o.provides.build
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o.provides

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o.provides.build: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/flags.make
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o: app/dhcp_usr/client/dhcp_control.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/phra/tesi/mih/802_21/myODTONE/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o -c /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhcp_control.cpp

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.i"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhcp_control.cpp > CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.i

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.s"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhcp_control.cpp -o CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.s

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o.requires:
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o.requires

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o.provides: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o.requires
	$(MAKE) -f app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build.make app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o.provides.build
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o.provides

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o.provides.build: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/flags.make
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o: app/dhcp_usr/client/dhcp_usr.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/phra/tesi/mih/802_21/myODTONE/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o -c /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhcp_usr.cpp

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.i"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhcp_usr.cpp > CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.i

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.s"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/dhcp_usr.cpp -o CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.s

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o.requires:
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o.requires

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o.provides: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o.requires
	$(MAKE) -f app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build.make app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o.provides.build
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o.provides

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o.provides.build: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/flags.make
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o: app/dhcp_usr/client/log.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/phra/tesi/mih/802_21/myODTONE/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o -c /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/log.cpp

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtone-dhcp_usr.dir/log.cpp.i"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/log.cpp > CMakeFiles/odtone-dhcp_usr.dir/log.cpp.i

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtone-dhcp_usr.dir/log.cpp.s"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/log.cpp -o CMakeFiles/odtone-dhcp_usr.dir/log.cpp.s

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o.requires:
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o.requires

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o.provides: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o.requires
	$(MAKE) -f app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build.make app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o.provides.build
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o.provides

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o.provides.build: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/flags.make
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o: app/dhcp_usr/client/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/phra/tesi/mih/802_21/myODTONE/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o -c /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/main.cpp

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtone-dhcp_usr.dir/main.cpp.i"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/main.cpp > CMakeFiles/odtone-dhcp_usr.dir/main.cpp.i

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtone-dhcp_usr.dir/main.cpp.s"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/main.cpp -o CMakeFiles/odtone-dhcp_usr.dir/main.cpp.s

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o.requires:
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o.requires

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o.provides: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o.requires
	$(MAKE) -f app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build.make app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o.provides.build
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o.provides

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o.provides.build: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o

# Object files for target odtone-dhcp_usr
odtone__dhcp_usr_OBJECTS = \
"CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o" \
"CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o" \
"CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o" \
"CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o" \
"CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o" \
"CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o" \
"CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o"

# External object files for target odtone-dhcp_usr
odtone__dhcp_usr_EXTERNAL_OBJECTS =

app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build.make
app/dhcp_usr/client/odtone-dhcp_usr: lib/external/libnl/nlwrap/libnlwrap.so.0.5.0
app/dhcp_usr/client/odtone-dhcp_usr: lib/odtone/libodtone.so.0.5.0
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/libs/omapip/libomapi.a
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/libs/common/libdhcp.a
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/libs/bind/lib/libdns.a
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/libs/bind/lib/libisc.a
app/dhcp_usr/client/odtone-dhcp_usr: /usr/lib/libboost_system-mt.so
app/dhcp_usr/client/odtone-dhcp_usr: /usr/lib/libboost_program_options-mt.so
app/dhcp_usr/client/odtone-dhcp_usr: /usr/lib/libboost_thread-mt.so
app/dhcp_usr/client/odtone-dhcp_usr: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable odtone-dhcp_usr"
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/odtone-dhcp_usr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build: app/dhcp_usr/client/odtone-dhcp_usr
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/build

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/requires: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clparse.c.o.requires
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/requires: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhc6.c.o.requires
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/requires: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhclient.c.o.requires
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/requires: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_control.cpp.o.requires
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/requires: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/dhcp_usr.cpp.o.requires
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/requires: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/log.cpp.o.requires
app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/requires: app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/main.cpp.o.requires
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/requires

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clean:
	cd /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client && $(CMAKE_COMMAND) -P CMakeFiles/odtone-dhcp_usr.dir/cmake_clean.cmake
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/clean

app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/depend:
	cd /home/phra/tesi/mih/802_21/myODTONE && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/phra/tesi/mih/802_21/myODTONE /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client /home/phra/tesi/mih/802_21/myODTONE /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client /home/phra/tesi/mih/802_21/myODTONE/app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/dhcp_usr/client/CMakeFiles/odtone-dhcp_usr.dir/depend

