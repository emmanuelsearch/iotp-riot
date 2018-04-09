# OTP for RIOT OS

Clone the repository and change directory

    git clone https://github.com/Agile-IoT/iotp-riot.git && cd iotp-riot

In the Makefile, change the device you want to run the code on, according to [RIOT OS Tutorial](https://github.com/RIOT-OS/Tutorials/tree/master/task-01).

    #BOARD ?= native
    BOARD ?=samr21-xpro
    
Flash the board

    sudo make all term flash

## Generate OTP

Create an OTP with the following pattern

    geneid IK TIMESTAMP

IK = initialization key, TIMESTAMP = the timestamp for which the OTP should be generated.

For example:

    geneid e2e12c2281cdf3d350a34de4d5f56613 100000
    
The above example outputs

    INFO # Init Key Data: e2 e1 2c 22 81 cd f3 d3 50 a3 4d e4 d5 f5 66 13 
    INFO # Temporary Key Data: 00 00 00 00 00 00 00 00 00 00 00 ff 00 00 00 01 
    INFO # Temporary Key: fe a6 5d ed 44 a4 4e 36 e7 01 d8 33 88 28 63 22 
    INFO # Ephermal Id Data: 00 00 00 00 00 00 00 00 00 00 00 00 00 01 86 a0 
    INFO # Ephermal Id: e7 25 72 e3 84 ba 63 6b f5 01 83 ae 06 c7 c5 8f 
    INFO # Generated OTP: e7 25 72 e3 84 ba 63 6b

    
## Write to internal storage

Write a string to the internal storage with the following pattern

    write_internal string address
    
For example:

    write_internal e2e12c2281cdf3d350a34de4d5f56613 100

The above example outputs

    INFO # write_internal e2e12c2281cdf3d350a34de4d5f56613 100
    INFO # successfully erased page 100 (addr 0x6400)
    INFO # wrote local page to flash page 100 at addr 0x6400
    
## Read from internal storage  

Read a string from the internal storage with the following pattern

    read_internal address length_of_string
    
For example:

    write_internal 100 32
    
The above example outputs

    INFO # e2e12c2281cdf3d350a34de4d5f56613