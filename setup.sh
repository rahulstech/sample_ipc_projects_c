clear
gcc library.h library.c server.c -lpthread -lrt -o server
gcc library.h library.c client.c -lpthread -lrt -o client