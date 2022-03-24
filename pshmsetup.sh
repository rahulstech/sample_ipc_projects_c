clear
rm -f /dev/shm/SHM_TEST
gcc pshmpub.c -lpthread -lrt -o pshmpub
gcc pshmsub.c -lpthread -lrt -o pshmsub