#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Struct used for type file
struct extn{
	char *ext;
	char *mediatype;
};
struct extn *extensions;

