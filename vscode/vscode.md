Go to Line in a file
    filename:line number
Go to Symbol in a file
    filename@symbol name
Quick Open multiple files
    press Right Arrow to open the selected file but leave the dropdown available
Multi-cursor selection
    Alt+Click on Windows and Linux, Option+Click on macOS to add a new cursor
    Ctrl+Alt+Up Ctrl+Alt+Down to add a new cursor above or below the current position
    Ctrl+Shift+L to add cursors to all matches of the current selection
IntelliSense smart code completions
    trigger IntelliSense with Ctrl+Space

# launch.json
the following attributes are mandatory for every launch configuration:

* type -
   the type of debugger to use for this launch configuration. 
   Every installed debug extension introduces a type: node for the built-in Node debugger, for example, or php and go for the PHP and Go extensions.
* request - 
  the request type of this launch configuration. 
  Currently, launch and attach are supported.
* name - 
  the reader-friendly name to appear in the Debug launch configuration dropdown.

Here are some optional attributes available to all launch configurations:

* presentation - 
    sing the order, group, and hidden attributes in the presentation object, you can sort, group, and hide configurations and compounds in the Debug configuration dropdown and in the Debug quick pick.
* preLaunchTask - 
  to launch a task before the start of a debug session, set this attribute to the label of a task specified in tasks.json (in the workspace's .vscode folder). Or, this can be set to ${defaultBuildTask} to use your default build task.
* postDebugTask - 
  to launch a task at the very end of a debug session, set this attribute to the name of a task specified in tasks.json (in the workspace's .vscode folder).
* internalConsoleOptions - 
  this attribute controls the visibility of the Debug Console panel during a debugging session.
* debugServer - 
  for debug extension authors only: this attribute allows you to connect to a specified port instead of launching the debug adapter.
* serverReadyAction - 
  if you want to open a URL in a web browser whenever the program under debugging outputs a specific message to the debug console or integrated terminal. For details see section Automatically open a URI when debugging a server program below.

Many debuggers support some of the following attributes:

* program - 
  executable or file to run when launching the debugger
* args - 
  arguments passed to the program to debug
* env - 
  environment variables (the value null can be used to "undefine" a variable)
* envFile - 
  path to dotenv file with environment variables
* cwd - 
  current working directory for finding dependencies and other files
* port - 
  port when attaching to a running process
* stopOnEntry - 
  break immediately when the program launches
* console - 
  what kind of console to use, for example, internalConsole, integratedTerminal, or externalTerminal

Launch.json supports defining values (for example, arguments to be passed to the program) that depend on the operating system where the debugger is running. To do so, put a platform-specific literal into the launch.json file and specify the corresponding properties inside that literal.

Valid operating properties are "windows" for Windows, "linux" for Linux, and "osx" for macOS. Properties defined in an operating system specific scope override properties defined in the global scope.

Please note that the type property cannot be placed inside a platform-specific section, because type indirectly determines the platform in remote debugging scenarios, and that would result in a cyclic dependency.


## task
The task's properties have the following semantic:

* label: 
  The task's label used in the user interface.
* type: 
  The task's type. For a custom task, this can either be shell or process. If shell is specified, the command is interpreted as a shell command (for example: bash, cmd, or PowerShell). If process is specified, the command is interpreted as a process to execute.
* command: 
  The actual command to execute.
* windows:
  Any Windows specific properties. Will be used instead of the default properties when the command is executed on the Windows operating system.
* group:
   Defines to which group the task belongs. In the example, it belongs to the test group. Tasks that belong to the test group can be executed by running Run Test Task from the Command Palette.
* presentation: 
  Defines how the task output is handled in the user interface. In this example, the Integrated Terminal showing the output is always revealed and a new terminal is created on every task run.
* options: 
  Override the defaults for cwd (current working directory), env (environment variables), or shell (default shell). Options can be set per task but also globally or per platform. Environment variables configured here can only be referenced from within your task script or process and will not be resolved if they are part of your args, command, or other task attributes.
* runOptions: 
  Defines when and how a task is run.