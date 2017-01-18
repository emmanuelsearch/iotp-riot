#IOTP-RIOT IoT friendly (One Time Password OTP) on RIOT OS






# Setup

First of all you need to clone the RIOT OS in the root directory of this repo, and check a particular release:

``` 
  git clone https://github.com/RIOT-OS/RIOT
  cd RIOT
  git checkout RIOT-2016.10
  cd ..
```

Also, you can just run build.sh which does exactly that.

You may want to check the instructions at https://github.com/RIOT-OS/RIOT/wiki/Family%3A-native for your operating system.
Here are some hints on libraries you may need:
* `sudo apt-get install libc6-dev-i386`
* `sudo apt-get install gcc-multilib g++-multilib`

For a general guide for RIOT OS please have a look at https://github.com/RIOT-OS/RIOT/wiki/Introduction

# Compiling

## Configuration
Some parameters can be modified by setting variables needed for the application can be found in the file **config.h**
To main.c application registers clients to the server and sets the keys for the client in a deterministic way for demo purposes.

Iotp-riot's main file compiles two different console-based applications, namely a client and a server.

### Compile client
To compile the client application for generating root secrets and encrypting messages use the command
`make PART=client`
As the client is build by default you could just use a simple `make` aswell.
The compiled files will be found in the **bin/** folder.

### Compile server
To compile the server application for maintaining client infotmation and decrypting messages use the command
`make PART=server`
The compiled files will be found in the **bin/** folder, too.

At the end of the compilation process there will be shown some data that help you to estimate the RAM and ROM usage needed for you application.
As stated in https://github.com/RIOT-OS/RIOT/wiki/FAQ the required RAM is simply **data + bss** and the needed ROM memory is **text + data**

By default the BOARD is set to **native** so it is compiled to run in you host OS.
In order to compile the application for a specific hardware you have to set the BOARD variable in the make command.
You can find a list of supported hardware at https://github.com/RIOT-OS/RIOT/wiki/RIOT-Platforms.
There you can also find tutorials on how to compile and flash your RIOR OS application for the corresponding board.

# Running the apps

Although they don't interact with each other directly, you can start a client, type a message and then copy this message into the server, and it will be decrypted by the server.

To start the applications just do the following from the root of the repo directory:

## Running the client application
```
./bin/native/iotp-riot-client.elf
```
Then you will be prompted to choose which client you want to use (number 0 or number 1); afterwards, you can start typing messages that will be decrypted.

## Running the server application

```
./bin/native/iotp-riot-server.elf 
```

From this moment on, you can copy encrypted messages from the clients to the server, and see the decrypted value as well as the client id for the client generating the message





