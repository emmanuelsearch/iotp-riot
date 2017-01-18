#ifndef IOTPUTILS_H_
#define IOTPUTILS_H_

#include <stdlib.h>
#include <string.h>
#include "config.h"

void from_hex_to_binary(const char *in, int length_in, char *out);
void from_binary_to_hex(const char *in, int length_in, char *out);
void get_first_half_otp_as_bytes(const char *otp, int length, char *result);
void get_second_half_otp_as_bytes(const char *otp, int otp_length, char *result);
#endif


