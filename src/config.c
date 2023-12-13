#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void load_env(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256]; 
    while (fgets(line, sizeof(line), file) != NULL) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");

        if (key != NULL && value != NULL) {
            setenv(key, value, 1);
        }
    }

    fclose(file);
}

char **get_config() {
   load_env(".env");
   const char *URL = getenv("URL");
   const char *RAPID_API_KEY = getenv("RAPID_API_KEY");
   const char *RAPID_API_HOST = getenv("RAPID_API_HOST");

   char **config = (char **)malloc(3 * sizeof(char*));
   if(config == NULL) {
   	perror("[ERROR]: FATAL Mem allocation Error");
	exit(EXIT_FAILURE);
   }

     for (size_t i = 0; i < 3; i++) {
        config[i] = (char *)malloc(100 * sizeof(char)); 
        if (config[i] == NULL) {		
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    strcpy(config[0], URL);
    strcpy(config[1], RAPID_API_KEY);
    strcpy(config[2], RAPID_API_HOST);

    return config;
}

void free_config(char **config) {
	for(size_t i = 0; i < 3; i++) {
		free(config[i]);
	}
	free(config);
}
