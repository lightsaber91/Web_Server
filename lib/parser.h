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
