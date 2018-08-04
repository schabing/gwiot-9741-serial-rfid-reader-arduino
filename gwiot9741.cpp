/*
  gwiot9741 rfid reader library
  Copyright (C) by Moritz Schabinger (mail: stadtfrosch@gmail.com)
  
  This Library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with the gwiot9741 rfid reader library.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "gwiot9741.h"


gwiot9741::gwiot9741(uint8_t rxPin, uint8_t txPin): _rfidSerial(rxPin, txPin){
	_rfidSerial.begin(9600);
}

bool gwiot9741::newCardAvailable(void){
	if (_rfidSerial.available()) {				
    if (_rfidSerial.read() == 0x02) {    		//check for correct Data Head
      while (!_rfidSerial.available());			//wait for the next byte to be sent
      if (_rfidSerial.read() == 0x0A) {  		//check if packet has 10 byte, -> ID has 4 byte
        while (!_rfidSerial.available());
		
        _data.type = _rfidSerial.read();		//read in chip type
        while (!_rfidSerial.available());
        
		_data.hiddenByte = _rfidSerial.read();	//read in hidden byte
        
		for (size_t i = 0; i < 4; i++) {		//read in 4 id bytes
          while (!_rfidSerial.available());
          _data.id[i] = _rfidSerial.read();
        }
		
		while (!_rfidSerial.available());		//clear BCC check byte from buffer
		_rfidSerial.read();
		while (!_rfidSerial.available());
        if (_rfidSerial.read() != 0x03) {		//check for correct Data packet end
          _corruptDataEnd = true;
        }
      } else {
        _wrongLength = true;
      }
    }
    else {
      _corruptDataHead = true;
    }
    if(_corruptDataHead == false && _wrongLength == false && _corruptDataEnd == false){
		_newIDavailable = true;
	}else{
		_newIDavailable = false;
		_corruptDataHead = false;
		_corruptDataEnd = false;
		_wrongLength = false;
	}
	return true;
  }
  return false;
}

uint32_t gwiot9741::getID(void){				//convert the 4 id bytes into a 32 bit unsigned integer
	if(_newIDavailable){
	_ID = 0;
	_ID += _data.id[0];
	_ID = _ID<<8;
	_ID += _data.id[1];
	_ID = _ID<<8;
	_ID += _data.id[2];
	_ID = _ID<<8;
	_ID += _data.id[3];
	_newIDavailable = false;
	return _ID;
	}
	return 0;
}