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

// ensure this library description is only included once
#ifndef gwiot9741_h
#define gwiot9741_h

// includes
#include "Arduino.h"
#include "SoftwareSerial.h"

struct rfidData_t{
	char type;
	char hiddenByte;
	char id[4];
};

class gwiot9741
{
  public:
    bool newCardAvailable(void);
	gwiot9741(uint8_t rxPin, uint8_t txPin);
    uint32_t getID(void);
	
  private:
    rfidData_t _data;
	bool _corruptDataHead = false;
	bool _corruptDataEnd = false;
	bool _wrongLength = false;
	bool _newIDavailable = false;
	SoftwareSerial _rfidSerial;
	uint32_t _ID;
};

#endif

