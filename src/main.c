#include "../lib/main.h"

/**
 * Web Server main function.
 * Set signal-handler for terminating, parse configuration file and mime types file.
 * Create connections and launch threads to manage these. 
 */
int main() {
	printf("Starting server...\n");
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

	bool toLog = false;
	if( setting->log_lvl > -1 ) {
		//Open Log File
		LogFile = openLogFile(setting->log_path);
		if(LogFile != NULL) 
			toLog = true;
		
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
	printf("Server started successfully!\n");
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
			create_thread(setting, skt_accpt, toLog, LogFile);
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
