#include<stdio.h>
#include<curl/curl.h>

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

	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "src=What's up Palak and Dharmik Seriously guys!, you weirdos!&hl=en-us&r=0&c=mp3&f=8khz_8bit_mono");

	free_config(config);
	FILE *response_file = fopen("output/response.mp3", "wb");
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
