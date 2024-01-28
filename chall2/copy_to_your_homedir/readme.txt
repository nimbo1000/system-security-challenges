Race condition challenge
========================

suid_cat.c is an example of a program vulnerable to a file race. The
file race attack allows to read the contents of the "private" file
form a normal user account by leveraging the file race vulerability in
the suid_cat program.


You can copy the files in copy_to_your_homedir to your home dir to be
able to use/modify them, the scat program, private file and the
clear_cache program must be used in place.

The work consists in :

1) Writing a script to build the maze.
2) Complete the attack script that should change the target of the
link (and possibly the restore script, which is handy is the attack
failed for some reason).
3) Run the attack.
4) Obtain the token in the private file and submit it as a file containing the token only using the comand :
submit chall2 tokenfile

Then please clean your maze, those files are quickly filling the disk (it's inodes actually).
