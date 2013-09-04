#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>
#include <ctype.h>

#define WURFL_NOT_FOUND "Wurfl XML repository not found at path specified by user. Image processing will be disabled."
#define RES_NOT_FOUND "Resolution not found in wurfl: resizing disabled."
#define UA_NOT_FOUND "User Agent NOT found in wurlf. No resizing"

typedef struct device_property {
	xmlNode *device;
	char *device_id;
	char *device_fallback_id;
	int resolution_width;
	int resolution_height;
}DEVICE;
