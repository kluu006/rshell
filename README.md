# rshell

Rshell is my version of the BASH terminal. Rshell can run commands using connectors such as `;` and `||` and `&&` or no connectors at all. Anything after a `#` will be considered a comment, and is disregarded when runnings commands.

The connector `||` means that if the first command is true, then do not run the second command. If the first command is false, then run the second command.

The connector `&&` means that if the first command is true, then run the second command. If the first command is false, then does not run the second command.

The connector `;` will run any command if the command is true.

Running the command `true` will return true. Running the command `false` will return false.

Rshell supports I/O redirection and piping using `<`, `>`, `>>`, and `|`.

If I/O redirection or pipe symbol is the first instance, it will throw an error for wrong syntax.


For I/O redirection and piping, connector logic does not matter. They will always run even if the first part is true and I/O redirection or piping is after a `&&`. Some for the other logic.

`cd`, `cd -`, `cd path`, and `cd ~` is supported.

Ctrl-C is supported along with Ctrl-Z (fg only).

**EDGE CASES:**
Having an empty command returns false. There is no error output.

Comments are marked as `#`. Anything after the comment will not be ran.

Having an empty pipe throws an error for wrong syntax.

# ls

It is my version of ls. Directories are colored as blue, executables as green, hidden directories as blue with a gray background, hidden executables as green with a gray background, and other files as white.

Instead of outputting vertically, my ls will output horizontally. For non `-l` flags, there is a column width that the outputs follow, it is more noticeable when output exceeds one line.

Flags that are enabled are `-a`, `-l`, and `-R`. Any combo of those three can be pass through as flags, for example, `-aR` or `-laR`.

Flags can be passed through one by one or by any combo too. For example `-a -l -R` or `-aR -l`.

Every input starting with a `-` will be passed through as a flag.

Input without a `-` are passed through as directories. Multiple directories can be inputted.

**NOTE:**
Passing in multiple valid flags of the same flag will not output an error, instead will recognize it as a flag. For example, `-aaaaaaRllllRaaall` is simply `-aRl`.

Any flag besides those three are ignored. For example `-eeeea -Ru123` is the flag `-aR`. 
If all flags are neither `-a -l -R`, then it is simply executed as normal ls with no flags.

## How to run

```
$git clone https://github.com/kluu006/rshell.git
$cd rshell
$git checkout hw2
$make
$bin/rshell
$bin/ls

```
## Bugs / Limitations for rshell

```
Echo with quotes will not work as expected.
echo can output standard in; however, cannot write to a file with quotations.
Quotes do not work in general.
Does not have the option for changing file descriptors.
Does not do <<<.
There are probably more bugs I haven't found.
Cannot have input redirection after piping, but it can work if input redirection is before piping.
Bg does not work properly.

```
## Bugs / Limitations for ls

```
When running -R, an extra new line is at the bottom; it was put in recursively.
When opening or closing a directory, there are extra blank lines outputted.

```
##Licensing

Licensing can be found [here](/LICENSE).
