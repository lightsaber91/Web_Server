#include "../lib/quality_caching.h"

char *create_new_q_dir(char *file, int quality) {

	char *ap = strrchr(file, '/');

	char *dir = malloc(strlen(file));
	if(dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}

	dir = cut_name(file, ap);
	if(dir == NULL) {
		return NULL;
	}
	char *new_dir = malloc(strlen(dir)+strlen(MAX_DIM_Q)+1);
	if(new_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}

	if(sprintf(new_dir,"%s/q_%d/",dir, quality) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}

	if(access(new_dir, F_OK) == 0) {
		return new_dir;
	}	

	if(mkdir((const char *)new_dir, 0777) != 0) {
		fprintf(stderr,"Error creating directory\n");
		return NULL;
	}
	return new_dir;
}

char *verify_existence_q(char *file, int quality) {
	char *file_name = get_file_name(file);
	if(file_name == NULL) {
		return NULL;
	}
	char *new_dir = create_new_q_dir(file, quality);
	if(new_dir == NULL) {
		return NULL;
	}
	char *new_file = malloc(strlen(new_dir)+strlen(file_name)+1);
	if(new_file == NULL) {
		perror("Memory Allocation Failure");
		return NULL;
	}

	if(sprintf(new_file,"%s%s",new_dir, file_name) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}
	if(access(new_file, F_OK) == 0) {
		return new_file;
	}
	else {
		return resize_by_quality(new_file, file, quality);
	}
}

char *cache_by_quality(char *file, int quality) {
	char *new_file = verify_existence_q(file, quality);
	if(new_file == NULL) {
		return file;
	}
	return new_file;
}
