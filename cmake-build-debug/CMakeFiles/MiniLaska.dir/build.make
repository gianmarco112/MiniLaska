# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\samu9\Documents\GitHub\MiniLaska

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\samu9\Documents\GitHub\MiniLaska\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\MiniLaska.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\MiniLaska.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\MiniLaska.dir\flags.make

CMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.obj: CMakeFiles\MiniLaska.dir\flags.make
CMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.obj: ..\Laska\MiniLaska.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\samu9\Documents\GitHub\MiniLaska\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/MiniLaska.dir/Laska/MiniLaska.c.obj"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.299\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.obj /FdCMakeFiles\MiniLaska.dir\ /FS -c C:\Users\samu9\Documents\GitHub\MiniLaska\Laska\MiniLaska.c
<<

CMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MiniLaska.dir/Laska/MiniLaska.c.i"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.299\bin\Hostx86\x86\cl.exe > CMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\samu9\Documents\GitHub\MiniLaska\Laska\MiniLaska.c
<<

CMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MiniLaska.dir/Laska/MiniLaska.c.s"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.299\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.s /c C:\Users\samu9\Documents\GitHub\MiniLaska\Laska\MiniLaska.c
<<

# Object files for target MiniLaska
MiniLaska_OBJECTS = \
"CMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.obj"

# External object files for target MiniLaska
MiniLaska_EXTERNAL_OBJECTS =

MiniLaska.exe: CMakeFiles\MiniLaska.dir\Laska\MiniLaska.c.obj
MiniLaska.exe: CMakeFiles\MiniLaska.dir\build.make
MiniLaska.exe: CMakeFiles\MiniLaska.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\samu9\Documents\GitHub\MiniLaska\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable MiniLaska.exe"
	"C:\Program Files\JetBrains\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\MiniLaska.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.299\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\MiniLaska.dir\objects1.rsp @<<
 /out:MiniLaska.exe /implib:MiniLaska.lib /pdb:C:\Users\samu9\Documents\GitHub\MiniLaska\cmake-build-debug\MiniLaska.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\MiniLaska.dir\build: MiniLaska.exe

.PHONY : CMakeFiles\MiniLaska.dir\build

CMakeFiles\MiniLaska.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\MiniLaska.dir\cmake_clean.cmake
.PHONY : CMakeFiles\MiniLaska.dir\clean

CMakeFiles\MiniLaska.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\samu9\Documents\GitHub\MiniLaska C:\Users\samu9\Documents\GitHub\MiniLaska C:\Users\samu9\Documents\GitHub\MiniLaska\cmake-build-debug C:\Users\samu9\Documents\GitHub\MiniLaska\cmake-build-debug C:\Users\samu9\Documents\GitHub\MiniLaska\cmake-build-debug\CMakeFiles\MiniLaska.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\MiniLaska.dir\depend

