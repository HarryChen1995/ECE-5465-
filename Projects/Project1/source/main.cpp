/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MKL25Z128xxx4_Project.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include "mbedtls/check_config.h"
#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/timing.h"
#include "mbedtls/ssl_internal.h"
#include "mbedtls/aes.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    class Crypt {
        public:

            int generateKey(){
            	int ret=0;

				mbedtls_aes_init(&aes_in);
				mbedtls_aes_init(&aes_out);
				memcpy(iv,ogiv,16);

				mbedtls_aes_setkey_enc( &aes_in, key, 256 );
				mbedtls_aes_setkey_dec( &aes_out, key, 256 );

				return ret;
            }
            int encryptData(unsigned char* input,unsigned char* output){
            	int ret=0;

				ret = mbedtls_aes_crypt_cbc( &aes_in, MBEDTLS_AES_ENCRYPT, 128, iv, input, output );

				return ret;
            }
            int decryptData(unsigned char* input,unsigned char* output){
            	 int ret=0;

				ret = mbedtls_aes_crypt_cbc( &aes_out, MBEDTLS_AES_DECRYPT, 128, ogiv, input, output );

				return ret;
            }

            char* pers = "CyberAuto";

            mbedtls_aes_context aes_in;
            mbedtls_aes_context aes_out;

        private:
            unsigned char key[32]={ 'C', 12, 102, 74, 1, 99, 'y', 'b',\
             12, 36, 'S', 53, 2, 5, 34, 'e', 82, 91, 'C', 43, 110, 0, 55, 100, 'A', 13, 12, 45, 29, 110, 'R' };
            unsigned char iv[16];
            unsigned char ogiv[16] = { 14, 31, 6, 126, 18, 12, 36, 70, 100, 9, 42, 51, 111, 84, 3, 25 };

    };

    Crypt secretStuff;

    secretStuff.generateKey();

    unsigned char input[128];
	unsigned char output[128];
	unsigned char in[128];
	unsigned char put[128];

	memset(input, 0, 128);
	memset(output, 0, 128);
	memset(in, 0, 128);
	memset(put, 0, 128);

	memcpy(input,"hello",strlen("hello"));


	secretStuff.encryptData(input,output);
	//memset(input, 0, 128);
	printf("%s", output);
	secretStuff.decryptData(output,put);

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
    }
    return 0 ;
}
