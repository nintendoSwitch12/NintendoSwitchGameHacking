#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "sdk.h"
#include "utils.h"

int main(int argc, char** argv)
{
    if(argc < 5)
    {
        printf("Usage: %s [enc / dec] [random_with_hex_string] [key_with_hex_string] [packet_with_hex_string]\n", argv[0]);
        return -1;
    }
    
    if(argv[1][0] != 'e' && argv[1][0] != 'd')
    	return -1;

    if(LoadSDK("./splatoon2_main.bin", 0x80004000, 0x2ebe000) != 0x80004000)
        return -1;

    if(LoadSDK("./splatoon2_sdk.bin", 0x82EC2000, 0xa10ad8) != 0x82EC2000)
        return -1;   
    
    unsigned char out[0x600] = {0, };
    unsigned char packet[0x5C0] = {0, };
    unsigned int len;
    unsigned int random;
    unsigned char key[16] = {0, };
	
	GetBytesFromHex(&random, argv[2], 4);
    GetBytesFromHex(key, argv[3], 16);
    len = strlen(argv[4])/2;
    GetBytesFromHex(packet, argv[4], len);
    
	int error = 0;
	if(argv[1][0] == 'e')
		error = EncryptPia(out, packet, len, random, key);
    else if(argv[1][0] == 'd')
    	error = DecryptPia(out, packet, len, random, key);

	if(error)
        printf("Error");
    else
        PrintHex(out, len);
    puts("");

    return 0;
}

