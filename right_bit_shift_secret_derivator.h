#ifndef RBSSD_H
#define RBSSD_H

#include <stdio.h>
#include <hashes/sha256.h>
#include <string.h>
#include "config.h"

void shift_bits_right(int j, char *bytes, int length);
void do_sha256(char *root_secret, char *result);

void derive_secret(int j, char *root_secret, char *result);
void down_level(char *root_secret, char *result);
void down_level_leaf(char *root_secret, char *result);
//due to the properties of the bitshift and the implementation nature of this class you can derive directly from the preivous OTP :)
int allows_direct_secret_derivation(void);
#endif
