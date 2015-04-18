# rshell

Rshell is my version of the BASH terminal. Rshell can run commands using connectors such as `;` and `||` and `&&` or no connectors at all. Anything after a # will be considered a comment, and is disregarded when runnings commands.

The connector `||` means that if the first command is true, then do not run the second command. If the first command is false, then run the second command.

The connector `&&` means that if the first command is true, then run the second command. If the first command is false, then does not run the second command.

The connector `;` will run any command if the command is true.

Running the command `true` will return true. Running the command false will return false.

**EDGE CASES:**
Having an empty command returns false. There is no error output.

Comments are marked as `#`. Anything after the comment will not be ran.

## How to run rshell

```
$git clone https://github.com/kluu006/rshell.git
$cd rshell
$git checkout hw0
$make
$bin/rshell

```
## Bugs / Limitations

```
Anything under quotes will not work.
echo can output standard in; however, cannot write to a file with quotations.

```

## Licensing

Licensing can be found [here](/LICENSE)
