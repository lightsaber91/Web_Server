/**
 * Web Server
 * Copyright (C) 2013 - Simone Martucci <martucci.simone.91@gmail.com>
 *
 *
 * This Program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
**/

#include "../lib/quality_caching.h"
/**
 * This function creates the directories where the images should be saved.
 */
char *create_new_q_dir(int quality) {

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
	char *q_dir = malloc(strlen(cache_dir)+strlen(MAX_DIM_Q)+1);
	if(q_dir == NULL) {
		perror("Memory Allocation Failure\n");
		return NULL;
	}
	//name directory for correct image compression inside CACHE
	if(sprintf(q_dir,"%sq_%d",cache_dir, quality) < 0) {
		perror("In sprintf: nothing written\n");
		return NULL;
	}
	//control if previous directory exist otherwise will be created
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
/**
 * This function search compressed images in correct directories, launch the function to create it if not exists, 
 * and if compressed image doesen't exist call the function to compress it.
 */
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
/**
 * Verify if compressed version of image exists and, if no error occurs, return file compressed, otherwise return original file.
 */
char *cache_by_quality(char *file, int quality) {
	char *new_file = verify_existence_q(file, quality);
	if(new_file == NULL) {
		return file;
	}
	return new_file;
}
