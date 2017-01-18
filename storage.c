//TODO ignore file would be better
extern const int HASHLENGTH;
#ifdef SERVER
#include <string.h>
#include "storage.h"

struct data data_array[MAX_CLIENTS];

void init_array(void){
	for(int i = 0; i < MAX_CLIENTS; i++){
		data_array[i].in_use = 0;
		data_array[i].index = 0;
		data_array[i].depth = 0;
		data_array[i].max_width = 0;
		data_array[i].max_height = 0;
		data_array[i].factor_of_secret_derivation = 0;
		for(int j = 0; j < HASHLENGTH; j++){
			data_array[i].root_secret[j] = '\0';
			data_array[i].derived[j] = '\0';
			if(j < 22)
				data_array[i].one_time_pad[j] = '\0';
		}
	}
}

int get_index(char *key){
	for(int i = 0; i < MAX_CLIENTS; i++){
		if(data_array[i].in_use != 0 && !strcmp(key, data_array[i].key)) {
			return i;
		}
	}
	return -1;
}

int get_next_free_slot(void){
	for(int i = 0; i < MAX_CLIENTS; i++){
		if(data_array[i].in_use == 0) {
			return i;
		}
	}
	return -1;
}

struct data search_future_keys(char *key){
	//go trough all stored clients
	for(int i = 0; i < MAX_CLIENTS; i++){
		//if there is a client stored
		if(data_array[i].in_use == 1){
			//temporally store current state of the client
			char init_key[ONETIMEPASSWORDLENGTH*2+1];
			memcpy(init_key, data_array[i].key, ONETIMEPASSWORDLENGTH*2+1);
			char init_secret[HASHLENGTH];
			memcpy(init_secret, data_array[i].root_secret, HASHLENGTH);
			char init_derived[HASHLENGTH];
			memcpy(init_derived, data_array[i].derived, HASHLENGTH);
			char init_otp[HASHLENGTH-ONETIMEPASSWORDLENGTH];
			memcpy(init_otp, data_array[i].one_time_pad, HASHLENGTH-ONETIMEPASSWORDLENGTH);
			int init_index = data_array[i].index;
			int init_depth = data_array[i].depth;
			//calculate the next FUTUREKEYS states for that client
			for(int j = 0; j < FUTUREKEYS; j++){
				move_to_expect_next_message(&data_array[i]);
				int index = get_index(key);
				//if key found -> keep that state
				if(index != -1)
					return data_array[index];
			}
			//no key found in the future states -> reset data to init state
			data_array[i].index = init_index;
			data_array[i].depth = init_depth;
			memcpy(data_array[i].key, init_key, ONETIMEPASSWORDLENGTH*2+1);
			memcpy(data_array[i].root_secret, init_secret, HASHLENGTH);
			memcpy(data_array[i].derived, init_derived, HASHLENGTH);
			memcpy(data_array[i].one_time_pad, init_otp, HASHLENGTH-ONETIMEPASSWORDLENGTH);
		}
	}
	//no key found in the future states of any stored client
	struct data null;
	null.in_use = 0;
	return null;
}

struct data read_data_from_key(char *key){
	int index = get_index(key);
	if(index != -1)
		return data_array[index];
	else{
		//check if key is found in the FUTUREKEYS next states
		return search_future_keys(key);
		/*
		struct data null;
		null.in_use = 0;
		return null;
		*/
	}
}

int store_object(struct data *d){
	int index = get_index(d->key);
	if(index == -1){
		index = get_next_free_slot();
		if(index == -1){
			return 0;
		}
	}
	memmove(data_array[index].key, d->key, strlen(d->key)+1);
	data_array[index].id = d->id;
	data_array[index].index = d->index;
	data_array[index].depth = d->depth;
	data_array[index].max_width = d->max_width;
	data_array[index].max_height = d->max_height;
	data_array[index].factor_of_secret_derivation = d->factor_of_secret_derivation;
	memmove(data_array[index].root_secret, d->root_secret, HASHLENGTH);
	memmove(data_array[index].derived, d->derived, HASHLENGTH);
	memmove(data_array[index].one_time_pad, d->one_time_pad, HASHLENGTH-ONETIMEPASSWORDLENGTH);
	data_array[index].in_use = 1;
	return 1;

}

int delete_object_from_key(char *key){
	int index = get_index(key);
	if(index != -1){
		data_array[index].in_use = 0;
		return 1;
	}
	else {
		return 0;
	}
}

//for debugging
void print_storage(void){
	for(int i = 0; i < MAX_CLIENTS; i++){
		if(data_array[i].in_use == 1){
			char key_hex[HASHLENGTH*2+1];
			from_binary_to_hex(data_array[i].root_secret, HASHLENGTH, key_hex);
			key_hex[HASHLENGTH*2] = '\0';

			char otp_hex[(HASHLENGTH-ONETIMEPASSWORDLENGTH)*2+1];
			from_binary_to_hex(data_array[i].one_time_pad, HASHLENGTH-ONETIMEPASSWORDLENGTH, otp_hex);
			otp_hex[(HASHLENGTH-ONETIMEPASSWORDLENGTH)*2] = '\0';

			printf("%d %s %d %d %d %d %d %s %s\n",  data_array[i].in_use, data_array[i].key, data_array[i].index, data_array[i].depth, data_array[i].max_width, data_array[i].max_height, data_array[i].factor_of_secret_derivation, key_hex, otp_hex);
		}
		else
			printf("empty\n");
	}
}
#endif
