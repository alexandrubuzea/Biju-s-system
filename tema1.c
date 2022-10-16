#include "homework.h" 

#define MAX_INPUT_LINE_SIZE 300

int main(void)
{
	Dir *root = init_filesystem(); /* creating home directory */
	Dir *current = root;

	const char delim[] = "\t ";
	char *command = (char *)malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	CHECK(command); 

	while (fgets(command, MAX_INPUT_LINE_SIZE, stdin)) {
		if (command[strlen(command) - 1] == '\n') {
			command[strlen(command) - 1] = '\0'; /* adding null terminator */
		}

		/* getting the first input string, which can be one of the
		 * functions below
		 */

		char *type = strtok(command, delim);

		if (!strcmp(type, "touch")) {
			char *filename = strtok(NULL, delim);
			touch(current, filename);

		} else if (!strcmp(type, "mkdir")) {
			char *dirname = strtok(NULL, delim);
			mkdir(current, dirname);

		} else if (!strcmp(type, "ls")) {
			ls(current);

		} else if (!strcmp(type, "rm")) {
			char *filename = strtok(NULL, delim);
			rm(current, filename);

		} else if (!strcmp(type, "rmdir")) {
			char *dirname = strtok(NULL, delim);
			rmdir(current, dirname);

		} else if (!strcmp(type, "cd")) {
			char *dirname = strtok(NULL, delim);
			cd(&current, dirname);

		} else if (!strcmp(type, "pwd")) {
			char *current_dir = pwd(current);
			printf("%s\n", current_dir);
			free(current_dir);

		} else if (!strcmp(type, "tree")) {
			tree(current, 0);

		} else if (!strcmp(type, "mv")) {
			char *oldname = strtok(NULL, delim);
			char *newname = strtok(NULL, delim);
			mv(current, oldname, newname);

		} else if (!strcmp(type, "stop")) {
			stop(root);
			free(command);
			return 0; /* the program stops */
		}
	}
}
