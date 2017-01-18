#ifndef CONFIG
	#define CONFIG
	//in this file are all necessary configuration-values for client and server
	
	//length of the root_secret and any produced hash
	#define HASHLENGTH 32
	//length of the pad used for identification of the client
	#define ONETIMEPASSWORDLENGTH 10
	//maximum length of a message to be encrypted
	#define MAX_MSG_SIZE 1000
	#ifdef SERVER
		//number of clients that can be stored in the server's database
		#define MAX_CLIENTS 5
		//number of keys that will be computed to find a missing key (how many messages can get lost before server and client are completely out of sync)
		#define FUTUREKEYS 3
		
	#endif //SERVER
#endif //CONFIG

