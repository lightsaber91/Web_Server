#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_DIM "9999x9999"

char *cut_name(char *file, char *ext) {
	char *new = malloc(strlen(file)+1);
	if(new == NULL) {
		perror("Memory Allocation Failure\n");
		return file;
	}
	new = strcpy(new, file);
	int i = strlen(new) - strlen(ext);
	*(new+i) = '\0';
	return new;
}

char *create_new_dir(char *file, int resolution_width ,int resolution_height) {

	char *ap = strrchr(file, '/');

	char *dir = malloc(strlen(file));
	if(dir == NULL) {
		perror("Memory Allocation Failure\n");
		return file;
	}

	dir = cut_name(file, ap);

	char *new_dir = malloc(strlen(dir)+strlen(MAX_DIM)+3);
	if(new_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return file;
	}

	if(sprintf(new_dir,"%s/%dx%d/",dir, resolution_width, resolution_height) < 0) {
		perror("In sprintf: nothing written\n");
		return file;
	}

	if(access(new_dir, F_OK) == 0) {
		return new_dir;
	}	

	if(mkdir((const char *)new_dir, 0777) != 0)
		fprintf(stderr,"Error creating directory\n");

	return new_dir;
}

char *get_file_name(char *file){

	char *ap = strrchr(file, '/');
	char *file_name = malloc(strlen(ap) +1);
	if(file_name == NULL) {
		perror("Memory Allocation Failure\n");
		return file;
	}
	if(strcpy(file_name, ap) == NULL) {
		perror("In strcpy");
		return file;
	}
	return file_name;
}

char *verify_existence(int resolution_width ,int resolution_height, char *new_dir, char *file){

	char *file_name = get_file_name(file);
	new_dir = create_new_dir(file, resolution_width, resolution_height);
	char *new_file = malloc(strlen(new_dir)+strlen(file_name)+1);
	if(new_file == NULL) {
		perror("Memory Allocation Failure");
		return file;
	}

	if(sprintf(new_file,"%s%s",new_dir, file_name) < 0) {
		perror("In sprintf: nothing written\n");
		return file;
	}
	if(access(new_file, F_OK) == 0) {
		return new_file;
	}
	else {
		return resize(new_file, file, resolution_width, resolution_height);
	}
}

char *cache_image(int resolution_width ,int resolution_height ,char *file) {
	char *name_dir;
	char *new_file = verify_existence(resolution_width, resolution_height, name_dir, file);
	return new_file;	
}
