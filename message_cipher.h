#ifndef MESSAGE_CIPHER_H
#define MESSAGE_CIPHER_H
	#include "cipher.h"
	#include "one_time_password_manager.h"
	#include "config.h"
	
	void apply_otp(char *message, int message_length, struct data *client_data, int otp_length, char *result);
	/**
	* This method encrypts a message using the OTPs regardless of its size
	*/
	#ifdef CLIENT
	void mc_encrypt(char *message, int message_length, struct data *client_data, int otp_length, char *result);
	#endif
	#ifdef SERVER
	void mc_decrypt(char *message, int message_length, struct data *client_data, int otp_length, char *result);
	#endif
#endif
