APPLICATION = eid
RIOT_DIR = RIOT
BOARD ?= native
RIOTBASE ?= $(CURDIR)/$(RIOT_DIR)/
QUIET ?= 1

CFLAGS += -DDEVELHELP
CFLAGS += -I$(RIOT_DIR)/sys
CFLAGS += -DCRYPTO_AES

include $(RIOTBASE)/Makefile.include

.PHONY: clean

clean:
	$(RM)
