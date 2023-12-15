#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<curl/curl.h>
#include "stories.h"

char **get_config();
void free_config(char** config_param);

size_t WriteCallback(void *contents, size_t size, size_t nmemb, FILE *file) {
	size_t realsize = size * nmemb;
    	fwrite(contents, size, nmemb, file);
    	return realsize;
}

int main() {
	CURL *hnd = curl_easy_init();
	
	if(!hnd) {
		printf("[ERROR]: fatal could not intialize curl");
		return 0;
	}
	
	char** config = get_config();
	
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, config[0]);

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "content-type: application/x-www-form-urlencoded");
	headers = curl_slist_append(headers, config[1]);
	headers = curl_slist_append(headers, config[2]);
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	
	char *story = strdup(stories[0]);
	char query[] = "&hl=en-us&v=Mary&r=0&c=mp3&f=ulaw_44khz_stereo";
	char *params = (char*)malloc(5 + strlen(story) + strlen(query));
	char src[] = "src=";
	strcpy(params, src);
	strcat(params, story);
	strcat(params, query);
	char real_params[strlen(params)];
	strcpy(real_params, params);
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, real_params);
	free(params);
	free_config(config);
	FILE *response_file = fopen("output/aud.mp3", "wb");
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, response_file);

	CURLcode ret = curl_easy_perform(hnd);
	
	/*if(CURLE_OK == ret) {
		char *ct; // content type
		
		ret = curl_easy_getinfo(hnd, CURLINFO_CONTENT_TYPE, &ct);

		if((CURLE_OK == ret) && ct)
			printf("We received Content-Type: %s\n", ct);
	}*/

	if(ret != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
	}
	curl_slist_free_all(headers);
	curl_easy_cleanup(hnd);
	fclose(response_file);
	return 0;
}
