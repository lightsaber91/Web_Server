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

