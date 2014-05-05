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

#include "../lib/mime_type.h"

/**
 * Read extension and type from file and save in memory.
 */
void save_mime(char *line, MIME *t, int n) {
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
MIME *load_mime_type(char *file) {
	//Open File
	FILE *mime = fopen(file, "r");
	if(mime == NULL) {
		perror("MIME file NOT FOUND!\n");
		exit(EXIT_FAILURE);
	}
	//Create struct
	MIME *types = malloc(sizeof(MIME));
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
			types = realloc(types, N_line*sizeof(MIME));	
		}
		
	}
	//close file
	fclose(mime);
	return types;
}
