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

/**
 * The config file will be located in the same directory of server
 */
#define conf_file_opt "/opt/tvs/server.conf"
#define conf_file "./server.conf"

/**
 * Definition of server setting and every parameter
 */
typedef struct server_setting {
	char *ip;
	char *root_folder;
	char *home_page;
	char *log_path;
	char *wurfl_location;
	char *mime_type_file;
	int port;
	int timeout;
	int MaxKeepAliveReq;
	int KeepAliveTimeout;
	int user_agent_max_attempts;
	bool KeepAlive;
	bool use_wurfl;
	long pend_connection;
	xmlDoc *doc;
	xmlNode *start;
} SETTING;

