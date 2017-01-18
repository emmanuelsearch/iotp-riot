//TODO ignore file would be better
extern int whatever;
#ifdef CLIENT
#include "data_sender.h"
int encrypt_message(char *message, int message_length, struct data *client_data, char* result)
{
	char otp[HASHLENGTH];
	int result_code = get_next_otp(client_data, otp);
	if(result_code != 1)
		return result_code;
	char id[ONETIMEPASSWORDLENGTH];
	char pad[HASHLENGTH - ONETIMEPASSWORDLENGTH];
	char enc_message[MAX_MSG_SIZE+ONETIMEPASSWORDLENGTH];
	get_first_half_otp_as_bytes(otp, ONETIMEPASSWORDLENGTH, id);
	get_second_half_otp_as_bytes(otp, HASHLENGTH, pad);
	memcpy(client_data->one_time_pad, pad, HASHLENGTH-ONETIMEPASSWORDLENGTH);
	mc_encrypt(message, message_length, client_data, HASHLENGTH - ONETIMEPASSWORDLENGTH, enc_message);
	memcpy(result, id, ONETIMEPASSWORDLENGTH);
	memcpy(result + ONETIMEPASSWORDLENGTH, enc_message, message_length);
	return 1;
}
#endif
