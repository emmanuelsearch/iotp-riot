#include "right_bit_shift_secret_derivator.h"

void shift_bits_right(int j, char *bytes, int length) {
	int right_shifts = j;
	int left_shifts = 8 - right_shifts;
	char previous_byte = bytes[0]; // keep the byte before modification
	bytes[0] = (char) (((bytes[0] & 0xff) >> right_shifts) | ((bytes[length - 1] & 0xff) << left_shifts));
	for (int i = 1; i < length; i++) {
		char tmp = bytes[i];
		bytes[i] = (char) (((bytes[i] & 0xff) >> right_shifts) | ((previous_byte & 0xff) << left_shifts));
		previous_byte = tmp;
	}
}

void do_sha256(char *hash, char *result) {
	unsigned char secret[HASHLENGTH];
	memcpy(secret, hash, HASHLENGTH);
	unsigned char shaRes[HASHLENGTH];
	sha256(secret, HASHLENGTH, shaRes);
	memcpy(result, shaRes, HASHLENGTH);
}

void derive_secret(int j, char *hash, char *result) {
	memmove(result, hash, HASHLENGTH);
	int shifts = 0;
	while(j > 0) {
		if(j % 8 != 0)
			shifts = j % 8;
		else
			shifts = 8;
		shift_bits_right(shifts, result, HASHLENGTH);
		j -= shifts;
	}
}

void down_level(char *hash, char *result){
	do_sha256(hash, result);
}

void down_level_leaf(char *hash, char *result){
	do_sha256(hash, result);
}

//due to the properties of the bitshift and the implementation nature of this class you can derive directly from the preivous OTP :)
int allows_direct_secret_derivation(void){
	return 1;
}
