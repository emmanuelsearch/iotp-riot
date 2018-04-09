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

static void read_mem(void *addr, size_t len, char* output)
{
    unsigned pos = 0;
    uint8_t *mem = (uint8_t *)addr;

    for (unsigned i = 0; i < len; i++) {
      output[pos] = (char) mem[pos];
      pos++;
    }
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

static const shell_command_t commands[] = {
        { "geneid", "Generates an EID", genEID },
        { "info", "Show information about pages", cmd_info },
        { "write_internal", "Write a string to internal flashpage", write_internal },
        { "read_internal", "Read internal flashpage", read_internal },
        { NULL, NULL, NULL }
};

int main (void) {
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    cmd_info(0, NULL);
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}

