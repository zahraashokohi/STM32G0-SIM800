#ifndef    _SIM800_H
#define    _SIM800_H

#include "StreamBuffer.h"
#include "InputStream.h"
#include "OutputStream.h"

#include "main.h"


#define RX_BUFFER_SIZE     1024
#define TX_BUFFER_SIZE     1024

extern uint8_t uartStream1RxBuff[RX_BUFFER_SIZE];
extern uint8_t uartStream1TxBuff[TX_BUFFER_SIZE];

typedef enum 
{
  RING = 1,
  CLIP = 2,
  DTMF = 3,
  CMTI = 4,
  CLCC = 5,
  OK = 6,
  CMT = 8,
  CMGR = 9,
  SMS = 10,
  CMGS = 11,
  ERROR_ = 12,
  ARROWS = 13,
}SIM800_MOOD;

typedef struct{
	uint8_t RxData[FIFO_BUFFER_SIZE];
	uint8_t IndexRd;
	uint8_t IndexWr;
	uint8_t FifoRx[FIFO_BUFFER_SIZE];
	uint8_t FifoIndex;
	uint8_t Data;
	uint8_t DataIn;
	uint8_t FrameState;
	uint8_t ChecksumFrame;
	uint8_t uartData;
}buffer_t;
extern buffer_t BUF;


typedef struct {
	UART_HandleTypeDef*		HUART;
	DMA_HandleTypeDef*			RxDMA;
	DMA_HandleTypeDef*			TxDMA;
	IStream								In;
	OStream								Out;
} UARTStream;
extern UARTStream uartStream1;

typedef struct
{
  uint8_t FClip : 2;
  uint8_t FRing : 2;
  uint8_t FDTMF : 2;
  uint8_t FReciveSMS : 2;
  uint8_t FCheckSMS : 2;
  uint8_t FTrueSMS : 2;
  uint8_t FOK : 2;
  uint8_t FCLCC : 2;
  uint8_t FCMT : 2;
  uint8_t FCMTI : 2;
  uint8_t FCMGR : 2;
}sim800_t;
extern sim800_t SIM800;

typedef struct
{
  uint8_t case_1 : 1;
  uint8_t case_2 : 1;
  uint8_t case_3 : 1;
  uint8_t case_4 : 1;
  uint8_t case_5 : 1;
  uint8_t case_6 : 1;
  uint8_t password : 1;
  uint8_t contact : 1;
  uint8_t delet : 1;
  uint8_t report : 1;
  uint8_t menu : 1;
  uint8_t eeprom : 1;
  uint8_t sms : 1;
  uint8_t timer : 1;
  uint8_t correctPass : 1;
  uint8_t cmgr : 1;
  uint8_t cmti : 1;
  uint8_t smspass : 1;
  uint8_t smsmenu : 1;
  uint8_t checkcode : 1;
  uint8_t smsselsectmenu : 1;
  uint8_t smsphonesaver : 1;
  uint8_t smsphonedelet : 1;
  uint8_t smscase_1 : 1;
  uint8_t smscase_2 : 1;
  uint8_t smscase_3 : 1;
  uint8_t smscase_4 : 1;
  uint8_t savephoneok : 1;
  uint8_t deletphoneok : 1;
  uint8_t contactlist : 1;
  uint8_t passchange : 1;
  uint8_t OK : 1;
  uint8_t deletsms : 1;
  uint8_t AT : 1;
  uint8_t restar : 1;
  uint8_t Error : 1;
  uint8_t Arrows : 1;
  uint8_t CMGS : 1; 
  uint8_t endcall : 1;
}fcase;
extern fcase flg;

void sendData (char *str);
uint8_t Sim800_answerCall(void);
uint8_t Sim800_hangoffCall(void);
void pwrkey_on(GPIO_TypeDef *gpio, uint16_t pin);
void Uart_dataPars_(void);
uint8_t Sim800_config(void);
#endif
