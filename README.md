# UNIXProcesses_C
In this program written in C, the following process tree is generated:

![arbol](resources/arbol.png)

Shared memory is used to print the PIDs of the different processes.
Also, the program requires 2 parameters or arguments: Z process, after the number of seconds specified by the second parameter, will send a signal to the process indicated by the first parameter (A, B, X, Y), instructing it to execute another command (if the parameter is A or B, it will execute the "pstree" command, while if the parameter is X or Y, it will execute the "ls" command).
