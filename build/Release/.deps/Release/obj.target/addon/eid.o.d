cmd_Release/obj.target/addon/eid.o := cc '-DNODE_GYP_MODULE_NAME=addon' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-DSERVER' '-DBUILDING_NODE_EXTENSION' -I/home/firsti/.node-gyp/8.9.4/include/node -I/home/firsti/.node-gyp/8.9.4/src -I/home/firsti/.node-gyp/8.9.4/deps/uv/include -I/home/firsti/.node-gyp/8.9.4/deps/v8/include -I../RIOT/sys -I../RIOT/sys/include  -fPIC -pthread -Wall -Wextra -Wno-unused-parameter -m64 -O3 -fno-omit-frame-pointer  -MMD -MF ./Release/.deps/Release/obj.target/addon/eid.o.d.raw   -c -o Release/obj.target/addon/eid.o ../eid.c
Release/obj.target/addon/eid.o: ../eid.c \
 ../RIOT/sys/include/crypto/ciphers.h ../RIOT/sys/crypto/ciphers.c \
 ../RIOT/sys/include/crypto/ciphers.h ../RIOT/sys/crypto/aes.c \
 ../RIOT/sys/include/crypto/aes.h ../RIOT/sys/crypto/modes/ecb.c \
 ../RIOT/sys/include/crypto/modes/ecb.h ../eid.h
../eid.c:
../RIOT/sys/include/crypto/ciphers.h:
../RIOT/sys/crypto/ciphers.c:
../RIOT/sys/include/crypto/ciphers.h:
../RIOT/sys/crypto/aes.c:
../RIOT/sys/include/crypto/aes.h:
../RIOT/sys/crypto/modes/ecb.c:
../RIOT/sys/include/crypto/modes/ecb.h:
../eid.h:
