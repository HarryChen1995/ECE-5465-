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
 * @file    Project_3.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "RF24.h"

using namespace std;

//TODO: Configure GPIO
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;
char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

int main(void) {
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();


	bool role_ping_out = 1, role_pong_back = 0;
	bool role = 0;
	// Print preamble:
	cout << "RF24/examples/pingpair_dyn/\n";
	// Setup and configure rf radio
	radio.begin();
	radio.enableDynamicPayloads();
	radio.setRetries(5,15);
	radio.printDetails();

	printf("\n ************ Role Setup ***********\n");
	string input = "";
	char myChar = {0};
	cout << "Choose a role: Enter 0 for receiver, 1 for transmitter (CTRL+C to exit) \n>";
	getline(cin,input);
	if(input.length() == 1) {
		myChar = input[0];
		if(myChar == '0'){
			cout << "Role: Pong Back, awaiting transmission " << endl << endl;
		} else {  cout << "Role: Ping Out, starting transmission " << endl << endl;
		role = role_ping_out;
		}
	}

	if ( role == role_ping_out ) {
		radio.openWritingPipe(pipes[0]);
		radio.openReadingPipe(1,pipes[1]);
	} else {
		radio.openWritingPipe(pipes[1]);
		radio.openReadingPipe(1,pipes[0]);
		radio.startListening();
	}

	// forever loop
	while (1)
	{
		if (role == role_ping_out)
		{
			// The payload will always be the same, what will change is how much of it we send.
			static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012";
			// First, stop listening so we can talk.
			radio.stopListening();
			// Take the time, and send it.  This will block until complete
			printf("Now sending length %i...",next_payload_size);
			radio.write( send_payload, next_payload_size );
			// Now, continue listening
			radio.startListening();
			// Wait here until we get a response, or timeout
			unsigned long started_waiting_at = millis();
			bool timeout = false;
			while ( ! radio.available() && ! timeout )
				if (millis() - started_waiting_at > 500 )
					timeout = true;
			// Describe the results
			if ( timeout )
			{
				printf("Failed, response timed out.\n\r");
			}
			else
			{
				// Grab the response, compare, and send to debugging spew
				uint8_t len = radio.getDynamicPayloadSize();
				radio.read( receive_payload, len );
				// Put a zero at the end for easy printing
				receive_payload[len] = 0;
				// Spew it
				printf("Got response size=%i value=%s\n\r",len,receive_payload);
			}
			// Update size for next time.
			next_payload_size += payload_size_increments_by;
			if ( next_payload_size > max_payload_size )
				next_payload_size = min_payload_size;
			// Try again 1s later
			delay(100);
		}
		//
		// Pong back role.  Receive each packet, dump it out, and send it back
		//
		if ( role == role_pong_back )
		{
			// if there is data ready
			if ( radio.available() )
			{
				// Dump the payloads until we've gotten everything
				uint8_t len;
				while (radio.available())
				{
					// Fetch the payload, and see if this was the last one.
					len = radio.getDynamicPayloadSize();
					radio.read( receive_payload, len );
					// Put a zero at the end for easy printing
					receive_payload[len] = 0;
					// Spew it
					printf("Got payload size=%i value=%s\n\r",len,receive_payload);
				}
				// First, stop listening so we can talk
				radio.stopListening();
				// Send the final one back.
				radio.write( receive_payload, len );
				printf("Sent response.\n\r");
				// Now, resume listening so we catch the next packets.
				radio.startListening();
			}
		}
	}
}
