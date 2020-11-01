# Command-Line-Interface

Implemented a command line interpreter using process control API such as fork, exec and wait. It repeatedly takes commands as input from the user via the terminal and executes each command.

Run the following commands in the terminal to invoke the lightweight shell:
```
gcc shell.c -o shell
./shell
```

When the shell program is invoked, it displays a command prompt as:
``` myshell> ```
and then waits for user input

When an input is given, it first checks whether it is a legal command. If not, then it prints a message "Illegal command or arguments" and goes back to display the prompt and waits for user input. It does not exit the program.

If the command is legal and has the right number of arguments, it executes the command and waits until it is finished.

The following commands are legal inputs which the CLI takes:
```console
myshell> checkcpupercentage 12810
User Mode CPU Percentage: 38%  
System Mode CPU Percentage: 11%  

myshell> checkresidentmemory 13410  
538290

myshell> listFiles
Desktop Downloads Documents Music Photos Videos

myshell> sortFile aFileThatHasBCA.txt 
A 
B 
C

myshell> checkresidentmemory 13410 ; listFiles 
Desktop Downloads Documents Music Photos Videos 
325235 

myshell> exit  
```

**checkcpupercentage [pid]**: This command will accept a process identifier as an argument and print the percentage CPU used by the process with identifier [pid] in the user mode as well as the system mode. 
  
**checkresidentmemory [pid]**: This command will accept a pid of a process and output the resident set size which is the physical memory that the task has used without any headers.

**listFiles**: This command will save all the files and folders, in the current working directory to a file named files.txt. If the file already exists, it will overwrite its contents.

**sortFile [file]**: This command will accept input from a file and then sorts the file’s contents line by line and prints to the STDOUT.

**[command1] ; [command2]**: This semicolon operator executes both commands [command1] and [command2] in parallel. The shell only displays the prompt for the next command once both of the commands are done executing. It assumes that there won’t be more than two commands executed in parallel. If there is "exit" command as one of the 2 commands in parallel execution, then CLI will be exited without executing the other command.

**exit**: This command exits the CLI.

Also if there are more or less number of arguments than required for a particular command, then the CLI will print "Illegal command or arguments". 

If a command is executed without giving any input at all, it will again print "Illegal command or arguments". 

If right number of arguments are given for a command but argument given is invalid, then the CLI will print error thrown by the execution of the bash command corresponding to the CLI command.
