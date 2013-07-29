#include "../lib/wurfl_caching.h"

char *create_new_res_dir(int resolution_width ,int resolution_height) {

	char *cache_dir = malloc(strlen(setting->root_folder)+strlen(CACHE)+1);
	if(cache_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	if(sprintf(cache_dir,"%s%s", setting->root_folder, CACHE) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}
	char *res_dir = malloc(strlen(cache_dir)+strlen(MAX_DIM_RES)+1);
	if(res_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	if(sprintf(res_dir,"%scache/%dx%d/", setting->root_folder, resolution_width, resolution_height) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}
	if(access(cache_dir, F_OK) == 0){
		if(access(res_dir, F_OK) == 0) {
			return res_dir;
		}	
		if(mkdir((const char *)res_dir, 0777) != 0) {
			fprintf(stderr,"Error creating directory\n");
			return NULL;
		}
		return res_dir;
	}
	else {
		if(mkdir((const char *)cache_dir, 0777) != 0) {
			fprintf(stderr,"Error creating directory\n");
			return NULL;
		}
		if(mkdir((const char *)res_dir, 0777) != 0) {
			fprintf(stderr,"Error creating directory\n");
			return NULL;
		}
		return res_dir;
	}
	return NULL;
}

char *get_file_name(char *file){

	char *ap = strrchr(file, '/');
	char *file_name = malloc(strlen(ap) +1);
	if(file_name == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	if(strcpy(file_name, ap) == NULL) {
		perror("In strcpy");
		return NULL;
	}
	return file_name;
}

char *verify_existence_res(int resolution_width ,int resolution_height, char *file){

	char *filename = malloc(strlen(file)-strlen(setting->root_folder)+1);
	if(filename == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	filename = file+strlen(setting->root_folder);
	char *new_dir = create_new_res_dir(resolution_width, resolution_height);
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
		return resize_by_resolution(new_file, file, resolution_width, resolution_height);
	}
}

char *cache_by_resolution(int resolution_width ,int resolution_height ,char *file) {
	char *new_file = verify_existence_res(resolution_width, resolution_height, file);
	if(new_file == NULL) {
		return file;
	}
	return new_file;	
}
