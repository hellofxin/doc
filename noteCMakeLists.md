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
  add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL]
  source1 source2 ... sourceN
)

* The add_library command can also create IMPORTED library targets using this signature:
add_library(<name> <SHARED|STATIC|MODULE|UNKNOWN> IMPORTED)
An IMPORTED library target references a library file located outside the project. No rules are generated to build it.
The target name has scope in the directory in which it is created and below. It may be referenced like any target built within the project.
IMPORTED libraries are useful for convenient reference from commands like target_link_libraries.

* add_subdirectory: Add a subdirectory to the build.
  add_subdirectory(source_dir [binary_dir] 
                   [EXCLUDE_FROM_ALL])
                   
* add_test: Add a test to the project with the specified arguments.
  add_test(testname Exename arg1 arg2 ...)
  
* aux_source_directory: Find all source files in a directory.
  aux_source_directory(<dir> <variable>)


                   

