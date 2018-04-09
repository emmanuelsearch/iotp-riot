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
#include <xtimer.h>
#include "shell.h"

#include "periph/flashpage.h"

char stack[THREAD_STACKSIZE_MAIN];
int IK_SIZE=32;
int TS_SIZE=6;
char ik[32 + 1];
char ts_str[6 + 1];

static void read_mem(void *addr, size_t len, char* output)
{
    unsigned pos = 0;
    uint8_t *mem = (uint8_t *)addr;

    for (unsigned i = 0; i < len; i++) {
      output[pos] = (char) mem[pos];
      pos++;
    }
}

int read_ik(int argc, char **argv) {
    if(argc > 1) {
      void *addr;
      int page;
      addr = flashpage_addr(atoi(argv[1]));
      uint8_t page_mem[FLASHPAGE_SIZE];
      page = flashpage_page(addr);

      flashpage_read(page, page_mem);
     // printf("Read flash page %i into local page buffer\n", page);
     // puts("Local page buffer:");
      ik[IK_SIZE] = '\0';
      read_mem(page_mem, IK_SIZE, ik);
      printf("%s\n", ik);
    }
    return 0;
}

int read_ts(int argc, char **argv) {
    if(argc > 1) {
      void *addr;
      int page;
      addr = flashpage_addr(atoi(argv[1]));
      uint8_t page_mem[FLASHPAGE_SIZE];
      page = flashpage_page(addr);

      flashpage_read(page, page_mem);
     // printf("Read flash page %i into local page buffer\n", page);
     // puts("Local page buffer:");

      ts_str[TS_SIZE] = '\0';
      read_mem(page_mem, TS_SIZE, ts_str);
      printf("%s\n", ts_str);
    }
    return 0;
}



int read_internal(int argc, char **argv) {
    if(argc > 2) {
      void *addr;
      int page;
      addr = flashpage_addr(atoi(argv[1]));
      uint8_t page_mem[FLASHPAGE_SIZE];
      page = flashpage_page(addr);

      flashpage_read(page, page_mem);
     // printf("Read flash page %i into local page buffer\n", page);
     // puts("Local page buffer:");

      int size = atoi(argv[2]);
      char out[size + 1];
      out[size] = '\0';
      read_mem(page_mem, size, out);
      printf("%s\n", out);
    }
    return 0;
}

int write_internal(int argc, char **argv) {
    if(argc > 2) {
      uint8_t page_mem[FLASHPAGE_SIZE];
      void *addr;
      void *data = argv[1];
      int page;
      addr = flashpage_addr(atoi(argv[2]));
      page = flashpage_page(addr);
      flashpage_write(page, NULL);
      printf("successfully erased page %i (addr %p)\n",
           page, flashpage_addr(page));
      memcpy(&page_mem, data, strlen(data));
      if (flashpage_write_and_verify(page, page_mem) != FLASHPAGE_OK) {
        printf("error: verification for page %i failed\n", page);
   	return 1;
      }

      printf("wrote local page to flash page %i at addr %p\n",
           page, flashpage_addr(page));
      return 0;
    }
    return 1;
}

int genEID(int argc, char **argv)
{
    (void) argc;
    if(argc > 2) {
      char *ikString = argv[1];
      int scaler = 0;
      int beacon_time_seconds = atoi(argv[2]);
      uint8_t eid[16] = {0};
      generateEID(ikString, scaler, beacon_time_seconds, eid);
      printf("Generated OTP: ");
      printHex(eid, 8);
    } else {
       printf("Too few arguments\n");
    }
    return 0;
}

static int cmd_info(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("Flash start addr:\t0x%08x\n", (int)CPU_FLASH_BASE);
    printf("Page size:\t\t%i\n", (int)FLASHPAGE_SIZE);
    printf("Number of pages:\t%i\n", (int)FLASHPAGE_NUMOF);

    return 0;
}

static int run_otp(int argc, char **argv)
{
   (void) argv;
   (void) argc;
    while(true) {
        char *readik[2] = {"read_internal", "100"};
        read_ik(2, readik);
        char *readts[3] = {"read_internal", "101"};
            read_ts(2, readts);
        int ts = atoi(ts_str);
            printf("identity key found: %s\n", ik);
            printf("counter found: %d\n", ts);
            char buffer[16 + 1];
        buffer[16] = '\0';
            sprintf(buffer, "%d", ts);
            char *gen[4] = {"genEID", ik, "0", buffer};
            genEID(4, gen);
            sprintf(buffer, "%d", ++ts);
        char *write[3] = {"write_internal", buffer, "101"};
            write_internal(3, write);
            //printf("%d", xtimer_now());
        xtimer_sleep(5);
    }
    return 0;
}


static const shell_command_t commands[] = {
        { "geneid", "Generates an EID", genEID },
        { "info", "Show information about pages", cmd_info },
        { "write_internal", "Write a string to internal flashpage", write_internal },
        { "read_internal", "Read internal flashpage", read_internal },
	    { "run_otp", "Write a string to internal flashpage", run_otp },
        { NULL, NULL, NULL }
};

int main (void) {
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    cmd_info(0, NULL);
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}

