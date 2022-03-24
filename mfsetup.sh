clear
rm -f /dev/shm/shm_test
gcc mfclient.c -lpthread -lrt -o mfclient
gcc mfserver.c -lpthread -lrt -o mfserver