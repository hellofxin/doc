## 
```
target … : prerequisites …
    recipe
    …
    …

```


### .INCLUDE_DIRS
    The .INCLUDE_DIRS variable will contain the current list of directories that make will
search for included files

    You can avoid searching in these default directories by adding the command line
option -I with the special value - (e.g., -I-) to the command line. This will cause
make to forget any already-set include directories, including the default
directories.

    If you want make to simply ignore a makefile which does not exist or cannot be
remade, with no error message, use the -include directive instead of include, like
this:
-include filenames…

###  How make Reads a Makefile
    GNU make does its work in two distinct phases. During the first phase it reads all
the makefiles, included makefiles, etc. and internalizes all the variables and their
values and implicit and explicit rules, and builds a dependency graph of all the
targets and their prerequisites. 
    During the second phase, make uses this
internalized data to determine which targets need to be updated and run the
recipes necessary to update them.

### Variable Assignment
Variable definitions are parsed as follows:
```
immediate = deferred
immediate ?= deferred
immediate := immediate
immediate ::= immediate
immediate :::= immediate-with-escape
immediate += deferred or immediate
immediate != immediate

define immediate
deferred
endef

define immediate =
deferred
endef

define immediate ?=
deferred
endef

define immediate :=
immediate
endef

define immediate ::=
immediate
endef

define immediate :::=
immediate-with-escape
endef

define immediate +=
deferred or immediate
endef

define immediate !=
immediate
endef

```
For the append operator ‘+=’, the right-hand side is considered immediate if
the variable was previously set as a simple variable (‘:=’ or ‘::=’), and
deferred otherwise.

For the immediate-with-escape operator ‘:::=’, the value on the right-hand
side is immediately expanded but then escaped (that is, all instances of $ in the
result of the expansion are replaced with $$).

For the shell assignment operator ‘!=’, the right-hand side is evaluated
immediately and handed to the shell. The result is stored in the variable named
on the left, and that variable is considered a recursively expanded variable (and
will thus be re-evaluated on each reference).

### Conditional Directives
Conditional directives are parsed immediately. This means, for example, that
automatic variables cannot be used in conditional directives, as automatic
variables are not set until the recipe for that rule is invoked. If you need to use
automatic variables in a conditional directive you must move the condition into
the recipe and use shell conditional syntax instead

### How Makefiles Are Parsed
GNU make parses makefiles line-by-line. Parsing proceeds using the following
steps:
1. Read in a full logical line, including backslash-escaped lines (see Splitting
Long Lines).
2. Remove comments (see What Makefiles Contain).
2024/9/3 21:32 GNU make
https://www.gnu.org/software/make/manual/make.html 29/305
3. If the line begins with the recipe prefix character and we are in a rule
context, add the line to the current recipe and read the next line (see
Recipe Syntax).
4. Expand elements of the line which appear in an immediate expansion
context (see How make Reads a Makefile).
5. Scan the line for a separator character, such as ‘:’ or ‘=’, to determine
whether the line is a macro assignment or a rule (see Recipe Syntax).
6. Internalize the resulting operation and read the next line.
7. 
An important consequence of this is that a macro can expand to an entire rule, if
it is one line long. This will work:

In order to properly expand a multi-line macro you must use the eval function:
this causes the make parser to be run on the results of the expanded macro

### Secondary Expansion
```
.SECONDEXPANSION:
AVAR = top
onefile: $(AVAR)
twofile: $$(AVAR)
AVAR = bottom

```
Here the prerequisite of onefile will be expanded immediately, and resolve to the
value top, while the prerequisite of twofile will not be full expanded until the
secondary expansion and yield a value of bottom
```
SECONDEXPANSION:
main_OBJS := main.o try.o test.o
lib_OBJS := lib.o api.o
main lib: $$($$@_OBJS)
```
Here, after the initial expansion the prerequisites of both the main and lib targets
will be $($@_OBJS). During the secondary expansion, the $@ variable is set to the
name of the target and so the expansion for the main target will yield $(main_OBJS),
or main.o try.o test.o, while the secondary expansion for the lib target will yield
$(lib_OBJS), or lib.o api.o.

## Rule Example
```
foo.o : foo.c defs.h # module for twiddling the frobs
cc -c -g foo.c

```
Its target is foo.o and its prerequisites are foo.c and defs.h. It has one command in
the recipe: ‘cc -c -g foo.c’. The recipe starts with a tab to identify it as a recipe

Because dollar signs are used to start make variable references, if you really want a
dollar sign in a target or prerequisite you must write two of them, ‘$$’ (see
How to Use Variables). If you have enabled secondary expansion (see Secondary
Expansion) and you want a literal dollar sign in the prerequisites list, you must
actually write four dollar signs (‘$$$$’).

A rule tells make two things: when the targets are out of date, and how to update
them when necessary.

A target is out of date if it does not exist or if it is older than any of the prerequisites (by comparison of last-modification times). The idea is that the contents of the
target file are computed based on information in the prerequisites, so if any of
the prerequisites changes, the contents of the existing target file are no longer
necessarily valid.

How to update is specified by a recipe. This is one or more lines to be executed
by the shell (normally ‘sh’),

### Types of Prerequisites
```
OBJDIR := objdir
OBJS := $(addprefix $(OBJDIR)/,foo.o bar.o baz.o)
$(OBJDIR)/%.o : %.c
$(COMPILE.c) $(OUTPUT_OPTION) $<
all: $(OBJS)
$(OBJS): | $(OBJDIR)
$(OBJDIR):
mkdir $(OBJDIR)
```
Now the rule to create the objdir directory will be run, if needed, before any
‘.o’ is built, but no ‘.o’ will be built because the objdir directory timestamp
changed.


### Using Wildcard Characters in File Names
~john/bin expands to /home/john/bin

Wildcard expansion is performed by make automatically in targets and in prerequisites. In recipes, the shell is responsible for wildcard expansion. In other contexts, wildcard expansion happens only if you request it explicitly with the wildcard function.

Wildcard expansion does not happen when you define a variable. Thus, if you write this
```
objects = *.o
```
then the value of the variable objects is the actual string ‘*.o’. However, if you
use the value of objects in a target or prerequisite, wildcard expansion will take
place there. If you use the value of objects in a recipe, the shell may perform
wildcard expansion when the recipe runs. 
To set objects to the expansion, instead use:
```
objects := $(wildcard *.o)
```

### The Function wildcard
One use of the wildcard function is to get a list of all the C source files in a
directory, like this:
```
$(wildcard *.c)
```
We can change the list of C source files into a list of object files by replacing the
‘.c’ suffix with ‘.o’ in the result, like this:
```
$(patsubst %.c,%.o,$(wildcard *.c))
```
Thus, a makefile to compile all C source files in the directory and then link them
together could be written as follows:
```
objects := $(patsubst %.c,%.o, $(wildcard *.c))
foo : $(objects)
    cc -o foo $(objects)
```

## 4.5 Searching Directories for Prerequisites

### 4.5.1 VPATH: Search Path for All Prerequisites
```
vpath %.h ../headers
```
tells make to look for any prerequisite whose name ends in .h in the directory ../headers if the file is not found in the current directory.

## Phony Targets
A phony target is one that is not really the name of a file; rather it is just a name
for a recipe to be executed when you make an explicit request. There are two
reasons to use a phony target: to avoid a conflict with a file of the same name,
and to improve performance.

If you write a rule whose recipe will not create the target file, the recipe will be
executed every time the target comes up for remaking. Here is an example:
```
clean:
    rm *.o temp
```
Because the rm command does not create a file named clean, probably no such
file will ever exist. Therefore, the rm command will be executed every time you
say ‘make clean’.

In this example, the clean target will not work properly if a file named clean is ever
created in this directory. Since it has no prerequisites, clean would always be
considered up to date and its recipe would not be executed. To avoid this
problem you can explicitly declare the target to be phony by making it a
prerequisite of the special target .PHONY
```
.PHONY: clean
clean:
rm *.o temp
```
Once this is done, ‘make clean’ will run the recipe regardless of whether there is
a file named clean.

Phony targets are also useful in conjunction with recursive invocations of make
(see Recursive Use of make). In this situation the makefile will often contain a
variable which lists a number of sub-directories to be built. A simplistic way to
handle this is to define one rule with a recipe that loops over the sub-directories,
like this:
```
SUBDIRS = foo bar baz
subdirs:
for dir in $(SUBDIRS); do \
$(MAKE) -C $$dir; \
done

```
There are problems with this method, however. First, any error detected in a submake is ignored by this rule, so it will continue to build the rest of the directories
even when one fails. This can be overcome by adding shell commands to note
the error and exit, but then it will do so even if make is invoked with the -k option,
which is unfortunate. Second, and perhaps more importantly, you cannot take
full advantage of make’s ability to build targets in parallel (see Parallel Execution),
since there is only one rule. Each individual makefile’s targets will be built in
parallel, but only one sub-directory will be built at a time.

By declaring the sub-directories as .PHONY targets (you must do this as the subdirectory obviously always exists; otherwise it won’t be built) you can remove
these problems:
```
SUBDIRS = foo bar baz
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
    $(MAKE) -C $@
foo: baz

```
Here we’ve also declared that the foo sub-directory cannot be built until after
the baz sub-directory is complete; this kind of relationship declaration is
particularly important when attempting parallel builds.

4.7 Rules without Recipes or Prerequisites
If a rule has no prerequisites or recipe, and the target of the rule is a nonexistent
file, then make imagines this target to have been updated whenever its rule is run.
This implies that all targets depending on this one will always have their recipe
run.
An example will illustrate this:
```
clean: FORCE
    rm $(objects)
FORCE:
```
Here the target ‘FORCE’ satisfies the special conditions, so the target clean that
depends on it is forced to run its recipe. There is nothing special about the name
‘FORCE’, but that is one name commonly used this way.
As you can see, using ‘FORCE’ this way has the same results as using ‘.PHONY:
clean’.
Using ‘.PHONY’ is more explicit and more efficient. However, other versions of
make do not support ‘.PHONY’; thus ‘FORCE’ appears in many makefiles