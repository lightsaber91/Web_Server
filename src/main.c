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

#include "../lib/main.h"

/**
 * Web Server main function.
 * Set signal-handler for terminating, parse configuration file and mime types file.
 * Create connections and launch threads to manage these. 
 */
int main() {

	//Socket Stuff
	int skt_lst, skt_accpt;
	struct sockaddr_in skaddr;
	socklen_t socksize;

	//Initiate signal handler for sigterm & sigint
	signal(SIGTERM, sigterm_handler);
	signal(SIGINT, sigint_handler);
	//Read conf file and save parameters
	setting = parse_config_file();

	//Load mime types from file
	extensions = load_mime_type(setting->mime_type_file);

	if(toLog > -1) {
		//Open Log File
		LogFile = openLogFile(setting->log_path);
		if(LogFile == NULL)
			toLog = -1;	
	}
	if(setting->use_wurfl == true) {
		//Open wurfl file and build the xml three
		setting->doc = initDoc(setting->wurfl_location);
		if(setting->doc == NULL) {
			setting->use_wurfl = false;
		}
		setting->start = getDevicesNode(setting->doc);
		if(setting->start == NULL) {
			xmlFree(setting->doc);
			setting->use_wurfl = false;
		}
	}
	//Create socket
	skt_lst = create_and_bind(skaddr, socksize);
	//Write On Log File when server started
	if(toLog > -1)
		writeInfoLog(START, NULL);

	for(;;) {
		if(max_thread == 0 || n_thread < max_thread) {
			//Take connection and launch thread to manage it
			if(listen(skt_lst, setting->pend_connection) == -1) {
				perror("Socket Listening Error\n");
				return(EXIT_FAILURE);
			}
	
			if((skt_accpt = accept(skt_lst, (struct sockaddr *)&skaddr, &socksize)) == -1){
				perror("Socket Accepting Error\n");
				return(EXIT_FAILURE);
			}
			n_thread++;
			create_thread(setting, skt_accpt);
		}
		else {
			again:
				usleep(1);
				if(n_thread >= max_thread) goto again;
		}
	}
	//Close socket and exit
	if(close(skt_lst) == -1) {
		perror("Socket Closing Error\n");
	}
	return EXIT_SUCCESS;

}
