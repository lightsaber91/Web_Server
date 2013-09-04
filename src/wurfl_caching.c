#include "../lib/wurfl_caching.h"
/**
 * This function creates the directories where the images should be saved.
 */
char *create_new_res_dir(int resolution_width ,int resolution_height) {

	char *cache_dir = malloc(strlen(setting->root_folder)+strlen(CACHE)+1);
	if(cache_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	//name CACHE directory inside server ROOT
	if(sprintf(cache_dir,"%s%s", setting->root_folder, CACHE) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}
	char *res_dir = malloc(strlen(cache_dir)+strlen(MAX_DIM_RES)+1);
	if(res_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	//name directory for correct image dimension inside CACHE
	if(sprintf(res_dir,"%sCACHE/%dx%d/", setting->root_folder, resolution_width, resolution_height) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}
	//control if previous directory exist otherwise will be created
	if(access(cache_dir, F_OK) == 0){
		if(access(res_dir, F_OK) == 0) {
			return res_dir;
		}	
		else if(mkdir((const char *)res_dir, 0777) != 0) {
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
/**
 * This function search resized images in correct directories, launch the function to create it if not exists, 
 * and if resized image doesen't exist call the function to resize it.
 */
char *verify_existence_res(int resolution_width ,int resolution_height, char *file){

	char *filename = malloc(strlen(file)-strlen(setting->root_folder)+1);
	if(filename == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	filename = strrchr(file, '/');
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
/**
 * Verify if resized version of image exists and, if no error occurs, return file resized, otherwise return original file.
 */
char *cache_by_resolution(int resolution_width ,int resolution_height ,char *file) {
	char *new_file = verify_existence_res(resolution_width, resolution_height, file);
	if(new_file == NULL) {
		return file;
	}
	return new_file;	
}
