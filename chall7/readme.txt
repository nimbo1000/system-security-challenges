use-after-free challenge
==============================


arena.c is an example of a program vulnerable to a use-after-free. The
arena program is setuid to the user "secret".  This program should then
be able to read the private file that contains the solution you need
to provide to pass the challenge. But for this to happen you need to
abuse the use-after-free vulnerability. You can use the shellcode provided
in the previous challenge to exploit it.

This program is a (really serious) fighting simulator. Weapons are provided
in a file where each line has format "aa bb long name...".
"aa": min damage
"bb": max damage
"long name...": name of the weapon

Hint 1: you can save your input in a file and redirect stdin to read from it:
$ ./arena weaponry < input
