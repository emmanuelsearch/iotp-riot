#include "iotp_utils.h"

void from_hex_to_binary(const char *in, int length_in, char* out){
	int j = 0;
	int i;
	for(i = 0; i < length_in; i += 2) {
		const char buf[5] = {'0', 'x', in[i], in[i+1], 0};
		out[j] = strtol(buf, NULL, 0); //TODO get rid of this expensive function, also gets rid of cstdlib
		j++;
	}
}

void from_binary_to_hex(const char *in, int length_in, char* out){
	const char * hex = "0123456789abcdef";
	int j = 0;
	int i;
	for(i = 0; i < (length_in); i++) {
		out[j] = hex[(in[i]>>4) & 0xf];
		out[j+1] = hex[ in[i]   & 0xf];
		j += 2;
	}
}

void get_first_half_otp_as_bytes(const char *otp, int length, char *result){
	memcpy(result, otp, length);
}

void get_second_half_otp_as_bytes(const char *otp, int otp_length, char *result){
	memcpy(result, otp + ONETIMEPASSWORDLENGTH, otp_length - ONETIMEPASSWORDLENGTH);
}
