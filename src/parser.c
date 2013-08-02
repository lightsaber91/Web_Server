#include "../lib/parser.h"

xmlDoc *initDoc(char *wurfl_location) {
	xmlDoc *doc;
	if (access(wurfl_location, R_OK) == 0) {
        	//build the tree from the xml file
        	doc = xmlReadFile(wurfl_location, NULL, 0);
        	if (doc == NULL ) {
           		perror("Document not parsed successfully\n");
			return NULL;
        	}
		return doc;
	}
        perror("Wurfl XML repository not found at path specified by user. Image processing will be disabled.\n");
	return NULL;
}

xmlNode *getDevicesNode(xmlDoc *doc) {
	xmlNode *start = xmlDocGetRootElement(doc);
       	if (start == NULL) {
      		perror("Empty document\n");
       		return NULL;
       	}
	if (strcmp((char *)start->name, "wurfl") != 0) {
		perror("Document of the wrong type, root node != wurfl\n");
       		return NULL;
       	}
	start = start->xmlChildrenNode;
        while (start != NULL) {
		if (strcmp((char *)start->name, "devices") == 0)
			return start;
		start = start->next;
	}
	return NULL;
}

int findDeviceNodeByUserAgent(xmlNode *node, char *header_useragent, struct device_property *property) {
	xmlNode *cur = node;
	//start checking every device
	cur = cur->xmlChildrenNode;
	xmlChar *user_agent;
	while (cur != NULL) {
	        //check if this is a device element
		if (strcmp((char *)cur->name, "device") == 0) {
        		//extract user-agent of the selected device element
        		user_agent = xmlGetProp(cur, (const xmlChar *)"user_agent");
            	
        		//compare the given user-agent with the one of the selected device element
        		if (strstr((char *)user_agent, header_useragent) != NULL) {
        			//ok, device found, returning...
				property->device = cur;
                		xmlFree(user_agent);
				return 0;
            		}
            		xmlFree(user_agent);
        	}
		cur = cur->next;
	}
	return -1;
}

void reduce_user_agent(char *user_agent) {
	char* end;
    	int len = strlen(user_agent);
	end = user_agent + strlen(user_agent) - 1;
	while(!isspace(*end) && len>0) {
		end--;
		len--;
	}
	if(len == 0) {
		user_agent = NULL;
		return;
	}
    	end--;
    	*(end+1) = '\0';
}

char *getDeviceId(xmlNode *device)
{
	return (char *)xmlGetProp((device), (const xmlChar *)"id");
}

char *getDeviceFallBackId(xmlNode *device)
{
	return (char *)xmlGetProp((device), (const xmlChar *)"fall_back");
}

void getDeviceDimension(xmlNode *node, int *width, int *height) {
	xmlNode *cur = node;
	cur = cur->xmlChildrenNode;
	xmlChar *id, *name;
	while(cur != NULL) {
		if(strcmp((char *) cur->name, "group") == 0) {
			id = xmlGetProp(cur, (const xmlChar *)"id");
			if(strcmp((char *)id, "display") == 0) {
				cur = cur->xmlChildrenNode;
				while(cur != NULL) {
					if(strcmp((char *) cur->name, "capability") == 0) {
						if(*width != 0 && *height != 0) {
							break;
						}
						name = xmlGetProp(cur, (const xmlChar *)"name");
						if(strcmp((char *)name, "resolution_width") == 0) {
							*width = atoi((char *) xmlGetProp(cur, (const xmlChar *)"value"));
						}
						if(strcmp((char *)name, "resolution_height") == 0) {
							*height = atoi((char *) xmlGetProp(cur, (const xmlChar *)"value"));
						}
						xmlFree(name);
					}
					cur = cur->next;
				}
				xmlFree(id);
				return;
			}
			xmlFree(id);
		}
		cur = cur->next;
	}
	return;
}

int parse_UA(char *user_agent, struct device_property *property, xmlNode *start) {

	int max_attempts = setting->user_agent_max_attempts;
	xmlNode *cur = start;
	while(user_agent != NULL && max_attempts > 0) {
		if(findDeviceNodeByUserAgent(cur, user_agent, property) == 0) {
			property->device_id = getDeviceId(property->device);
			property->device_fallback_id = getDeviceFallBackId(property->device);
			property->resolution_width = 0;
			property->resolution_height = 0;
			getDeviceDimension(property->device, &property->resolution_width, &property->resolution_height);
			if(property->resolution_width == 0 && property->resolution_height == 0) {
				perror("Resolution not found in wurfl: resizing disabled.\n");
				return -1;
			}
			return 0;
		}
		reduce_user_agent(user_agent);
		max_attempts--;
	}
	return -1;
}
