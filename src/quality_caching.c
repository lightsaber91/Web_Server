#include "../lib/quality_caching.h"

char *create_new_q_dir(int quality) {

	char *cache_dir = malloc(strlen(setting->root_folder)+strlen(CACHE)+1);
	if(cache_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	if(sprintf(cache_dir,"%s%s", setting->root_folder, CACHE) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}
	char *q_dir = malloc(strlen(cache_dir)+strlen(MAX_DIM_Q)+1);
	if(q_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}

	if(sprintf(q_dir,"%sq_%d",cache_dir, quality) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}

	if(access(cache_dir, F_OK) == 0) {
		if(access(q_dir, F_OK) == 0) {
			return q_dir;
		}	
		if(mkdir((const char *)q_dir, 0777) != 0) {
			fprintf(stderr,"Error creating directory\n");
			return NULL;
		}
		return q_dir;
	}
	else {
		if(mkdir((const char *)cache_dir, 0777) != 0) {
			fprintf(stderr,"Error creating directory\n");
			return NULL;
		}
		if(mkdir((const char *)q_dir, 0777) != 0) {
			fprintf(stderr,"Error creating directory\n");
			return NULL;
		}
		return q_dir;
	}
	return NULL;
}

char *verify_existence_q(char *file, int quality) {

	char *filename = malloc(strlen(file)-strlen(setting->root_folder)+1);
	if(filename == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	filename = strrchr(file, '/');
	char *new_dir = create_new_q_dir(quality);
	if(new_dir == NULL) {
		return NULL;
	}
	char *new_file = malloc(strlen(new_dir)+strlen(filename)+1);
	if(new_file == NULL) {
		perror("Memory Allocation Failure");
		return NULL;
	}

	if(sprintf(new_file,"%s%s",new_dir, filename) < 0) {
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
