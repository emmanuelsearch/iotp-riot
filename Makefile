PART ?= client
# name of your application
APPLICATION = iotp-riot-$(PART)

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#CFLAGS += -DDEVELHELP
ifeq ($(PART),client)
	CFLAGS += -DCLIENT
else ifeq ($(PART),server)
	CFLAGS += -DSERVER
endif


# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += hashes

include $(RIOTBASE)/Makefile.include
