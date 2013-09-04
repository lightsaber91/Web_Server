#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Struct used for type file
typedef struct extn{
	char *ext;
	char *mediatype;
} MIME;
MIME *extensions;

