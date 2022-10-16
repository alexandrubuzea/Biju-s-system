#ifndef STRUCT_H_
#define STRUCT_H_

struct Dir;
struct File;

/* the used structures for the filesystem (directory and file respectively) */

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

#endif  /* STRUCT_H_ */
