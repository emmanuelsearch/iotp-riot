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
	(void) argc;
	(void) argv;

	char *ikString = "e2e12c2281cdf3d350a34de4d5f56613";

	int scaler = 0;
	int beacon_time_seconds = 2352222;

	uint8_t eid[16] = {0};
	generateEID(ikString, scaler, beacon_time_seconds, eid);
	printf("Generated EID: ");
	printHex(eid, 8);
	return 0;
}

