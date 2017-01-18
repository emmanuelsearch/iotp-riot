#ifdef SERVER
	#ifndef STORAGE_H
	#define STORAGE_H

	#include <stdio.h>
	#include "data_struct.h"
	#include "iotp_utils.h"
	#include "data_receiver.h"
	#include "config.h"

	void init_array(void);
	struct data read_data_from_key(char *key);
	int store_object(struct data *d);
	int delete_object_from_key(char *key);
	void print_storage(void);
	#endif
#endif
