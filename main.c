#include <stdio.h>
#include <string.h>
#include "config.h"

//these keys are used in both programs. Client and server
const int num_keys = 2;
//careful here! the length of each string needs to be HASHLENGTH * 2
char*  keys[2] = {"bbef5eb80ede92f57137875b11dd363602068c8d699fcf75f58c8d699fcf75f5","abef5eb80ede92f57137875b11dd363602068c8d699fcf75f58c8d699fcf75f4"};

#ifdef SERVER
	#include "data_receiver.h"
	int main(void)
	{
		init_array();
		//registration
		//register client 1
		struct data client_data1;
		client_data1.id = 0;
		client_data1.index = 0;
		client_data1.depth = 0;
		client_data1.max_width = 2;
		client_data1.max_height = -1;
		client_data1.factor_of_secret_derivation = 1;
		for(int i = 0; i < HASHLENGTH; i++){
			client_data1.derived[i] = '\0';
			if(i < 22)
				client_data1.one_time_pad[i] = '\0';
		}
		from_hex_to_binary(keys[0], HASHLENGTH * 2, client_data1.root_secret);
		add_new_otp_sender_data(&client_data1);

		//register client 2
		struct data client_data2;
		client_data2.id = 1;
		client_data2.index = 0;
		client_data2.depth = 0;
		client_data2.max_width = 2;
		client_data2.max_height = -1;
		client_data2.factor_of_secret_derivation = 1;
		for(int i = 0; i < HASHLENGTH; i++){
			client_data2.derived[i] = '\0';
			if(i < 22)
				client_data2.one_time_pad[i] = '\0';
		}
		from_hex_to_binary(keys[1], HASHLENGTH * 2, client_data2.root_secret);
		add_new_otp_sender_data(&client_data2);
		//registration done

		while(1){
			char message_hex[MAX_MSG_SIZE*2];
			for(int i = 0; i < MAX_MSG_SIZE*2; i++)
				 message_hex[i] = '\0';
			printf("Enter the encrypted message: ");
			fgets(message_hex, MAX_MSG_SIZE*2, stdin);
			//remove newline at the end of input
			message_hex[strcspn(message_hex, "\n")] = 0;
			char message_encrypted[MAX_MSG_SIZE];
			for(int i = 0; i < MAX_MSG_SIZE; i++)
				 message_encrypted[i] = '\0';
			from_hex_to_binary(message_hex, strlen(message_hex), message_encrypted);
			message_encrypted[(strlen(message_hex))/2] = '\0';

			char message[MAX_MSG_SIZE + 1];
			for(int i = 0; i < MAX_MSG_SIZE+1; i++)
				 message[i] = '\0';
		  int id = 0;
			int result_code = decrypt_message(message_encrypted, (strlen(message_hex))/2 - ONETIMEPASSWORDLENGTH, message, &id);
			if(result_code == 1)
				printf("decrypted message from device %d : %s\n", id, message);
			else if(result_code == 0){
				printf("ERROR: database full\n");
				break;
			}
			else if(result_code == 2){
				printf("ERROR: current otp not allowed\n");
				break;
			}
			else if(result_code == 3){
				printf("ERROR: key not found\n");
			}
			else if(result_code == 4){
				printf("ERROR: derivation limit reached\n");
				break;
			}
			else{
				printf("ERROR: unknown error. Code: %d\n", result_code);
				break;
			}
		}
		return 0;
	}
#endif

#ifdef CLIENT
	#include "data_sender.h"
	int main(void)
	{
		struct data client_data;
		client_data.index = 0;
		client_data.depth = 0;
		client_data.max_width = 2;
		client_data.max_height = -1;
		client_data.factor_of_secret_derivation = 1;
		for(int i = 0; i < HASHLENGTH; i++){
			client_data.derived[i] = '\0';
		}

		char option[3];
		while(1){
			printf("Choose the index of the root secretyou want to use. The value must be between 0 and %d: ",num_keys-1);
			fgets(option, 3, stdin);
			int index = atoi(option);
			if(0<=index && index<num_keys){
				printf("###################################################################################\n");
				printf("#  root secret: %s  #\n", keys[index]);
				printf("###################################################################################\n\n");
				//from_binary_to_hex(client_data.root_secret, HASHLENGTH, root_secret_hex);
				from_hex_to_binary(keys[index], HASHLENGTH*2, client_data.root_secret);
				break;
			}
		}

		/*
			to automatically generate keys one could use...
			init_cipher();
			generate_key(HASHLENGTH, client_data.root_secret);
			*/
		while(1){
			char message[MAX_MSG_SIZE];
			printf("Enter your message: ");
			fgets(message, MAX_MSG_SIZE, stdin);
			//remove newline at the end of input
			message[strcspn(message, "\n")] = 0;

			char enc[MAX_MSG_SIZE + ONETIMEPASSWORDLENGTH];
			int result_code = encrypt_message(message, strlen(message), &client_data, enc);
			if(result_code == 1){
				char enc_hex[(MAX_MSG_SIZE + ONETIMEPASSWORDLENGTH)*2+1];
				from_binary_to_hex(enc, strlen(message) + ONETIMEPASSWORDLENGTH, enc_hex);
				enc_hex[(strlen(message) + ONETIMEPASSWORDLENGTH)*2] = '\0';
				printf("encrypted message: %s\n", enc_hex);
			}
			else if(result_code == 4){
				printf("ERROR: derivation limit reached\n");
				break;
			}
			else{
				printf("ERROR: unknown error. Code: %d\n", result_code);
				break;
			}
		}
		return 0;
	}
#endif
