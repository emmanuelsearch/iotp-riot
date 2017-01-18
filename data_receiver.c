//TODO ignore file would be better
extern int whatever;
#ifdef SERVER
#include "data_receiver.h"
#include "config.h"

int move_to_expect_next_message(struct data *client_data){
	char otp[HASHLENGTH];
	int result_code = get_next_otp(client_data, otp);
	if(result_code != 1)
		return result_code;
	char k[ONETIMEPASSWORDLENGTH];
	get_first_half_otp_as_bytes(otp, ONETIMEPASSWORDLENGTH, k);
	char rest_pad[HASHLENGTH - ONETIMEPASSWORDLENGTH];
	get_second_half_otp_as_bytes(otp, HASHLENGTH, rest_pad);
	memcpy(client_data->one_time_pad, rest_pad, HASHLENGTH - ONETIMEPASSWORDLENGTH);
	char key[ONETIMEPASSWORDLENGTH * 2 + 1];
	from_binary_to_hex(k, ONETIMEPASSWORDLENGTH, key);
	key[ONETIMEPASSWORDLENGTH * 2] = '\0';
	strcpy(client_data->key, key);
	result_code = store_object(client_data);
	return result_code;
}

int add_new_otp_sender_data(struct data *client_data){
	return move_to_expect_next_message(client_data);
}

int decrypt_message(char *message, int message_length, char *result, int* id){
	char k[ONETIMEPASSWORDLENGTH];
	get_first_half_otp_as_bytes(message, ONETIMEPASSWORDLENGTH, k);
	char k_hex[ONETIMEPASSWORDLENGTH * 2 + 1];
	from_binary_to_hex(k, ONETIMEPASSWORDLENGTH, k_hex);
	k_hex[ONETIMEPASSWORDLENGTH * 2] = '\0';

	struct data d = read_data_from_key(k_hex);
	if(d.in_use == 1){
		(*id)= d.id;
		char enc_message[MAX_MSG_SIZE*2];
		for(int i = 0; i < MAX_MSG_SIZE*2; i++)
			 enc_message[i] = '\0';
		get_second_half_otp_as_bytes(message, message_length + ONETIMEPASSWORDLENGTH, enc_message);

		int result_code = 0;
		if(is_current_otp_allowed(&d) == 1){
			mc_decrypt(enc_message, message_length, &d, HASHLENGTH - ONETIMEPASSWORDLENGTH, result);
			delete_object_from_key(k_hex);
			result_code = move_to_expect_next_message(&d);
		}
		else
			result_code = 2; //derivation limit reached
		return result_code;
	}
	else
		return 3; //no key found
}
#endif
