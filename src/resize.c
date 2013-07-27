#include "../lib/resize.h"

char *resize_by_resolution(char *new_file, char *file, int resolution_width, int resolution_height) {

	MagickWand *m_wand = NULL;

	MagickWandGenesis();

	m_wand = NewMagickWand();
	// Read the image - all you need to do is change "logo:" to some other
	// filename to have this resize and, if necessary, convert a different file
	MagickReadImage(m_wand, file);

	// Resize the image using the Lanczos filter
	// The blur factor is a "double", where > 1 is blurry, < 1 is sharp
	// I haven't figured out how you would change the blur parameter of MagickResizeImage
	// on the command line so I have set it to its default of one.
	MagickResizeImage(m_wand, resolution_width, resolution_height, LanczosFilter,1);

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
