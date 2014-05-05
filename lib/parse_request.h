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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct client_request{
	char *method;
	char *file_requested;
	char *user_agent;
	char *connection_type;
	char *http_version;
	char *host;
	int accept_quality;
}HTTP_CONN;

int get_quality_factor(char *accept_type);

void parse_request(char *line, HTTP_CONN *request);

void parse_first_line(char *line, HTTP_CONN *request);

HTTP_CONN *parse_client_request(char *message);
