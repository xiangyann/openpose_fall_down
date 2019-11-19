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

//dirty hacks for higher than implementation
double coor_x[100][4]={[0 ... 99][0 ... 3] = 16384}, coor_y[100][4]={[0 ... 99][0 ... 3] = 16384}, coor_int[100][4]={[0 ... 99][0 ... 3] = 16384};
int lefthand[100]={[0 ... 99] = 0},righthand[100]={[0 ... 99] = 0}, hand_int[100]={[0 ... 99] = 0};
//implement if needed
//double coor_x_old[100][4]={0}, coor_y_old[100][4]={0};
int num = -1;
int num_old = 0;
char defaultfilename[16] = "_keypoints.json";
char prependfilename[6] = "json/";
char filenamestring[13] = "000000000000";
char filename[33] = "json/000000000000_keypoints.json";

/* 
gcc main.c json.c -lm
 */
static void print_depth_shift(int depth)
{
        int j;
        for (j=0; j < depth; j++) {
		//printf(" ");
        }
}
static void output();
char* body_parts(int x);
static void coorx(json_value* value, int x, int y);
static void coory(json_value* value, int x, int y);
static void spit(json_value* value, int x, int y);
static void process_value(json_value* value, int depth, int x);

static void process_object(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.object.length;
        for (x = 0; x < length; x++) {
                print_depth_shift(depth);
                //printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
                process_value(value->u.object.values[x].value, depth+1, x);
        }
}

static void process_array(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.array.length;
        //printf("array: %d\n", depth);
        for (x = 0; x < length; x++) {
                process_value(value->u.array.values[x], depth, x);
        }
}

static void process_value(json_value* value, int depth, int x)
{
        int j;
		char a[100];
        if (value == NULL) {
                return;
        }
        if (value->type != json_object) {
                print_depth_shift(depth);
        }
        switch (value->type) {
                case json_none:
//                printf("none\n");
                        break;
                case json_object:
                        process_object(value, depth+1);
                        break;
                case json_array:
                        process_array(value, depth+1);
                        break;
                case json_integer:
//                        printf("int: %10" PRId64 "\n", value->u.integer);
                        break;
                case json_double:
                        //printf("double: %f\n", value->u.dbl);
					if(x==0)num++;
					//printf("%d",x);
					if(x/3==2 || x/3==3 || x/3==5 || x/3==6){
						switch(x/3){
							case 2:spit(value, x, 0);break;
							case 3:spit(value, x, 1);break;
							case 5:spit(value, x, 2);break;
							case 6:spit(value, x, 3);break;
							default:break;
						}
					}
					//if(num > num_old){
						//printf("hooman[%d], y2=%f, y3=%f, y5=%f, y6=%f\n",num,coor_y[num][0],coor_y[num][1],coor_y[num][2],coor_y[num][3]);
						//num_old++;
					//}
					break;
                case json_string:
//                        printf("string: %s\n", value->u.string.ptr);
                        break;
                case json_boolean:
//                        printf("bool: %d\n", value->u.boolean);
                        break;
        }
}
char* body_parts(int x){
	switch(x/3){
		case 2:return "RShoulder";
		case 3:return "RElbow";
		case 5:return "LShoulder";
		case 6:return "LElbow";
		default:return "Unimplemented";
	}
}
static void coorx(json_value* value, int x, int y){
	char* part_name=body_parts(x);
	//doing YOLO
	//if(coor_x[num][y]!=0)coor_x_old[num][y]=coor_x[num][y];
	coor_x[num][y]=value->u.dbl;
	//printf("x: %d, coor_x[%d][%d] = %f\n",x/3,num,y,coor_x[num][y]);
	///printf("Person[%d] %s xcoor=%f, x=%d",num,part_name,coor_x[num][y],x);
}
static void coory(json_value* value, int x, int y){
	//char* part_name=body_parts(x);
	//YOLO
    //if(coor_y[num][y]!=0)coor_y_old[num][y]=coor_y[num][y];
    coor_y[num][y]=value->u.dbl;
	//printf("==DEBUG== Human[%d], y2=%f, y3=%f, y5=%f, y6=%f ==DEBUG==\n",num,coor_y[num][0],coor_y[num][1],coor_y[num][2],coor_y[num][3]);
	//printf("x2=%f, x3=%f, x5=%f, x6=%f\n",coor_x[num][0],coor_x[num][1],coor_x[num][2],coor_x[num][3]);
	(coor_y[num][0]>coor_y[num][1])?(righthand[num]=1):(righthand[num]=0);
	(coor_y[num][2]>coor_y[num][3])?(lefthand[num]=1):(lefthand[num]=0);
	output();
    //printf("x: %d, coor_x[%d][%d] = %f\n",x/3,num,y,coor_y[num][y]);
	//printf(", ycoor=%f, y=%d\n",coor_y[num][y],y);
}

static void output(){
	//DO SOMETHING HERE!!!!!!
	if(lefthand[num])printf("人類 %d 舉起了他的右手！\n", num);
	if(righthand[num])printf("人類 %d 舉起了他的左手！\n", num);
}

static void spit(json_value* value, int x, int y){
	switch(x%3){
		case 0:coorx(value, x, y);break;
		case 1:coory(value, x, y);break;
		case 2:
		       //confidence, used as counter
		       //printf("x: %d, This is c: %f!\n",x/3,value->u.dbl);
		       //if(y==3)num++;
			   //printf("Y: %d\n",y);
			   break;
		default:break;
	}
}


int main(int argc, char** argv){
	FILE *fp;
	struct stat filestatus;
	int file_size;
	long long int file_i = 0;
	char* file_contents;
	int pollingDelay = 100;
	json_char* json;
	json_value* value;
	
	for(file_i = 1;file_i < 999999999999; file_i++){
		sprintf(filenamestring, "%012lld", file_i);
		strcpy(filename, prependfilename);
		strcat(filename, filenamestring);
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
			
			json = (json_char*)file_contents;
			
			value = json_parse(json,file_size);
			
			if (value == NULL) {
				fprintf(stderr, "Unable to parse data\n");
				free(file_contents);
				exit(1);
			}
			
			process_value(value, 0, 0);
			
			json_value_free(value);
			free(file_contents);
			for(int l=0; l<100; l++){
				for(int n=0; n<3; n++){
					coor_y[l][n] = coor_x[l][n] = 16384;
				}
				lefthand[l] = righthand[l] = 0;
			}
			num = -1;
			num_old = 0;
			
			#ifdef _WIN32
			Sleep(pollingDelay);
			#else
			usleep(pollingDelay*1000);
			#endif
		}
	}

	return 0;
}