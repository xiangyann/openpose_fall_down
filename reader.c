#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "json.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char** argv){
	char defaultfilename[16] = "_keypoints.json";
	char filenamestring[13] = "000000000000";
	char filename[28] = "000000000000_keypoints.json";
	FILE *fp;
	struct stat filestatus;
	int file_size;
	long long int file_i = 0;
	char* file_contents;
	int pollingDelay = 1;
	json_char* json;
	json_value* value;
	
	for(file_i = 1;file_i < 999999999999; file_i++){
		sprintf(filenamestring, "%012lld", file_i);
		strcpy(filename, filenamestring);
		strcat(filename, defaultfilename);
		//if file not found
		if ( stat(filename, &filestatus) != 0) {
			printf("Filename: %s\n", filename);
			file_i--;
			#ifdef _WIN32
			Sleep(pollingDelay);
			#else
			usleep(pollingDelay*1000);
			#endif
		}
		else{
			//if OOM
			file_size = filestatus.st_size;
			file_contents = (char*)malloc(filestatus.st_size);
			if ( file_contents == NULL) {
				fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
				return 1;
			}
			
			//if no read permission
			fp = fopen(filename, "rt");
			if (fp == NULL) {
				fprintf(stderr, "Unable to open %s\n", filename);
				fclose(fp);
				free(file_contents);
				return 1;
			}
			//if unable to read for some reason
			if ( fread(file_contents, file_size, 1, fp) != 1 ) {
				fprintf(stderr, "Unable to read content of %s\n", filename);
				fclose(fp);
				free(file_contents);
				return 1;
			}
			fclose(fp);
			
			printf("%s\n", file_contents);
			
			//json = (json_char*)file_contents;
			
			/*value = json_parse(json,file_size);
			
			if (value == NULL) {
				fprintf(stderr, "Unable to parse data\n");
				free(file_contents);
				exit(1);
			}
			
			process_value(value, 0, 0);
			
			json_value_free(value);*/
			free(file_contents);
		}
	}

	return 0;
}