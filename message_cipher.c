#include "message_cipher.h"

void apply_otp(char *message, int message_length, struct data *client_data, int otp_length, char *result){
	char pad[HASHLENGTH]; //can be shorter than 32
	char chunk[HASHLENGTH]; //can be shorter than 32
	memcpy(pad, client_data->one_time_pad, otp_length);
	int i = otp_length;
	int chunk_length;
	if(otp_length < message_length)
		chunk_length = otp_length;
	else
		chunk_length = message_length;
	memcpy(chunk, message, chunk_length); //copy message part as long as pad for xor
	xor_arrays(chunk, pad, chunk_length, chunk);
	memcpy(result, chunk, chunk_length);

	int first = 1;
	char new_pad[HASHLENGTH];
	int tmp_otp_length = 0;
	while(i < message_length){
		if(first == 1){
			tmp_otp_length = otp_length;
		}
		else{
			tmp_otp_length = HASHLENGTH;
			get_next_otp(client_data, new_pad);
		}
		int new_chunk_length = 0;
		if(tmp_otp_length < message_length - i)
			new_chunk_length = tmp_otp_length;
		else{
			new_chunk_length = message_length - i;
		}
		char new_chunk[HASHLENGTH]; //can be only 22 long or shorter
		memcpy(new_chunk, message + i, new_chunk_length);
		if(first == 1){
			xor_arrays(new_chunk, pad, new_chunk_length, new_chunk);
			first = 0;
		}
		else
			xor_arrays(new_chunk, new_pad, new_chunk_length, new_chunk);
		memcpy(result + i, new_chunk, new_chunk_length);
		i = i + tmp_otp_length;
	}
}
#ifdef CLIENT
void mc_encrypt(char *message, int message_length, struct data *client_data, int otp_length, char *result){
	apply_otp(message, message_length, client_data, otp_length, result);
}
#endif
#ifdef SERVER
void mc_decrypt(char *message, int message_length, struct data *client_data, int otp_length, char *result){
	apply_otp(message, message_length, client_data, otp_length, result);
}
#endif
