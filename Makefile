APPLICATION = eid
RIOT_DIR = RIOT
#BOARD ?= native
BOARD ?=samr21-xpro
RIOTBASE ?= $(CURDIR)/$(RIOT_DIR)/
QUIET ?= 1

CFLAGS += -DDEVELHELP
CFLAGS += -I$(RIOT_DIR)/sys
CFLAGS += -DCRYPTO_AES

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps


include $(RIOTBASE)/Makefile.include

.PHONY: clean

clean:
	$(RM)
