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

#include "../lib/resize.h"
/**
 * Function that maintain original ratio of image.
 */
int maintainRatio(size_t initial_w, int resolution_w, size_t *fixed_w, size_t initial_h, int resolution_h, size_t *fixed_h) {

	if(initial_w == 0 || initial_h == 0) {
		return -1;
	}
	if(initial_w > initial_h) {
		double ratio = (double)initial_w/(double)initial_h;
		*fixed_w = resolution_w;
		*fixed_h = (double)(*fixed_w)/ratio;
		return 0;
	}
	else {
		double ratio = (double)initial_w/(double)initial_h;
		*fixed_h = resolution_h;
		*fixed_w = (double)(*fixed_h)*ratio;
		return 0;
	}
	return -1;
}
/**
 * Uses magickwand to resize immage correctly
 */
char *resize_by_resolution(char *new_file, char *file, int resolution_width, int resolution_height) {

	MagickWand *m_wand = NULL;
	size_t initial_width = 0, initial_height = 0, fixed_width = 0, fixed_height = 0;

	MagickWandGenesis();

	m_wand = NewMagickWand();
	if(m_wand == (MagickWand *) NULL){
		perror("Error inizializing new MagickWand\n");
		return file;	
	}
	// Read the image
	if(MagickReadImage(m_wand, file) == MagickFalse) {
		return file;
	}

	//Get initial size of image
	initial_width = MagickGetImageWidth(m_wand);
	initial_height = MagickGetImageHeight(m_wand);
	// Resize the image using the Lanczos filter
	// The blur factor is a "double", where > 1 is blurry, < 1 is sharp
	if(maintainRatio(initial_width, resolution_width, &fixed_width, initial_height, resolution_height, &fixed_height) == 0) {
		if(MagickResizeImage(m_wand, fixed_width, fixed_height, LanczosFilter,1) == MagickFalse) {
			return file;
		}
	}
	else {
		if(MagickResizeImage(m_wand, resolution_width, resolution_height, LanczosFilter, 1) == MagickFalse) {
			return file;
		}
	}
	// Set the compression quality to 95 (high quality = low compression)
	if(MagickSetImageCompressionQuality(m_wand, 95) == MagickFalse) {
		return file;
	}

	// Write the new image 
	if(MagickWriteImage(m_wand, new_file) == MagickFalse) {
		return file;
	}

	// Clean up 
	if(m_wand)m_wand = DestroyMagickWand(m_wand);

	MagickWandTerminus();
	return new_file;
}
/**
 * Uses magickwand to compress immage correctly
 */
char *resize_by_quality(char *new_file, char *file, int quality) {
	MagickWand *m_wand = NULL;

	MagickWandGenesis();

	m_wand = NewMagickWand();
	if(m_wand == (MagickWand *) NULL){
		perror("Error inizializing new MagickWand\n");
		return file;	
	}
	// Read the image
	if(MagickReadImage(m_wand, file) == MagickFalse) {
		return file;
	}
	// Set the compression quality (high quality = low compression)
	if(MagickSetImageCompressionQuality(m_wand, quality) == MagickFalse) {
		return file;
	}

	// Write the new image 
	if(MagickWriteImage(m_wand, new_file) == MagickFalse) {
		return file;
	}

	// Clean up 
	if(m_wand)m_wand = DestroyMagickWand(m_wand);

	MagickWandTerminus();
	return new_file;
}
