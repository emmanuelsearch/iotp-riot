/*
 * main.c
 *
 *  Created on: Feb 6, 2018
 *      Author: Eduard Brehm
 */
#include <stdlib.h>
#include <stdio.h>
#include "eid.h"

#include <string.h>
#include "shell.h"

int genEID(int argc, char **argv)
{
    (void) argc;
    if(argc > 3) {
      char *ikString = argv[1];
      int scaler = atoi(argv[2]);
      int beacon_time_seconds = atoi(argv[3]);

      uint8_t eid[16] = {0};
      generateEID(ikString, scaler, beacon_time_seconds, eid);
      printf("Generated EID: ");
      printHex(eid, 8);
    } else {
       printf("Too few arguments\n");
    }
    return 0;
}

static const shell_command_t commands[] = {
        { "geneid", "Generates an EID", genEID },
        { NULL, NULL, NULL }
};


int main (void) {
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}

