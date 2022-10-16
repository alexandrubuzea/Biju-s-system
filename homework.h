#ifndef HOMEWORK_H_
#define HOMEWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "other.h"

/* A function used to create a directory (memory allocation and 
 * initialization).
 */
Dir *create_dir(Dir *parent, char *name);

/* A function used to create the root of our filesystem (a directory
 * named "home")
 */
Dir *init_filesystem(void);

/* A function which creates a file with the given name in the given
 * directory (similar behaviour with the touch command in linux terminal).
 * It also checks if there is already an existing directory or file with
 * the given name (in this case, the function prints an error and does
 * nothing).
 */
void touch(Dir* parent, char* name);

/* A function which creates a directory with the given name inside the parent
 * directory (similar behaviour with the touch command in linux terminal).
 * It also checks if there is another directory or file with the same given
 * name. In this case, the function prints an error and does nothing.
 */
void mkdir(Dir* parent, char* name);

/* A function that prints on separate lines the directories, then the files
 * in a directory (similar behaviour with the ls command in linux terminal).
 */
void ls(Dir* parent);

/* A function that removes a file with the given name. If there is no such
 * file, the function prints an error and does nothing.
 */
void rm(Dir* parent, char* name);

/* A function that removes a directory, with all its contents (subdirectories
 * and files), unlike the command rmdir from linux terminal (which removes
 * only empty directories). If the directory does not exist, the function
 * prints an error and does nothing.
 */
void rmdir(Dir* parent, char* name);

/* A function that changes the current directory (similar to the cd command
 * in linux terminal). If the directory does not exist, the function prints
 * an error and does nothing.
 */
void cd(Dir** target, char *name);

/* A function that prints the absolute path of the current directory. Similar
 * to the pwd command in linux terminal.
 */
char *pwd(Dir* target);

/* A function that stops the program and frees all the allocated memory
 * #nomemoryleaks <3
 */
void stop(Dir* target);

/* A function that prints a hierarchy of directories, up to a given level.
 * Similar to the "tree -L <level>" command in Linux terminal.
 */
void tree(Dir* target, int level);

/* A function that changes the name of a given file or directory. The function
 * prints errors if there is no file or directory with the given name (oldname)
 * or the name given (newname) is already used. 
 */
void mv(Dir* parent, char *oldname, char *newname);

#endif  /*  HOMEWORK_H_ */
