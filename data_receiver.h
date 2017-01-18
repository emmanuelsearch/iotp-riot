#ifdef SERVER
	#ifndef DATARECEIVER_H
	#define  DATARECEIVER_H

		#include "cipher.h"
		#include "message_cipher.h"
		#include "one_time_password_manager.h"
		#include "iotp_utils.h"
		#include "storage.h"
		#include "config.h"

		int move_to_expect_next_message(struct data *client_data);
		/**
		* This method allows to insert a new OTP data so that messages encrypted with the root secret
		* can be received
		*/
		int add_new_otp_sender_data(struct data *client_data);
		/**
		* Attempts to decrypt the message using the storage provider used
		*/
		int decrypt_message(char *message, int message_length, char *result, int* id);
	#endif
#endif
