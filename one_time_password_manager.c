#include "one_time_password_manager.h"
#include "iotp_utils.h"

int can_go_one_level_down(struct data *client_data){
	if(client_data->depth > client_data->max_height &&  client_data->max_height != -1)
		return 0;
	return 1;
}

int is_current_otp_allowed(struct data *client_data){
	if(client_data->depth > client_data->max_height &&  client_data->max_height != -1)
		return 0;
	if(client_data->index > client_data->max_width)
		return 0;
	return 1;
}

int get_next_otp(struct data *client_data, char *result){
	//derived_secret is empty -> use root_secret
	if(client_data->derived[0] == '\0') {
		memcpy(client_data->derived, client_data->root_secret, HASHLENGTH);
	}
	if(client_data->index == client_data->max_width) {
		if(can_go_one_level_down(client_data) == 1) {
			client_data->index = 0;
			client_data->depth = client_data->depth + 1;
			derive_secret(client_data->factor_of_secret_derivation, client_data->derived, client_data->derived);
			down_level(client_data->derived, client_data->root_secret);
			memcpy(client_data->derived, client_data->root_secret, HASHLENGTH);
		}
		else {
			return 4; //derivation limit reached
		}
	}
	if(client_data->index <client_data->max_width) {
		client_data->index = client_data->index + 1;
		derive_secret(client_data->factor_of_secret_derivation, client_data->derived, client_data->derived);
	}
	down_level_leaf(client_data->derived, result);//to generate the hash that will be sent
	return 1;
}
