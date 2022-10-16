#include "homework.h"

/* not a standard C function, so I created it myself
 */
static char *my_strdup(char *name)
{
    char *copy = (char *)malloc((strlen(name) + 1) * sizeof(char));
    CHECK(copy); /* always check malloc #defensiveprogramming */

    strcpy(copy, name);
    return copy;
}

Dir *create_dir(Dir *parent, char *name)
{
    Dir *new = (Dir *)malloc(sizeof(Dir));
    CHECK(new); /* always check malloc #defensiveprogramming */
    
    new->name = my_strdup(name);
    new->parent = parent;

    /* the initialization for these fields will be made in other
     * functions (such as mkdir) or when a file/directory is created
     * inside this directory
     */
    new->head_children_files = NULL;
    new->head_children_dirs = NULL;
    new->next = NULL;

    return new;
}

static File *create_file(Dir *parent, char *name)
{
    File *new = (File *)malloc(sizeof(File));
    CHECK(new); /* always check malloc #defensiveprogramming */

    new->name = my_strdup(name);

    new->parent = parent;
    new->next = NULL;

    return new;
}

/* A function that frees the allocated memory for a file struct
 * #nomemoryleaks
 */
static void destroy_file(File *target)
{
    free(target->name);
    free(target);
}

Dir *init_filesystem(void)
{
	return create_dir(NULL, "home");
}

void touch(Dir *parent, char *name)
{
    Dir *d_it = parent->head_children_dirs;

    /* Iterating through all subdirectories
     */
    while (d_it != NULL) {
        if (!strcmp(d_it->name, name)) {
            printf("File already exists\n");
            return;
        }
        d_it = d_it->next;
    }

    if (parent->head_children_files == NULL) {
        File *new = create_file(parent, name);
        parent->head_children_files = new;
        return;
    }

    File *it = parent->head_children_files, *prev = it;

    /* Iterating through all files
     */
    while (it != NULL) {
        if (!strcmp(it->name, name)) {
            printf("File already exists\n");
            return;
        }

        prev = it;
        it = it->next;
    }

    /* Creating a file struct at the end of the list
     */
    prev->next = create_file(parent, name);
}

void mkdir(Dir *parent, char *name)
{
    File *f_it = parent->head_children_files;

    /* Iterating through all files
     */
    while (f_it != NULL) {
        if (!strcmp(f_it->name, name)) {
            printf("Directory already exists\n");
            return;
        }
        f_it = f_it->next;
    }

    if (parent->head_children_dirs == NULL) {
        Dir *new = create_dir(parent, name);
        parent->head_children_dirs = new;
        return;
    }

    Dir *it = parent->head_children_dirs, *prev = it;

    /* Iterating through all directories
     */
    while (it != NULL) {
        if(!strcmp(it->name, name)) {
            printf("Directory already exists\n");
            return;
        }

        prev = it;
        it = it->next;
    }

    prev->next = create_dir(parent, name);
}

void ls(Dir *parent)
{
    Dir *d_it = parent->head_children_dirs;

    /* Iterating through all directories
     */
    while (d_it != NULL) {
        printf("%s\n", d_it->name);
        d_it = d_it->next;
    }

    File *f_it = parent->head_children_files;

    /* Iterating through all files
     */
    while (f_it != NULL) {
        printf("%s\n", f_it->name);
        f_it = f_it->next;
    }
}

void rm(Dir *parent, char *name)
{
    File *it = parent->head_children_files, *prev = it;

    while (it != NULL) {
        /* If we found the file to be removed
         */
        if (!strcmp(it->name, name)) {
            /* If the file to be removed is the first file in the file list
             */
            if (prev == it) {
                parent->head_children_files = it->next;
                destroy_file(it);
                return;
            }

            prev->next = it->next;
            it->next = NULL;

            /* freeing the allocated memory #nomemoryleaks
             */
            destroy_file(it);
            return;
        }

        prev = it;
        it = it->next;
    }

    printf("Could not find the file\n");
}

static void destroy_file_chain(File *target)
{
    File *it = target, *prev = it;

    while (it != NULL) {
        prev = it;
        it = it->next;

        /* freeing the allocated memory #nomemoryleaks
         */
        destroy_file(prev);
    }
}

/* We have two recursive functions using each other
 */
static void __rmdir(Dir *target);

static void destroy_dir_chain(Dir *target)
{
    Dir *it = target, *prev = it;

    while (it != NULL) {
        prev = it;
        it = it->next;

        /* freeing all the allocated memory for the directory and its
         * content #nomemoryleaks
         */
        __rmdir(prev);
    }
}

static void __rmdir(Dir *target)
{
    if (!target) {
        return;
    }

    /* destroying all the current directory's content
     * #nomemoryleaks
     */
    destroy_file_chain(target->head_children_files);
    destroy_dir_chain(target->head_children_dirs);

    free(target->name);
    free(target);
}

void rmdir(Dir *parent, char *name)
{
    Dir *it = parent->head_children_dirs, *prev = it;

    while (it != NULL) {
        if (!strcmp(it->name, name)) {
            /* If the found directory is the first directory in the list
            */
            if (prev == it) {
                parent->head_children_dirs = it->next;

                /* remove the found directory with the given name and all
                 * its content (of course, #nomemoryleaks)
                 */
                __rmdir(it);
                return;
            }

            prev->next = it->next;

            /* remove the found directory with the given name and all
             * its content (of course, #nomemoryleaks)
             */
            __rmdir(it);
            return;
        }

        prev = it;
        it = it->next;
    }

    printf("Could not find the dir\n");   
}

void cd(Dir** target, char *name)
{
    /* if we want to change the directory to the parent directory
     */
    if (!strcmp(name, "..")) {
        if ((*target)->parent)
            *target = (*target)->parent;
        return;
    }

    Dir *it = (*target)->head_children_dirs;

    /* If the directory is not the parent, but a subdirectory
     */
    while (it != NULL) {
        if (!strcmp(it->name, name)) {
            *target = it;
            return;
        }

        it = it->next;
    }

    printf("No directories found!\n");
}

char *pwd(Dir* target)
{
    Dir *it = target;
    int len = 0;

    /* calculating the length of the path to be returned
     */
    while (it != NULL) {
        len += (int)strlen(it->name);
        ++len;
        it = it->parent;
    }

    char *current_path = (char *)malloc((len + 1) * sizeof(char));
    current_path[len] = '\0';

    /* pos will always be the greatest index of an unitialised char
     */
    int pos = len - 1; 

    it = target;
    while (it != NULL) {
        int tmp_len = strlen(it->name);
        pos -= tmp_len;
        memcpy(current_path + pos + 1, it->name ,tmp_len * sizeof(char));
        pos--;
        memcpy(current_path + pos + 1, "/", 1);

        it = it->parent;
    }

    return current_path;
}

void stop(Dir* target)
{
    /* Using the already-built __rmdir function, which recursively
     * frees all the allocated memory of a given directory (#nomemoryleaks)
     */
    __rmdir(target);
}

/* A function which prints some spaces to the stdout (indent for the
 * tree function).
 */
static void print_indent(int level)
{
    for (int i = 0; i < level; ++i) {
        printf("    ");
    }
}

void tree(Dir* target, int level)
{
    Dir *d_it = target->head_children_dirs;

    while (d_it != NULL) {
        print_indent(level);
        printf("%s\n", d_it->name);
        tree(d_it, level + 1);

        d_it = d_it->next;
    }

    File *f_it = target->head_children_files;

    while (f_it != NULL) {
        print_indent(level);
        printf("%s\n", f_it->name);

        f_it = f_it->next;
    }
}

/* helper function for mv command/function
 */
static int has_dir(Dir *parent, char *name)
{
    Dir *it = parent->head_children_dirs;

    while (it != NULL) {
        if (!strcmp(it->name, name))
            return 1;
        
        it = it->next;
    }
    
    return 0;
}

/* helper function for mv command/function
 */
static int has_file(Dir *parent, char *name)
{
    File *it = parent->head_children_files;

    while (it != NULL) {
        if (!strcmp(it->name, name))
            return 1;

        it = it->next;
    }

    return 0;
}

void mv(Dir* parent, char *oldname, char *newname)
{
    if (has_file(parent, newname) == 1 || has_dir(parent, newname) == 1) {
        printf("File/Director already exists\n");
        return;
    }

    /* If the given name is a file's, find the file and
     * change its name
     */
    if (has_file(parent, oldname) == 1) {
        File *it = parent->head_children_files, *prev = it, *to_change = NULL;

        if (it->next == NULL) {
            free(it->name);
            it->name = my_strdup(newname);
            return;
        }

        while (it != NULL) {
            if (!strcmp(it->name, oldname)) {
                /* if the wanted file is the first
                 */
                if (prev == it) {
                    parent->head_children_files = it->next;
                    it = it->next;
                    prev->next = NULL;
                    to_change = prev;
                } else {
                    to_change = it;
                    prev->next = it->next;
                    to_change->next = NULL;
                    it = prev;
                }
            }
            prev = it;
            it = it->next;
        }

        prev->next = to_change;
        free(to_change->name);
        to_change->name = my_strdup(newname);

        return;
    }

    /* If the given name is a directory's, find the directory and
     * change its name
     */
    if (has_dir(parent, oldname) == 1) {
        Dir *it = parent->head_children_dirs, *prev = it, *to_change = NULL;

        if (it->next == NULL) {
            free(it->name);
            it->name = my_strdup(newname);

            return;
        }

        while (it != NULL) {
            if (!strcmp(it->name, oldname)) {
                if (prev == it) {
                    parent->head_children_dirs = it->next;
                    it = it->next;
                    prev->next = NULL;
                    to_change = prev;
                } else {
                    to_change = it;
                    prev->next = it->next;
                    to_change->next = NULL;
                    it = prev;
                }
            }

            prev = it;
            it = it->next;
        }

        prev->next = to_change;
        free(to_change->name);
        to_change->name = my_strdup(newname);
        return;
    }

    /* If there is no file/directory with the given oldname
     */
    printf("File/Director not found\n");
}
