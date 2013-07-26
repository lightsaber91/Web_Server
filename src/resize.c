#include "../lib/resize.h"

char *cut_name(char *file, char *ext) {
	int i = strlen(file) - strlen(ext);
	*(file+i) = '\0';
	return file;
}

char *resize(char *file)
{

	char *ap = strrchr(file, '.');
	char *ext = malloc(strlen(ap) +1);
	if(ext == NULL) {
		perror("Memory Allocation Failure\n");
		return file;
	}
	if(strcpy(ext, ap) == NULL) {
		perror("In strcpy");
		return file;
	}
	char *no_ext = malloc(strlen(file));
	if(no_ext == NULL) {
		perror("Memory Allocation Failure\n");
		return file;
	}
	MagickWand *m_wand = NULL;

	int width,height;

	MagickWandGenesis();

	m_wand = NewMagickWand();
	// Read the image - all you need to do is change "logo:" to some other
	// filename to have this resize and, if necessary, convert a different file
	MagickReadImage(m_wand, file);

	no_ext = cut_name(file, ap);
	char *img_resized = malloc(strlen(no_ext)+strlen("_resized")+strlen(ext)+1);
	if(img_resized == NULL) {
		perror("Memory Allocation Failure\n");
		return file;
	}
	if(sprintf(img_resized, "%s_resized%s", no_ext, ext) < 0) {
		perror("In sprintf: nothing written\n");
		return file;
	}

	if(access(img_resized, F_OK) == 0) {
		return img_resized;
	}

	// Get the image's width and height
	width = MagickGetImageWidth(m_wand);
	height = MagickGetImageHeight(m_wand);

	// Cut them in half but make sure they don't underflow
	if((width /= 2) < 1)width = 1;
	if((height /= 2) < 1)height = 1;

	// Resize the image using the Lanczos filter
	// The blur factor is a "double", where > 1 is blurry, < 1 is sharp
	// I haven't figured out how you would change the blur parameter of MagickResizeImage
	// on the command line so I have set it to its default of one.
	MagickResizeImage(m_wand,width,height,LanczosFilter,1);

	// Set the compression quality to 95 (high quality = low compression)
	MagickSetImageCompressionQuality(m_wand,95);

	/* Write the new image */
	MagickWriteImage(m_wand, img_resized);

	/* Clean up */
	if(m_wand)m_wand = DestroyMagickWand(m_wand);

	MagickWandTerminus();
	return img_resized;
}
