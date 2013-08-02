#define conf_file "/home/lightsaber/.devel/Web_Server/server.conf"

struct server_setting {
	char *ip;
	char *root_folder;
	char *home_page;
	char *log_path;
	char *wurfl_location;
	int port;
	int log_lvl;
	int timeout;
	int MaxKeepAliveReq;
	int KeepAliveTimeout;
	int user_agent_max_attempts;
	bool KeepAlive;
	bool use_wurfl;
	xmlDoc *doc;
	xmlNode *start;
};

void write_parameter(char *line, struct server_setting *s);

struct server_setting *parse_config_file();
