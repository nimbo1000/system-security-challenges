Basic Buffer overflow challenge
==============================


vuln.c is an example of a program vulnerable to a buffer overflow. The
vuln program is setuid to the user "secret".  This program should then
be able to read the private file that contains the solution you need
to provide to pass the challenge. But for this to happen you need to
abuse the buffer overflow vulnerability. One way to do this is to
provide an input string that will include a "shellcode".

The work consists in :

1) Writing a string that will perform a stack based buffer overflow
and corrupt the return address. 
2) make the return address point to the shellcode (e.g. the one
provided as an example in shell.c/shell)
3) it is likely that it will not work the first time, use gdb and
track stack contents/execution of the attack.

4) Once you were able to execute the shellcode, you obtain a shell
under the user "secret" use this to access the private file.


Hint 1 : passing strings as input to the program, you can use perl e.g.:
  challenges/chall4/vuln `perl -e 'printf("helloworld");printf("\x90"x10);' 
  that will pass the string made of "helloworld" immediately followed
  by 10 times the byte 0x90 (nop).

Hint 2 : make sure that no null or space characters are present in
your string:
- spaces will lead vuln to interpret the string as several parameters
- Null bytes are end string markers and the string copy operation will
  therefore stop on a null byte.
