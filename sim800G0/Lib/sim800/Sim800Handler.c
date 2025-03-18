#include "Sim800Handler.h"
#include <stdlib.h>

buffer_t BUF;
fcase flg;

#define Send_string(str)                  sendData((str))
#define DELAY(time)			                 (HAL_Delay((time)))


UARTStream uartStream1;
Stream mybuffer[1];

uint8_t uartStream1RxBuff[RX_BUFFER_SIZE];
uint8_t uartStream1TxBuff[TX_BUFFER_SIZE];
uint8_t state_t = 0;
sim800_t SIM800 = {0};


char  *start;
char  *end;

void Clip_data (void)
{
  	 if((SIM800.FClip == 1) && (SIM800.FRing == 1))
	 {  
		  SIM800.FClip = 0;
		  SIM800.FRing = 0;
		  start= strtok(BUF.RxData,",");
		 // strcpy(number_20,start);
		  end = strchr(start,'"');

      Sim800_answerCall();

	 }
}

uint8_t ProssesaAt (uint8_t *str)
{
	if(strncmp("RING", BUF.RxData,4) ==  0)
	{
		 SIM800.FRing = 1;	
		 state_t = RING;
	}

	if(strncmp("+CLIP:", BUF.RxData, 6) == 0)
	{
		 SIM800.FClip = 1;
		 state_t = CLIP;
	}
	
	if(strncmp("+DTMF:", BUF.RxData, 6) == 0)
	{
		 SIM800.FDTMF = 1;
		 state_t = DTMF;
	}
	if(strncmp("NO CARRIER", BUF.RxData, 7) == 0)
	{

	}
	if(strncmp("+CLCC:", BUF.RxData, 6) == 0)
	{
	  state_t = CLCC;
	  SIM800.FCLCC = 1;
	}
    if(strncmp("OK", BUF.RxData, 2) == 0)
	{
		 SIM800.FOK = 1;
		 state_t = OK;
	}
    if(strncmp("ERROR", BUF.RxData, 4) == 0)
	{
		 state_t = ERROR_;
	}
	if(strncmp("+CMGS:", BUF.RxData, 6) == 0)
	{
		 state_t = CMGS;
	}
    if(strncmp(">", BUF.RxData, 1) == 0)
	{
		 state_t = ARROWS;
	}		
	if(strncmp("+CMTI:", BUF.RxData, 6) == 0)
	{
		 SIM800.FCMTI = 1;
		 state_t = CMTI;
	}
	if(strncmp("+CMT:", BUF.RxData, 5) == 0)
	{
		 SIM800.FCMT = 1;
		 state_t = CMT;
	}
	if(strncmp("+CMGR:", BUF.RxData, 6) == 0)
	{
		 SIM800.FCMGR = 1;
		 state_t = CMGR;
	}
	return state_t;
}

void state_check (uint8_t state)
{
  switch (state)
  {
	case OK:
	    flg.OK = 1;
	    state_t = 0;
		state = 0;
		break;
	case ERROR_:
	    flg.Error = 1;
	    state_t = 0;
		state = 0;
		break;
	case ARROWS:
	    flg.Arrows = 1;
	    state_t = 0;
		state = 0;
		break;
	case CMGS:
	//  Send_string("AT+CMGDA=\"DEL ALL\"\r\n");
	    flg.CMGS = 1;
	    flg.sms = 0;
		flg.passchange = 0;
		flg.savephoneok = 0;
		flg.deletphoneok = 0;
		flg.contactlist = 0;
		flg.case_5 = 0;
	    state_t = 0;
		state = 0;
		break;
	case RING:
		state_t = 0;
		state = 0;
		break;
	case CLIP:	
 		Clip_data();
		state_t = 0;
		state = 0;
		break;
	case DTMF: 
	 
		break;
  	case CLCC:

		break;

  }
}


void Uart_dataPars_(void)
{
  while (IStream_available(&uartStream1.In) > 0) 
  {
	   BUF.uartData = IStream_readUInt8(&uartStream1.In);
		 BUF.RxData[BUF.IndexRd++] = BUF.uartData; 
		if(BUF.uartData == '\n')
		{
      BUF.RxData[BUF.IndexRd++] = '\0'; 			
			BUF.IndexRd = 0;
			state_check(ProssesaAt(BUF.RxData));

		}
		if(BUF.IndexRd == '>')
		{
			BUF.RxData[BUF.IndexRd++] = '\0'; 	
			BUF.IndexRd = 0;
			//flg.Arrows = 1;
		}
  }
}

uint8_t Sim800_answerCall(void)
{
    Send_string("ATA\r\n");  
	OStream_writeBytes(&uartStream1.Out, (uint8_t*)"ATA\r\n",sizeof("ATA\r\n"));
	    OStream_flush(&uartStream1.Out);
    return NULL;
}

uint8_t Sim800_hangoffCall(void)
{
    Send_string("ATH\r\n");  
	OStream_writeBytes(&uartStream1.Out, (uint8_t*)"ATH\r\n",sizeof("ATH\r\n"));
	    OStream_flush(&uartStream1.Out);
    return NULL;
}

uint8_t Sim800_delAllSms(void)
{
    // Can take up to 25 seconds
    Send_string("AT+CMGDA=\"DEL ALL\"\r\n");
	
	
	return NULL;
}

void sendData (char *str)
{
	OStream_writeBytes(&uartStream1.Out, (uint8_t*) &str, sizeof(&str));
	OStream_flush(&uartStream1.Out);
}
uint8_t Sim800_config(void)
{
	uint8_t state_set = 0;
  switch (state_set)
  {
	case 0:
	  // Send_string("AT+CMGF=1\r\n"); 
			OStream_writeBytes(&uartStream1.Out, (uint8_t*) "AT+CMGF=1\r\n",12);
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*) "ATE0\r\n",7);
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*) "AT\r\n",sizeof("AT\r\n"));
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*) "AT+CLIP=1\r\n",sizeof("AT+CLIP=1\r\n"));
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*)"AT+CFUN=1\r\n",sizeof("AT+CFUN=1\r\n"));
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*)"AT+CLCC=1\r\n",sizeof("AT+CLCC=1\r\n"));
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*)"AT+CSMP=17,167,0,0\r\n",sizeof("AT+CSMP=17,167,0,0\r\n"));
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*)"AT+CREG=0\r\n",sizeof("AT+CREG=0\r\n"));
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*)"AT+DDET=1\r\n",sizeof("AT+DDET=1\r\n"));
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			OStream_writeBytes(&uartStream1.Out, (uint8_t*)"AT+CMGDA=\"DEL ALL\"\r\n",sizeof("AT+CMGDA=\"DEL ALL\"\r\n"));
			OStream_flush(&uartStream1.Out);
			HAL_Delay(200);
			state_set = 1;
			flg.AT = 1;
	   break;	
	case 1:
	   // if(flg.OK == 1)
		{
	   	//	Send_string("ATE0\r\n"); 
			 
	   		state_set = 2;
			flg.OK = 0;
		
		}
	  	//if(flg.restar)
		{
		 // pwrkey_on(PWRKEY_GPIO_Port, PWRKEY_Pin);
		 // DELAY(8000);
		 // Send_string("AT\r\n"); 
		  flg.restar = 0;
		}
	   break;	
  	case 2:
	    //if(flg.OK == 1)
		{
	   		Send_string("AT\r\n"); 
		
	   		state_set = 3;
			flg.OK = 0;
		}
	   break;
	case 3:

	  // if(flg.OK == 1)
	   {	 
		 // Send_string("AT+CLIP=1\r\n");
			 
		  state_set = 4;
		  flg.OK = 0;
	   }
		break;
    case 4:   
	  //if(flg.OK == 1)
	  {
		//Send_string("AT+CFUN=1\r\n");
	 
		state_set = 5;
		flg.OK = 0;
	  }
		break;
	case 5:
	//  if(flg.OK == 1)
	  {
		//Send_string("AT+CLCC=1\r\n");
		
		state_set = 6;
		flg.OK = 0;
	  }
		break;
	case 6:
	 // if(flg.OK == 1)
	  {
	    //Send_string("AT+CSMP=17,167,0,0\r\n");
			
		state_set = 7;
		flg.OK = 0;
	  }
		break;
	case 7:
	 // if(flg.OK == 1)
	  {
	    //Send_string("AT+CREG=0\r\n");
			
		state_set = 8;
  		flg.OK = 0;
	  }
		break;
	case 8: 
	  //if(flg.OK == 1)
	  {
		//Send_string("AT+DDET=1\r\n");
			
		state_set = 9;
  		flg.OK = 0;
	  }
		break;
	case 9:
 // if(flg.OK == 1)
	  {
	    //Send_string("AT+CMGDA=\"DEL ALL\"\r\n");
			
   		state_set = 10;
   		flg.OK = 0;
	  }
	  case 10:
	//  if(flg.OK == 1)
	  {
	  //  Send_string("AT+CMGDA=\"DELALL\"\r\n");
			OStream_writeBytes(&uartStream1.Out, (uint8_t*)"AT+CMGDA=\"DELALL\"\r\n",sizeof("AT+CMGDA=\"DELALL\"\r\n"));
	    OStream_flush(&uartStream1.Out);
   		state_set = -1;
   		flg.OK = 0;
	  }
		break;
  }
  return -1;
}

void pwrkey_on(GPIO_TypeDef *gpio, uint16_t pin)
{
  HAL_GPIO_WritePin(gpio,pin,GPIO_PIN_SET);
  HAL_Delay(1500);
  HAL_GPIO_WritePin(gpio,pin,GPIO_PIN_RESET);
}

