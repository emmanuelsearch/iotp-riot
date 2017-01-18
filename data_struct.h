#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H
#include "config.h"

struct data{
	char key[ONETIMEPASSWORDLENGTH*2+1];
	int index;
	int depth;
	int max_width;
	int max_height;
	int factor_of_secret_derivation;
	char one_time_pad[HASHLENGTH-ONETIMEPASSWORDLENGTH];
	char root_secret[HASHLENGTH];
	char derived[HASHLENGTH];
	int in_use;
	#ifdef SERVER
	int id;
	#endif
};

#endif
