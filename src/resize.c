#include "../lib/resize.h"

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

char *resize_by_resolution(char *new_file, char *file, int resolution_width, int resolution_height) {

	MagickWand *m_wand = NULL;
	size_t initial_width = 0, initial_height = 0, fixed_width = 0, fixed_height = 0;

	MagickWandGenesis();

	m_wand = NewMagickWand();
	// Read the image - all you need to do is change "logo:" to some other
	// filename to have this resize and, if necessary, convert a different file
	MagickReadImage(m_wand, file);

	//Get initial size of image
	initial_width = MagickGetImageWidth(m_wand);
	initial_height = MagickGetImageHeight(m_wand);
	// Resize the image using the Lanczos filter
	// The blur factor is a "double", where > 1 is blurry, < 1 is sharp
	// I haven't figured out how you would change the blur parameter of MagickResizeImage
	// on the command line so I have set it to its default of one.
	if(maintainRatio(initial_width, resolution_width, &fixed_width, initial_height, resolution_height, &fixed_height) == 0) {
		MagickResizeImage(m_wand, fixed_width, fixed_height, LanczosFilter,1);
	}
	else {
		MagickResizeImage(m_wand, resolution_width, resolution_height, LanczosFilter,1);
	}
	// Set the compression quality to 95 (high quality = low compression)
	MagickSetImageCompressionQuality(m_wand, 95);

	// Write the new image 
	MagickWriteImage(m_wand, new_file);

	// Clean up 
	if(m_wand)m_wand = DestroyMagickWand(m_wand);

	MagickWandTerminus();
	return new_file;
}

char *resize_by_quality(char *new_file, char *file, int quality) {
	MagickWand *m_wand = NULL;

	MagickWandGenesis();

	m_wand = NewMagickWand();
	// Read the image - all you need to do is change "logo:" to some other
	// filename to have this resize and, if necessary, convert a different file
	MagickReadImage(m_wand, file);
	// Set the compression quality to 95 (high quality = low compression)
	MagickSetImageCompressionQuality(m_wand, quality);

	// Write the new image 
	MagickWriteImage(m_wand, new_file);

	// Clean up 
	if(m_wand)m_wand = DestroyMagickWand(m_wand);

	MagickWandTerminus();
	return new_file;
}
