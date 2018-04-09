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
#include <xtimer.h>
#include <thread.h>
#include "periph/flashpage.h"

char stack[THREAD_STACKSIZE_MAIN];

static void read_mem(void *addr, size_t len, char* output)
{
    unsigned pos = 0;
    uint8_t *mem = (uint8_t *)addr;

    for (unsigned i = 0; i < len; i++) {
      output[pos] = (char) mem[pos];
      pos++;
    }
}

char* read_internal(int address, int size, char* out) {
      void *addr;
      int page;
      addr = flashpage_addr(address);
      uint8_t page_mem[FLASHPAGE_SIZE];
      page = flashpage_page(addr);

      flashpage_read(page, page_mem);
      printf("Read flash page %i into local page buffer\n", page);
      puts("Local page buffer:");
      out[size] = '\0';
      read_mem(page_mem, size, out);
      printf("%s\n", out);

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

static int genEID(int argc, char **argv)
{
 //   printf("%d \n", pow(2, 4));
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
//void *run_otp(void *argc)
{
   (void) argv;
   (void) argc;
    while(true) {
        char ik[16 + 1];
	read_internal(100, 16*2, ik);
	char ts_str[6 + 1];
        read_internal(101, 6, ts_str);
	int ts = atoi(ts_str);
        printf("identity key found: %s\n", ik);
        printf("counter found: %d\n", ts);
        char buffer[16];
        sprintf(buffer, "%d", ts++);
        char *gen[4] = {"genEID", ik, "0", buffer};
        genEID(4, gen); 
        sprintf(buffer, "%d", ts);
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
        { "run_otp", "Write a string to internal flashpage", run_otp },
//        { "read_internal", "Read internal flashpage", read_internal },
        { NULL, NULL, NULL }
};

int main (void) {
    char *write_ik[3] = {"write_internal", "e2e12c2281cdf3d350a34de4d5f56613", "100"};
    write_internal(3, write_ik);
    char *write_ts[3] = {"write_internal", "100000", "101"};
    write_internal(3, write_ts);
//    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, run_otp, NULL, "otp_thread");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    cmd_info(0, NULL);
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}

