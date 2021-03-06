/*
 * FSMprotocol.cpp
 *
 *  Created on: 4 dic. 2019
 *      Author: Alberto
 */

#include "FSMprotocol.h"
#include <SoftwareSerial.h>

SoftwareSerial* serialProtocol = NULL;

FSMprotocol::FSMprotocol() {
	// TODO Auto-generated constructor stub
	state = WAIT;
}

void FSMprotocol::setSerial(void *uart_device) {
	serialProtocol = (SoftwareSerial*) uart_device;
}

void FSMprotocol::loop(){
	while(serialProtocol->available() > 0){
		receive(serialProtocol->read());
	}
}

void FSMprotocol::receive(int input) {
	switch (state) {
	case WAIT:
		if (input == START_CMD) {
			cmd_byte = input; // store valid command
			state = DATA;
		}
		break;
	case DATA:
		if (input > -1) {
			opr_byte = input; // store valid command
			state = END;
		} else {
			state = WAIT;
		}
		break;
	case END:
		if (input == END_CMD) {
			if(exec_cmd(opr_byte)){
				sendOK();
			}
			state = WAIT;
		}else {
			state = WAIT;
		}
		break;
	}
}

bool FSMprotocol::exec_cmd(unsigned int cmd) {
	bool check = true;
	switch (cmd) {
	case LED_1_OFF:
		funcion(cmd);
		break;
	case LED_1_ON:
		funcion(cmd);
		break;
	case LED_2_OFF:
		funcion(cmd);
		break;
	case LED_2_ON:
		funcion(cmd);
		break;
	case LED_3_OFF:
		funcion(cmd);
		break;
	case LED_3_ON:
		funcion(cmd);
		break;
	case LED_4_OFF:
		funcion(cmd);
		break;
	case LED_4_ON:
		// ENCENDER LED
		funcion(cmd);
		break;
	default:
		 check = false;
		break;
	}
	return  check;
}

void FSMprotocol::sendFail() {
	send(_FAIL);
}

void FSMprotocol::sendOK() {
	send(_OK);
}


void FSMprotocol::send(int input) {
	int written = 0;
	buf[0] = START_CMD;
	buf[1] = input;
	buf[2] = END_CMD;
	while (written < BUFFER_LENGHT) {
		serialProtocol->write(buf[written]);
		written++;
	}
}

void FSMprotocol::function_callback(void(*funcionEjecutar)(int)){
	funcion = funcionEjecutar;
}
