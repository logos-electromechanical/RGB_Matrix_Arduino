/**************************************************************************/
/*!
    @file     RGB_Matrix_Arduino.cpp
    @author   Pierce Nichols (Logos Electromechanical
    @license  GPL v3 (see LICENSE.txt)

    This is a library for the Logos Electromechanical RGB LED Matrix

    @section  HISTORY

    v0.1  - First release
*/
/**************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include "RGB_Matrix_Arduino.h"

/**************************************************************************/
/*! 
    @brief  Instantiates a new Logos_RGB_Matrix with the given I2C address. 
	
	This allocates a buffer and defaults to an r2 backpack in PWM mode.
	The allocated buffer is 192 bytes long, so this may be problematic in
	memory constrained applications.
*/
/**************************************************************************/
RGB_Matrix::RGB_Matrix(uint8_t address) {
	_address = address;
	_matrixType = RGB_MATRIX_PWM;
	_buf = (uint8_t *)malloc(RGB_MATRIX_PWM_BUF);
	_red = 0xff;
	_grn = 0xff;
	_blu = 0xff;
	_all = 0xff;
}

/**************************************************************************/
/*! 
    @brief  Instantiates a new Logos_RGB_Matrix with the given I2C address and buffer. 
	
	This needs to be passed a pointer to a pre-allocated array of 192 bytes. Multiple
	objects can share a single buffer in memory-constrained applications.
	This defaults to an r2 backpack in PWM mode.
*/
/**************************************************************************/
RGB_Matrix::RGB_Matrix(uint8_t address, uint8_t* buf) {
	_address = address;
	_matrixType = RGB_MATRIX_PWM;
	_buf = buf;
	_red = 0xff;
	_grn = 0xff;
	_blu = 0xff;
	_all = 0xff;
}

/**************************************************************************/
/*! 
    @brief  Instantiates a new Logos_RGB_Matrix with the given I2C address and buffer. 
	
	This needs to be passed a pointer to a pre-allocated array of either 192 or 24 bytes, 
	depending on mode. RGB_MATRIX_PWM requires 192 bytes; the other modes 24.
*/
/**************************************************************************/
RGB_Matrix::RGB_Matrix(uint8_t address, uint8_t* buf, rgb_matrix_t matrixType) {
	_address = address;
	_matrixType = matrixType;
	_buf = buf;
	_red = 0xff;
	_grn = 0xff;
	_blu = 0xff;
	_all = 0xff;
}
		
/**************************************************************************/
/*! 
    @brief  Start the TWI interface
*/
/**************************************************************************/
void RGB_Matrix::begin(void) {
	Wire.begin();
}
		
void RGB_Matrix::setPixel(uint8_t row, uint8_t col, uint8_t red, uint8_t grn, uint8_t blu) {
	if (_matrixType == RGB_MATRIX_PWM) {
		//Serial.println("Setting PWM pixel");
		//Serial.println(red, HEX);
		//Serial.println(grn, HEX);
		//Serial.println(blu, HEX);
		// the pixel color order is Green, Red, Blue, in order to match the neopixels
		_buf[(col*3) + (row * 24)] 		= grn;
		_buf[(col*3) + (row * 24) + 1] 	= red;
		_buf[(col*3) + (row * 24) + 2] 	= blu;
	} else {
		//Serial.println("Setting BIN pixel");
		(red) ? _buf[row] 		|= _BV(col) 	: _buf[row] 		&= ~_BV(col);
		(grn) ? _buf[row + 8] 	|= _BV(col) 	: _buf[row + 8] 	&= ~_BV(col);
		(blu) ? _buf[row + 16] 	|= _BV(col) 	: _buf[row + 16] 	&= ~_BV(col);
	}
}

void RGB_Matrix::setPixel(uint8_t row, uint8_t col, uint32_t rgb) {
	setPixel(row, col, (uint8_t)((rgb >> 16) & 0xff), (uint8_t)((rgb >> 8) & 0xff), (uint8_t)(rgb & 0xff));
}

void RGB_Matrix::setCol(uint8_t col, uint32_t* colArray) {
	uint8_t row = 0;
	
	for (row = 0; row < 8; row++) {
		setPixel(row, col, colArray[row]);
	}
}

void RGB_Matrix::setCol(uint8_t col, uint8_t* redArray, uint8_t* grnArray, uint8_t* bluArray) {
	uint8_t row = 0;
	
	for (row = 0; row < 8; row++) {
		setPixel(row, col, redArray[row], grnArray[row], bluArray[row]);
	}
}

void RGB_Matrix::setRow(uint8_t row, uint32_t* rowArray) {
	uint8_t col = 0;
	
	for (col = 0; col < 8; col++) {
		setPixel(row, col, rowArray[col]);
	}
}

void RGB_Matrix::setRow(uint8_t row, uint8_t* redArray, uint8_t* grnArray, uint8_t* bluArray) {
	uint8_t col = 0;
	
	for (col = 0; col < 8; col++) {
		setPixel(row, col, redArray[col], grnArray[col], bluArray[col]);
	}
}

void RGB_Matrix::clearPixel(uint8_t row, uint8_t col) {
	_buf[col + (row * 24)] 		= 0;
	_buf[col + (row * 24) + 1]	= 0;
	_buf[col + (row * 24) + 2] 	= 0;
}

void RGB_Matrix::clearRow(uint8_t row) {
	uint8_t col = 0;
	
	for (col = 0; col < 8; col++) clearPixel(row, col);
}

void RGB_Matrix::clearCol(uint8_t col) {
	uint8_t row = 0;
	
	for (row = 0; row < 8; row++) clearPixel(row, col);
}

void RGB_Matrix::clearBuf(void) {
	uint8_t row = 0, col = 0;
	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) clearPixel(row, col);
	}
}

void RGB_Matrix::printBuf(void) {
	uint8_t row = 0, col = 0;
	
	if (_matrixType == RGB_MATRIX_PWM) {
		Serial.println("\t0GRB\t1GRB\t2GRB\t3GRB\t4GRB\t5GRB\t6GRB\t7GRB");
		for (row = 0; row < 8; row++) {
			Serial.print(row); Serial.print("\t");
			for (col = 0; col < 8; col++) {
				Serial.print(_buf[(row*24) + (col*3)], HEX); Serial.print("|");		// green
				Serial.print(_buf[(row*24) + (col*3) + 1], HEX); Serial.print("|");	// red
				Serial.print(_buf[(row*24) + (col*3) + 2], HEX); 					// blue
				Serial.print("\t");
			}
			Serial.println("");
		}
	} else {
		Serial.println("Red\t\tGreen\t\tBlue");
		for (row = 0; row < 8; row++) {
			Serial.print(_buf[row], BIN);
			Serial.print("\t");
			Serial.print(_buf[row + 8], BIN);
			Serial.print("\t");
			Serial.println(_buf[row + 16], BIN);
		}
	}
}

void RGB_Matrix::setDim(uint8_t dim) {
	_all = dim;
	Wire.beginTransmission(_address);
	Wire.write(RGB_CMD_ALL_DIM);
	Wire.write(_all);
	Wire.endTransmission();
}

void RGB_Matrix::setRedDim(uint8_t dim) {
	_red = dim;
	Wire.beginTransmission(_address);
	Wire.write(RGB_CMD_RED_DIM);
	Wire.write(_red);
	Wire.endTransmission();
}

void RGB_Matrix::setGrnDim(uint8_t dim) {
	_grn = dim;
	Wire.beginTransmission(_address);
	Wire.write(RGB_CMD_GRN_DIM);
	Wire.write(_grn);
	Wire.endTransmission();
}

void RGB_Matrix::setBluDim(uint8_t dim) {
	_blu = dim;
	Wire.beginTransmission(_address);
	Wire.write(RGB_CMD_BLU_DIM);
	Wire.write(_blu);
	Wire.endTransmission();
}

uint8_t RGB_Matrix::getDim(void) {
	return _all;
}

uint8_t RGB_Matrix::getRedDim(void) {
	return _red;
}

uint8_t RGB_Matrix::getGrnDim(void) {
	return _grn;
}

uint8_t RGB_Matrix::getBluDim(void) {
	return _blu;
}

uint8_t * RGB_Matrix::getBuf(void) {
	return _buf;
}

rgb_matrix_t RGB_Matrix::getType(void) {
	return _matrixType;
}

uint8_t RGB_Matrix::transmitBuf(void) {
	uint8_t i, j;
	
	switch(_matrixType) {
		case RGB_MATRIX_PWM:
			for (i = 0; i < 8; i++) {
				// Apologies for the magic numbers in this section -- I couldn't think of good names for them
				// This loop runs through the 192 byte buffer, transmitting it in 24 byte chunks using repeated starts
				// This repeated start is necessitated by the relatively small size (32 bytes) of the Wire library
				// write buffer. 
				Wire.beginTransmission(_address);
				if (i == 0) Wire.write(RGB_CMD_24_GRB_DATA);
				for (j = 0; j < 24; j++) Wire.write(_buf[(i*24) + j]);
				(i == 7) ? Wire.endTransmission(1) : Wire.endTransmission(0);
			}
			break;
		case RGB_MATRIX_BIN:
			Wire.beginTransmission(_address);
			Wire.write(RGB_CMD_3_RGB_DATA);
			Wire.write(_buf, RGB_MATRIX_BIN_BUF);
			Wire.endTransmission();
			break;
		case RGB_MATRIX_R1:
			Wire.beginTransmission(_address);
			Wire.write(_buf, RGB_MATRIX_BIN_BUF);
			Wire.endTransmission();
			break;
		default:
			break;
	}
}