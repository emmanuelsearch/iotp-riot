/*
 * main.c
 *
 *  Created on: Feb 6, 2018
 *      Author: Eduard Brehm
 */
#include <stdlib.h>
#include <stdio.h>
#include "eid.h"


int main (int argc, char **argv) {
	char *ikString = "e2e12c2281cdf3d350a34de4d5f56613";
	printf("%d\n", argc);
	printf("%s\n", argv[1]);
	int scaler = 0;
	int beacon_time_seconds = 2352222;
//	uint8_t ik[16] = {
//			0xe2, 0xe1, 0x2c, 0x22,
//			0x81, 0xcd, 0xf3, 0xd3,
//			0x50, 0xa3, 0x4d, 0xe4,
//			0xd5, 0xf5, 0x66, 0x13
//	};

	uint8_t eid[16] = {0};
	generateEID(ikString, scaler, beacon_time_seconds, eid);

	return 0;
}

