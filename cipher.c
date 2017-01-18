#include "cipher.h"

void xor_arrays(const char *in, const char *key, int length_in, char *out){
	for(int i = 0; i < length_in; i++) {
		out[i] = in[i] ^ key[i];
	}
}

#ifdef CLIENT
void init_cipher(void){
	srand ((unsigned int) time (NULL));
}

void encrypt(const char *in, const char *key, int length_in, char *out){
	xor_arrays(in, key, length_in, out);
}

void generate_key(int length, char *out){
	for(int i = 0; i < length; i++) {
		out[i] = rand();
	}
}
#endif

#ifdef SERVER
void decrypt(const char *in, const char *key, int length_in, char *out){
	xor_arrays(in, key, length_in, out);
}
#endif

