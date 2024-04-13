
#define SPI_MEMORY_TEST
#define NOKIA_IMAGE_SIZE 504U

#define IMAGEN1		0x40000U
#define IMAGEN2		0x41000U
#define IMAGEN3		0x42000U
#define IMAGEN4		0x43000U
#define IMAGEN5		0x44000U

#include "S25FL164K.h"


int main(void)
{

#ifdef SPI_MEMORY_TEST
	uint32_t address = IMAGEN1;
	uint8_t readData[NOKIA_IMAGE_SIZE] = {0};
	s25fl_init();
	s25fl_read_data(address, readData, NOKIA_IMAGE_SIZE);
#endif

	for(;;)
	{

	}
	return 0 ;
}
