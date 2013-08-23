#include "../lib/mime_type.h"

/**
 * Read extension and type from file and save in memory.
 */
void save_mime(char *line, struct extn *t, int n) {
	if(line == NULL) {
		t[n].ext = (char *)0;
		t[n].mediatype = (char *)0;
	}
	else {
		t[n].ext = strtok(line, ",");
		t[n].mediatype = strtok(NULL, "\n");
	}
}
/**
 * Load mime type from file to an array of struct.
 */
struct extn *load_mime_type(char *file) {
	//Open File
	FILE *mime = fopen(file, "r");
	if(mime == NULL) {
		perror("MIME file NOT FOUND!\n");
		exit(EXIT_FAILURE);
	}
	//Create struct
	struct extn *types = malloc(sizeof(struct extn));
	if(types == NULL) {
		perror("Memory Allocation Failure\n");
		exit(EXIT_FAILURE);
	}
	size_t n = 10;
	int N_line = 1;
	while(1) {
		char *line = NULL;
		//read line
		if(getdelim(&line, &n, '\n', mime) == -1) {
			perror("In getdelim\n");
		}

		if( strncmp(line, "EOF" ,3) == 0 ) {
			save_mime(NULL, types, N_line-1);
			break;
		}
		//If mime type increase array dimension and save new mime
		if( strncmp(line, "#", 1) != 0 ) {
			save_mime(line, types, N_line-1);
			N_line++;
			types = realloc(types, N_line*sizeof(struct extn));	
		}
		
	}
	//close file
	fclose(mime);
	return types;
}
