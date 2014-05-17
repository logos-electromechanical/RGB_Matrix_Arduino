/**************************************************************************/
/*!
    @file     RGB_Matrix_Arduino.h
    @author   Pierce Nichols (Logos Electromechanical
    @license  GPL v3 (see LICENSE.txt)

    This is a library for the Logos Electromechanical RGB LED Matrix

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#define RGB_MATRIX_PWM_BUF	192		// buffer length for PWM output
#define RGB_MATRIX_BIN_BUF	24		// buffer length for binary output

typedef enum {						
	RGB_MATRIX_PWM		= 0x1,
	RGB_MATRIX_BIN		= 0x2,
	RGB_MATRIX_R1		= 0x3
} rgb_matrix_t;

typedef enum {
	RGB_CMD_24_GRB_DATA		= 0x01,
	RGB_CMD_3_RGB_DATA		= 0x02,
	RGB_CMD_ALL_DIM			= 0x04,
	RGB_CMD_RED_DIM			= 0x08,
	RGB_CMD_GRN_DIM			= 0x10,
	RGB_CMD_BLU_DIM			= 0x20
} rgb_cmd_t;

class RGB_Matrix {
	public:
		RGB_Matrix(uint8_t address);
		RGB_Matrix(uint8_t address, uint8_t * buf);
		RGB_Matrix(uint8_t address, uint8_t * buf, rgb_matrix_t matrixType);
		
		void begin(void);
		
		void setPixel(uint8_t row, uint8_t col, uint8_t red, uint8_t grn, uint8_t blu);
		void setPixel(uint8_t row, uint8_t col, uint32_t rgb);
		void setCol(uint8_t col, uint32_t* colArray);
		void setCol(uint8_t col, uint8_t* redArray, uint8_t* grnArray, uint8_t* bluArray);
		void setRow(uint8_t row, uint32_t* rowArray);
		void setRow(uint8_t row, uint8_t* redArray, uint8_t* grnArray, uint8_t* bluArray);
		void clearPixel(uint8_t row, uint8_t col);
		void clearRow(uint8_t row);
		void clearCol(uint8_t col);
		void clearBuf(void);
		void printBuf(void);
		void setDim(uint8_t dim);
		void setRedDim(uint8_t dim);
		void setGrnDim(uint8_t dim);
		void setBluDim(uint8_t dim);
		uint8_t getDim(void);
		uint8_t getRedDim(void);
		uint8_t getGrnDim(void);
		uint8_t getBluDim(void);
		
		uint8_t * getBuf(void);
		rgb_matrix_t getType(void);
		uint8_t transmitBuf(void);
		
	private:
		rgb_matrix_t _matrixType;
		uint8_t * _buf;
		uint8_t _address;
		uint8_t _red;
		uint8_t _grn;
		uint8_t _blu;
		uint8_t _all;
};
