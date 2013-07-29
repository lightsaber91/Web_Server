#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>
#include <ctype.h>

extern struct server_setting *setting;

struct device_property {
	xmlNode *device;
	char *device_id;
	char *device_fallback_id;
	int resolution_width;
	int resolution_height;
};
