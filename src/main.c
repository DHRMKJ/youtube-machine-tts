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

void make_audio(CURL* hnd, size_t ind) {
	char** config = get_config();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, config[0]);

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "content-type: application/x-www-form-urlencoded");
	headers = curl_slist_append(headers, config[1]);
	headers = curl_slist_append(headers, config[2]);
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	
	char *story = strdup(stories[ind]);
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
	char audio_path[] = "output/audio.mp3";
	char *dot_pos = strchr(audio_path, '.');
	char aud_path[30];
	size_t dot = dot_pos - audio_path;
	strncpy(aud_path, audio_path, dot);
	snprintf(aud_path + dot, sizeof(aud_path) - dot, "%ld", ind);
	strncat(aud_path, dot_pos, sizeof(aud_path) - strlen(aud_path) - 1);
	printf("%s \n", aud_path);
	FILE *response_file = fopen(aud_path, "wb");
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, response_file);

	CURLcode ret = curl_easy_perform(hnd);

	if(ret != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
	}
	curl_slist_free_all(headers);
	fclose(response_file);
}

int main() {
	CURL *hnd = curl_easy_init();
	
	if(!hnd) {
		printf("[ERROR]: fatal could not intialize curl");
		return 0;
	}
	for(size_t i = 0; i < sizeof(stories)/sizeof(stories[0]); i++) {
		make_audio(hnd, i);	
	}
	curl_easy_cleanup(hnd);
	return 0;
}
