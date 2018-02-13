/*
 * eid.c
 *
 *  Created on: Feb 6, 2018
 *      Author: Eduard Brehm
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "include/crypto/ciphers.h"
#include "crypto/ciphers.c"
#include "crypto/aes.c"
#include "crypto/modes/ecb.c"
#include "eid.h"

int howToUse(void) {
	printf(
"Usage:\n\
./eid <ik> <scaler> <beacon_time_seconds>\n\
\n\
Generates an EID from the given 16-bytes identity key (shared secret),\n\
the scaler (frame for same EID)\n\
and the beacon time in seconds.\n\
\n\
Binary arguments are interpreted depending on their first byte:\n\
a: the rest of the string is interpreted as ASCII;\n\
h: the rest of the string is interpreted as bytes in hex;\n\
b: the rest of the string is interpreted as base64.\n\
d: the rest of the string is interpreted as decimal (in little endian).\n\
Integer inputs can be written as decimal, or as hex as in 0x1234.");
	return 0;
}

int modulus(double a, double b) {
	int result = (int) ( a / b );
	return a - ( result ) * b;
}

void printHex(uint8_t *hex, int offset) {
	for(int i = 0; i < AES_BLOCK_SIZE; ++i) {
		if(offset > 0 && i >= offset) {
			break;
		} else {
			printf("%02x ", hex[i]);
		}
	};
	printf("\n");
}

uint8_t *parseIk(char *ikString) {
	uint8_t ik[16] = {0};
	char subString[3];
	subString[3] = '\0';
	int j = 0;
	for(int i = 0; i < 16*2; i+=2) {
		memcpy(subString, &ikString[i], 2);
		unsigned int intVal;
		if (sscanf (subString, "%xu", &intVal)!=1) {
		    fprintf(stderr, "error - not a hex integer");
		}
		ik[j] = intVal;
		++j;
	}

	uint8_t *result = malloc(sizeof(ik));
	if(result != 0) {
		memmove(result, ik, sizeof(ik));
	}

	return result;
}

int generateEID(char *ikString, int scaler, int beacon_time_seconds, uint8_t *eid) {
	uint8_t *ik = parseIk(ikString);
	printf("Init Key Data: ");
	//od_hex_dump(ik, AES_BLOCK_SIZE, 0);
	printHex(ik, 0);
	uint8_t tkdata[AES_BLOCK_SIZE] = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //"\x00" * 11
			0xff,	//"\xFF"
			0x00, 0x00, //"\x00" * 2
			modulus((double) (beacon_time_seconds / (pow(2, 24))), 256), //chr((beacon_time_seconds / (2 ** 24)) % 256)
			modulus((double) (beacon_time_seconds / (pow(2, 16))), 256) //chr((beacon_time_seconds / (2 ** 16)) % 256)
	};
	printf("Temporary Key Data: ");
	//od_hex_dump(tkdata, AES_BLOCK_SIZE, 0);
	printHex(tkdata, 0);
	cipher_t tkdataCipher;
	uint8_t tk[AES_KEY_SIZE] = {0};
	cipher_init(&tkdataCipher, CIPHER_AES_128, ik, AES_KEY_SIZE);
	cipher_encrypt(&tkdataCipher, tkdata, tk);
	printf("Temporary Key: ");
	//od_hex_dump(tk, AES_BLOCK_SIZE, 0);
	printHex(tk, 0);
	beacon_time_seconds = (beacon_time_seconds / pow(2, 0)) * pow(2, 0);
	uint8_t eiddata[AES_BLOCK_SIZE] = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //"\x00" * 11
			scaler,
			modulus((double) (beacon_time_seconds / (pow(2, 24))), 256),
			modulus((double) (beacon_time_seconds / (pow(2, 16))), 256),
			modulus((double) (beacon_time_seconds / (pow(2, 8))), 256),
			modulus((double) (beacon_time_seconds / (pow(2, 0))), 256)
	};
	printf("Ephermal Id Data: ");
	//od_hex_dump(eiddata, AES_BLOCK_SIZE, 0);
	printHex(eiddata, 0);
	cipher_t tkEidCipher;
	
	cipher_init(&tkEidCipher, CIPHER_AES_128, tk, AES_KEY_SIZE);
	cipher_encrypt(&tkEidCipher, eiddata, eid);
	printf("Ephermal Id: ");
	//od_hex_dump(eid, AES_BLOCK_SIZE, 0);
	printHex(eid, 8);
	return 0;
}















