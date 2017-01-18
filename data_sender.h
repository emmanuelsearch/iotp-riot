#ifdef CLIENT
	#ifndef DATASENDER_H
	#define  DATASENDER_H
		#include "cipher.h"
		#include "one_time_password_manager.h"
		#include "message_cipher.h"
		#include "iotp_utils.h"
		#include "config.h"

		/**
		* Attempts to encrypt the message using the current SenderOTPData
		*/
		int encrypt_message(char *message, int message_length, struct data *client_data, char* result);
	#endif
#endif
