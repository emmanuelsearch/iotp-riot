#ifndef CIPHER_H
#define CIPHER_H

	#include <stdlib.h>
	#include <time.h>

	void xor_arrays(const char *in, const char *key, int length_in, char *out);
	#ifdef CLIENT
	void init_cipher(void);
	void encrypt(const char *in, const char *key, int length_in, char *out);
	void generate_key(int length, char *out);
	#endif
	#ifdef SERVER
	void decrypt(const char *in, const char *key, int length_in, char *out);
	#endif
#endif

