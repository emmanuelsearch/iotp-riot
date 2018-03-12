APPLICATION = eid
RIOT_DIR = RIOT
#BOARD ?= native
BOARD ?=samr21-xpro
RIOTBASE ?= $(CURDIR)/$(RIOT_DIR)/
QUIET ?= 1

CFLAGS += -DDEVELHELP
CFLAGS += -I$(RIOT_DIR)/sys
CFLAGS += -DCRYPTO_AES
CFLAGS += -Wno-unused-variable

FEATURES_REQUIRED += periph_flashpage
FEATURES_OPTIONAL += periph_flashpage_raw

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += vfs
#USEMODULE += mtd

#USEMODULE += littlefs
#USEMODULE += constfs

ifneq (,$(filter littlefs, $(USEMODULE)))
  CFLAGS += -DVFS_FILE_BUFFER_SIZE=52 -DVFS_DIR_BUFFER_SIZE=44
else ifneq (,$(filter spiffs, $(USEMODULE)))
  SPIFFS_NB_FD ?= 8
  CFLAGS += '-DSPIFFS_FS_FD_SPACE_SIZE=(32 * $(SPIFFS_NB_FD))'
endif

include $(RIOTBASE)/Makefile.include

.PHONY: clean

clean:
	$(RM)
