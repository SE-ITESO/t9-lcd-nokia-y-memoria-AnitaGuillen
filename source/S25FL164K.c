/*
 * S25FL164K.c
 *
 *  Created on: 12 abr. 2024
 *      Author: prome
 */

#include "S25FL164K.h"
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "clock_config.h"

void s25fl_read_data(uint32_t address, uint8_t * readData, uint32_t dataSize)
{
	dspi_half_duplex_transfer_t masterXfer;
	uint8_t opcodeAndAddress[4] =
	{
		S25FL_READ_OPCODE,
		(uint8_t)(address >> 16),
		(uint8_t)(address >> 8),
		(uint8_t)(address),
	};

	masterXfer.txData                = opcodeAndAddress;
	masterXfer.rxData                = readData;
	masterXfer.txDataSize            = 4;
	masterXfer.rxDataSize            = dataSize;
	masterXfer.isTransmitFirst       = true;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.configFlags           = kDSPI_MasterCtar1 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;

	//Read data
	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);
}


void s25fl_init()
{
	uint32_t srcClock_Hz;

	CLOCK_SetSimSafeDivs();

	dspi_master_config_t masterConfig;

	CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

	PORT_SetPinMux(PORTC, 3, kPORT_MuxAlt2);            /* PORTC3 is configured as SPI0_PCS1 */

	PORT_SetPinMux(PORTD, 1, kPORT_MuxAlt2);            /* PORTD1 is configured as SPI0_SCK */
	PORT_SetPinMux(PORTD, 2, kPORT_MuxAlt2);            /* PORTD2 is configured as SPI0_SOUT */
	PORT_SetPinMux(PORTD, 3, kPORT_MuxAlt2);            /* PORTD3 is configured as SPI0_SIN */

	/* Master config */
	masterConfig.whichCtar                                = kDSPI_Ctar1;
	masterConfig.ctarConfig.baudRate                      = S25FL_TRANSFER_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame                  = 8U;
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveLow;
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseSecondEdge;
	masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 1000U;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 1000U;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000U;

	masterConfig.whichPcs           = kDSPI_Pcs1;
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK        = false;
	masterConfig.enableRxFifoOverWrite      = false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint                = kDSPI_SckToSin0Clock;

	srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);

}
