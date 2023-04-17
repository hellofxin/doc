### options
```
* -G <generator-name>: Specify a makefile generator

* -L[A][H]: List non-advanced cached variables.
List cache variables will run CMake and list all the variables from the CMake cache that are not marked as INTERNAL or ADVANCED. 
This will effectively display current CMake settings, which can be then changed with -D option. 
Changing some of the variable may result in more variables being created.
If A is specified, then it will display also advanced variables. If H is specified, it will also display help for each variable.

* --graphviz=[file]: Generate graphviz of dependencies.

* --system-information [file]: Dump information about this system

* --trace: Put cmake in trace mode.
Print a trace of all calls made and from where with message(send_error ) calls.

* --help-command cmd [file]: Print help for a single command and exit.
Full documentation specific to the given command is displayed.
If a file is specified, the documentation is written into and the output format is determined depending on the filename suffix.
Supported are man page, HTML, DocBook and plain text.

* --help-command-list [file]: List available listfile commands and exit

* --help-commands [file]: Print help for all commands and exit.

* --help-module module [file]: Print help for a single module and exit

* --help-variable var [file]: Print help for a single variable and exit

* --help: Print usage information and exit
```

### command
* add_executable: Add an executable to the project using the specified source files.
  add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
  source1 source2 ... sourceN
)
                 
* add_library: Add a library to the project using the specified source files.
  add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] source1 source2 ... sourceN
)

* The add_library command can also create IMPORTED library targets using this signature:
add_library(<name> <SHARED|STATIC|MODULE|UNKNOWN> IMPORTED)
An IMPORTED library target references a library file located outside the project. No rules are generated to build it.
The target name has scope in the directory in which it is created and below. It may be referenced like any target built within the project.
IMPORTED libraries are useful for convenient reference from commands like target_link_libraries.

* add_subdirectory: Add a subdirectory to the build.
  add_subdirectory(source_dir [binary_dir]  [EXCLUDE_FROM_ALL])
                   
* add_test: Add a test to the project with the specified arguments.
  add_test(testname Exename arg1 arg2 ...)
  
* aux_source_directory: Find all source files in a directory.
  aux_source_directory(<dir> <variable>)
  
* cmake_minimum_required: Set the minimum required version of cmake for a project.
  cmake_minimum_required(VERSION major[.minor[.patch]] [FATAL_ERROR])
  
 * configure_file: Copy a file to another location and modify its contents.
  configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
The Input and Output files have to have full paths. This command replaces any variables in the input file 
referenced as ${VAR} or @VAR@ with their values as determined by CMake. If a variable is not defined, it will be replaced with nothing.

* enable_language: Enable a language (CXX/C/Fortran/etc)
  enable_language(languageName [OPTIONAL] )
  
* export: Export targets from the build tree for use by outside projects.
  export(TARGETS [target1 [target2 [...]]] [NAMESPACE <namespace>] [APPEND] FILE <filename>)
  
* file: File manipulation command.
  file(WRITE filename "message to write"... )
  file(APPEND filename "message to write"... )
  file(READ filename variable [LIMIT numBytes] [OFFSET offset] [HEX])
  file(STRINGS filename variable [LIMIT_COUNT num]
       [LIMIT_INPUT numBytes] [LIMIT_OUTPUT numBytes]
       [LENGTH_MINIMUM numBytes] [LENGTH_MAXIMUM numBytes]
       [NEWLINE_CONSUME] [REGEX regex]
       [NO_HEX_CONVERSION])
  file(GLOB variable [RELATIVE path] [globbing expressions]...)
  file(GLOB_RECURSE variable [RELATIVE path] 
       [FOLLOW_SYMLINKS] [globbing expressions]...)
  file(REMOVE [file1 ...])
  file(REMOVE_RECURSE [file1 ...])
  file(MAKE_DIRECTORY [directory1 directory2 ...])
  file(RELATIVE_PATH variable directory file)
  file(TO_CMAKE_PATH path result)
  file(TO_NATIVE_PATH path result)
  file(DOWNLOAD url file [TIMEOUT timeout] [STATUS status] [LOG log])
  
* find_file: Find the full path to a file.
   find_path(<VAR> name1 [path1 path2 ...])
This is the short-hand signature for the command that is sufficient in many cases. It is the same as find_path(<VAR> name1 [PATHS path1 path2 ...])

   find_path(
             <VAR>
             name | NAMES name1 [name2 ...]
             [HINTS path1 [path2 ... ENV var]]
             [PATHS path1 [path2 ... ENV var]]
             [PATH_SUFFIXES suffix1 [suffix2 ...]]
             [DOC "cache documentation string"]
             [NO_DEFAULT_PATH]
             [NO_CMAKE_ENVIRONMENT_PATH]
             [NO_CMAKE_PATH]
             [NO_SYSTEM_ENVIRONMENT_PATH]
             [NO_CMAKE_SYSTEM_PATH]
             [CMAKE_FIND_ROOT_PATH_BOTH |
              ONLY_CMAKE_FIND_ROOT_PATH |
              NO_CMAKE_FIND_ROOT_PATH]
            )
This command is used to find a full path to named file. A cache entry named by <VAR> is created to store the result of this command.
  
* find_library: Find a library.
   find_library(<VAR> name1 [path1 path2 ...])
  
* find_path: Find the directory containing a file.
   find_path(<VAR> name1 [path1 path2 ...])
This is the short-hand signature for the command that is sufficient in many cases. It is the same as find_path(<VAR> name1 [PATHS path1 path2 ...])
  
* find_program: Find an executable program.
   find_program(<VAR> name1 [path1 path2 ...])
  
* include_directories: Add include directories to the build.
  include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)
Add the given directories to those searched by the compiler for include files.

* include_regular_expression: Set the regular expression used for dependency checking.
  include_regular_expression(regex_match [regex_complain])
Set the regular expressions used in dependency checking. Only files matching regex_match will be traced as dependencies. 
Only files matching regex_complain will generate warnings if they cannot be found (standard header paths are not searched). The defaults are:

  regex_match    = "^.*$" (match everything)
  regex_complain = "^$" (match empty string only)
  
* install: Specify rules to run at install time
 install(TARGETS targets... [EXPORT <export-name>]
          [[ARCHIVE|LIBRARY|RUNTIME|FRAMEWORK|BUNDLE|
            PRIVATE_HEADER|PUBLIC_HEADER|RESOURCE]
           [DESTINATION <dir>]
           [PERMISSIONS permissions...]
           [CONFIGURATIONS [Debug|Release|...]]
           [COMPONENT <component>]
           [OPTIONAL] [NAMELINK_ONLY|NAMELINK_SKIP]
          ] [...])
  
* link_directories: Specify directories in which the linker will look for libraries.
  link_directories(directory1 directory2 ...)
Specify the paths in which the linker should search for libraries. The command will apply only to targets created after it is called. For historical reasons, 
relative paths given to this command are passed to the linker unchanged (unlike many CMake commands which interpret them relative to the current source directory).

* message: Display a message to the user.
  message([SEND_ERROR | STATUS | FATAL_ERROR] "message to display" ...)
  
* option: Provides an option that the user can optionally select.
  option(<option_variable> "help string describing option" [initial value])
  
* project: Set a name for the entire project.
  project(projectname [CXX] [C] [Java])
Sets the name of the project. This creates the variables projectname_BINARY_DIR and projectname_SOURCE_DIR

* set: Set a CMAKE variable to a given value.
  set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
                   

