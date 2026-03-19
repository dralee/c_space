### 编译器相关配置

#### gcc编译级别
##### gcc 编译级别：
Add -ggdb to the command line for debug builds and -O2 -DNDEBUG for release builds. Use the debug build options for now.
For GCC and Clang, the -O# option is used to control optimization settings. The most common options are as follows:
- -O0 is the recommended optimization level for debug builds, as it disables optimization. This is the default setting.
- -O2 is the recommended optimization level for release builds, as it applies optimizations that should be beneficial for all programs.
- -O3 adds additional optimizations that may or may not perform better than -O2 depending on the specific program. Once your program is written, you can try compiling your release build with -O3 instead of -O2 and measure to see which is faster.
See https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html for information on optimization options.
默认为-O0，即为debug状态，要编译为release一般为-O2。
##### For VS Code users
When you first ran your program, a new file called tasks.json was created under the .vscode folder in the explorer pane. Open the tasks.json file, find “args”, and then locate the line “${file}” within that section.
Above the “${file}” line, add a new line containing the following command (one per line) when debugging:
"-ggdb",
Above the “${file}” line, add new lines containing the following commands (one per line) for release builds:
"-O2",
"-DNDEBUG",
#### 禁止gcc编译器扩展
##### For gcc and Clang users
You can disable compiler extensions by adding the -pedantic-errors flag to the compile command line.
##### For VS Code users
- Open the tasks.json file, find "args", and then locate the line "${file}" within that section.
- Above the "${file}" line, add a new line containing the following commands:
"-pedantic-errors",
As of the time of writing, VS Code does not automatically add a newline to the end of code files that are missing it (something that is pedantically required by the C++ standard). Fortunately, we can tell VS Code to do so:
- Open VS Code and go to File (Code if using a Mac) > Preferences > Settings. This will open a settings dialog.
- Enter insert final newline into the search bar.
- In both the Workspace Settings and User Settings tabs, ensure the checkbox labeled Files: Insert Final Newline is checked.
#### 警告级别
##### For gcc users
Add the following flags to your command line: -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion
##### For VS Code users
Open the tasks.json file, find “args”, and then locate the line “${file}” within that section.
Above the “${file}” line, add new lines containing the following commands (one per line):
```json
"-Wall",
"-Weffc++",
"-Wextra",
"-Wconversion",
"-Wsign-conversion",
```






