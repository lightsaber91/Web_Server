#include "../lib/parser.h"

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

int getDeviceDimension(xmlNode *device, int *width, int *height) {

	xmlNode *cur = device;
	cur = cur->children;
	xmlChar *id;
	while ((xmlStrcmp(cur->name, (const xmlChar *)"group")) == 0) {
		printf("entrato\n");
		id = xmlGetProp(cur, (const xmlChar *)"id");
		printf("\n%s\n", (char *)id);
		if(strcmp((char *)id, "display") == 0) {
			//prendo misure;
		}
		cur = cur->next;
	}
	return -1;
}

int parse_UA(char *user_agent, struct device_property *property) {

	xmlDoc *doc=NULL;
	xmlNode *cur=NULL;

	char *wurfl_location = "/home/W.xml";
	if (access("/home/W.xml", R_OK) == 0) {
        	//build the tree from the xml file
        	doc = xmlReadFile("/home/W.xml", NULL, 0);
        	if (doc == NULL ) {
           		fprintf(stderr,"Document not parsed successfully\n");
           		return -1;
        	}
	cur = xmlDocGetRootElement(doc);
        if (cur == NULL) {
            fprintf(stderr,"Empty document\n");
            xmlFreeDoc(doc);
            return -1;
        }
	if (strcmp((char *)cur->name, "wurfl") != 0) {
            fprintf(stderr,"Document of the wrong type, root node != wurfl\n");
            xmlFreeDoc(doc);
            return -1;
        }
	cur = cur->xmlChildrenNode;
        while (cur != NULL) {
            if (strcmp((char *)cur->name, "devices") == 0)
                break;
            cur = cur->next;
        }
	while(user_agent != NULL) {
		reduce_user_agent(user_agent);
		if(findDeviceNodeByUserAgent(cur, user_agent, property) == 0) {
			property->device_id = getDeviceId(property->device);
			property->device_fallback_id = getDeviceFallBackId(property->device);
			if(getDeviceDimension(cur, &property->resolution_width, &property->resolution_height) != 0) {
				property->resolution_width = 720;
				property->resolution_height = 1280;
			}
			return 0;
		}
		
	}
	return -1;
    } else {
        fprintf(stderr, "Wurfl XML repository not found at path %s. Image processing will be disabled.\n", wurfl_location);
	return -1;
	}
}
