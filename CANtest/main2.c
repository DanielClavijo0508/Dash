
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "config.h"
#include "CANInterface.h"
#include "COM1939.h"

//////////////////////////////////////
#include <time.h>
/////////////////////////////////////

byte msgNAME[] =
{
  (byte)(NAME_IDENTITY_NUMBER & 0xFF),
  (byte)((NAME_IDENTITY_NUMBER >> 8) & 0xFF),
  (byte)(((NAME_MANUFACTURER_CODE << 5) & 0xFF) | (NAME_IDENTITY_NUMBER >> 16)),
  (byte)(NAME_MANUFACTURER_CODE >> 3),
  (byte)((NAME_FUNCTION_INSTANCE << 3) | NAME_ECU_INSTANCE),
  (byte)(NAME_FUNCTION),
  (byte)(NAME_VEHICLE_SYSTEM << 1),
  (byte)((NAME_ARBITRARY_ADDRESS_CAPABLE << 7) | (NAME_INDUSTRY_GROUP << 4) | (NAME_VEHICLE_SYSTEM_INSTANCE))
};

byte SampleTxData[] = {0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31};

// Function calls in this module
int kbhit(char* key);

int main()
{
    // Declarations
    char sCOMPort[20];
    int nIndex;
    int nSA;
    int nJ1939Status;

    long lPGN;
    int nPriority;
    int nSourceAddress;
    int nDestAddress;
    int nDataLen;
    byte nData[MAXDATALEN];

    char sSWVersion[10];
    char sHWVersion[10];

    // Initialize the port and protocol settings
    strcpy(sCOMPort, COMPORT);
    COM1939_Initialize(&sCOMPort[0]);
    COM1939_SendMessageMode(MSGMODE_GATEWAY2);

    if(bCommunicationError == false)
    {
        printf("COM Port Init OK. %s\n\r", sPort);
        printf("Press 'h' for Help\n\r");
    }// end if
    else if(bErrorOpenCOMPort == true)
        printf("Unable to open COM port: %s - ErrNo: %d\n\r", sPort, nErrNum);
    else if(bErrorCOMPortSettings == true)
        printf("Unable to apply COM port settings: %s - ErrNo: %d\n\r", sPort, nErrNum);

    // Main loop
    bool bExit = false;
    bool bAllowAllPGNs = true;
    bool bEnableHeartbeat = false;
/////////////////////////////////AGREGADO POR DANIEL
    	int conteo=0;//VERIFICA ACTIVACION DE  LECTURA
	int a=0;//CONTEO DE TRAMAS
	int conteon=0;
    	int conteo2=0;//CONTEO DE CICLOS
	float volvo=0;
	float prev=0;
	int cont=0;//contol de pgn si entra o no
	//
		float res=0; //residuo para conteo microsegundos
	//

/////////////////////////////////


    while(1)
    {
        // Wait for assigned system loop time period
        usleep(SYSTEM_LOOP_TIME);
	conteon=conteon+1;
	conteo2=(conteon)/10;
        // Call the COM1939 protocol stack
        int nStatus = COM1939_Receive(&lPGN, &nPriority, &nSourceAddress, &nDestAddress, &nData[0], &nDataLen);

        switch(nStatus)
        {
            case RX_Message:        // PGN received
                printf("PGN:%x P:%d SA:%d DA:%d Len:%d Data:", lPGN, nPriority, nSourceAddress, nDestAddress, nDataLen);
		for(nIndex = 0; nIndex < nDataLen; nIndex++)
                    printf("%x ", nData[nIndex]);
                printf("\n\r");
//////////////////////////////////////////
		if(lPGN==0xFEEA||lPGN==0xFEF5||lPGN==0xFE4E||lPGN==0x1FF25||lPGN==0xFF25||lPGN==0xF009||lPGN==0xFEF1||lPGN==0x1FF27||lPGN==0xFEEC||lPGN==0xFEF3||lPGN==0xFEF1||lPGN==0xFEEE||lPGN==0xFEEF||lPGN==0xF004||lPGN==0xFD7D||lPGN==0xFEC1||lPGN==0xFEF2||lPGN==0xFEE9||lPGN==0xFEFC||lPGN==0xF005)
		{
			FILE *fichero;
			char archivo[50];
			char extension[]=".txt";

			sprintf( archivo, "%x%s",lPGN , extension );
//			sprintf( archivo, "PGN%s", extension );
//			sprintf( archivo, "PGN %x %s", lPGN, extension );
		fichero = fopen(archivo, "wt");

		    fprintf(fichero, "%x\n", lPGN);
		    for(nIndex = 0; nIndex < nDataLen; nIndex++)
			{
			    fprintf(fichero, "%x", nData[nIndex]);
			}

/////

	fprintf(fichero,"\n");

	time_t now = time(NULL);
    	if (now == -1) {
                puts("The time() function failed");
    	}
	    fprintf(fichero,"%ld\n", now);


/*	time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"%y%m%d%H%M%S",tlocal);
        fprintf(fichero,"%s",output);
=======
/*	time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"%y%m%d-%H%M%S",tlocal);
        fprintf(fichero,"\n%s",output);
>>>>>>> 4faf6073691c2e86a00f3ae62ccf81b9f8ac88b1
	res=conteo2%1000;
	fprintf(fichero,"-%d",res);


////
<<<<<<< HEAD
//		    fprintf(fichero, "\n");
=======
		    fprintf(fichero, "\n");
>>>>>>> 4faf6073691c2e86a00f3ae62ccf81b9f8ac88b1
*/		    fclose(fichero);
		    printf("Proceso completado");
		    cont=1;
		}
		else
		{
			cont=0;
		}
		a=a+1;
		conteo=1;
		volvo=conteon-prev;
		volvo=volvo/10;
		prev=conteon;
		printf("Activacion: %d\n",conteo);
		printf("N° de Tramas recibidas: %d\n",a);
		printf("N° de ciclos realizados (milisegundos): %d\n",conteo2);
		printf("Tiempo entre PGN (milisegundos): %f\n",volvo);
		printf("PGN valido en SKG %d\n",cont);
		printf("\n");
//		printf("prev: %d\n",prev);
///////////////////////////////////////////
                break;

            case RX_ACK_FA:
                printf(" FA ACK\n\r");
///////////////////////////////////////
		conteo=1;
		printf("Activacion: %d\n",conteo);
		printf("N° de Tramas recibidas: %d\n",a);
		printf("N° de ciclos realizados (milisegundos): %d\n",conteo2);
		printf("Tiempo entre PGN (milisegundos): %f\n",volvo);
		printf("\n");
//		printf("prev: %d\n",prev);
/////////////////////////////////////////
                break;

            case RX_ACK_FD:
                printf(" FD ACK\n\r");
//////////////////////////////////////////
		conteo=0;
		printf("Activacion: %d\n",conteo);
		printf("N° de Tramas recibidas: %d\n",a);
		printf("N° de ciclos realizados (milisegundos): %d\n",conteo2);
		printf("Tiempo entre PGN (milisegundos): %f\n",volvo);
		printf("\n");
//		printf("prev: %d\n",prev);
//////////////////////////////////////////
                break;

            case RX_ACK_SETPARAM:
                printf(" SETPARAM ACK\n\r");
                break;

            case RX_ACK_SETPARAM1:
                printf(" SETPARAM1 ACK\n\r");
                break;

            case RX_ACK_RESET:
                printf(" RESET ACK\n\r");
/////////
                        COM1939_AddFilter(PGN_ALLOW_ALL);
/////////
               break;

            case RX_ACK_MSGMODE:
                printf(" SETMSGMODE ACK\n\r");
/////////
                        COM1939_AddFilter(PGN_ALLOW_ALL);
/////////
                break;

            case RX_HEART:
                if(bEnableHeartbeat == true)
                {
                    if(bjCOM1939Heartbeat == true)
                        printf("Tick\n\r");
                    else
                        printf("Tock\n\r");
                }
                break;

            case RX_RS:
                nJ1939Status = COM1939_GetStatus(&nSA);
                printf(" Status: \n\r");
                printf("SA: %d - ", nSA );

                switch(nJ1939Status)
                {
                    case RS_STATUS_NONE:
                        printf("No J1939 Address Claimed.\n\r");
                        break;

                    case RS_STATUS_ADDRESSCLAIMINPROGRESS:
                        printf("Address Claim in Progress.\n\r");
                        break;

                    case RS_STATUS_ADRESSCLAIMSUCCESSFUL:
                        printf("Address Claim Successful.\n\r");
                        break;

                    case RS_STATUS_ADDRESSCLAIMFAILED:
                        printf("Address Claim Failed.\n\r");
                        break;

                    case RS_STATUS_LISTENONLYMODE:
                        printf("Listen-Only Mode.\n\r");
                        break;

                }// end switch

                break;

        }// end switch

        // Check for user interaction
        char key;
      if(kbhit(&key))
        {
	    switch(key)
            {
                case 'a':       // Allow all messages (Toggle)
                    bAllowAllPGNs = !bAllowAllPGNs;
                    if(bAllowAllPGNs == true)
                        COM1939_AddFilter(PGN_ALLOW_ALL);
                    else
                        COM1939_DelFilter(PGN_ALLOW_ALL);
                    break;

                case 'b':
                    bEnableHeartbeat = !bEnableHeartbeat;
                    break;

                case 'c':
                    COM1939_SetProtocolParameters(msgNAME, SA_PREFERRED, ADDRESSRANGEBOTTOM, ADDRESSRANGETOP, OPMODE_EVENT, true);
                    break;

                case 'd':
                    COM1939_DelFilter(PGN_SAMPLE_RX);
                    break;

                case 'f':
                    COM1939_AddFilter(PGN_SAMPLE_RX);
                    break;

                case 'h':       // Help message
                    printf("\n\ra - Toggle Pass [a]ll PGNs");
                    printf("\n\rb - Toggle heart[b]eat");
                    printf("\n\rc - [c]laim address");
                    printf("\n\rd - [d]elete sample PGN filter");
                    printf("\n\rf - Add sample PGN [f]ilter");
                    printf("\n\rr - Reset gateway");
                    printf("\n\rs - Request J1939 [s]tatus");
                    printf("\n\rt - [t]ransmit sample PGN");
                    printf("\n\rv - Get hardware & software [v]ersion");
                    printf("\n\rx - E[x]it\n\r");
                    break;

                case 'r':
                    COM1939_ResetGateway();
                    COM1939_SendMessageMode(MSGMODE_GATEWAY2);
                    break;

                case 's':
                    COM1939_RequestStatus();
                    break;

                case 't':
                    nJ1939Status = COM1939_GetStatus(&nSA);

                    if(nJ1939Status == RS_STATUS_ADRESSCLAIMSUCCESSFUL)
                    {
                        // Transmit the sample data
                        COM1939_Transmit(6, PGN_SAMPLE_TX, nSA, DEST_ADDR_SAMPLE, &SampleTxData[0], 8);
                        printf("Transmitted PGN %ld to node #%d:\n\r", (long)PGN_SAMPLE_TX, DEST_ADDR_SAMPLE);
                        for(nIndex = 0; nIndex < 8; nIndex++)
                            printf("0x%x ", SampleTxData[nIndex]);
                        printf("\n\r");

                    }// end if
                    else
                    {
                        // Print an error message
                        printf("Unable to Transmit - Reason: ");

                        switch(nJ1939Status)
                        {
                            case RS_STATUS_NONE:
                                printf("No J1939 Address Claimed.\n\r");
                                break;

                            case RS_STATUS_ADDRESSCLAIMINPROGRESS:
                                printf("Address Claim in Progress.\n\r");
                                break;

                            case RS_STATUS_ADDRESSCLAIMFAILED:
                                printf("Address Claim Failed.\n\r");
                                break;

                            case RS_STATUS_LISTENONLYMODE:
                                printf("Listen-Only Mode.\n\r");
                                break;

                        }// end switch

                    }// end else

                    break;

                case 'v':       // Get version strings
                    COM1939_GetVersion(&sSWVersion[0], &sHWVersion[0]);
                    printf(" SW: %s HW: %s\n\r", sSWVersion, sHWVersion);
                    break;

                case 'x':       // Exit the program
                    COM1939_Terminate();
                    bExit = true;
                    break;

            }// end switch

        }// end if
/////////////////////////////////////////////////////////////
/*	else{
//                    bAllowAllPGNs = !bAllowAllPGNs;
//                    if(bAllowAllPGNs == true)
                        COM1939_AddFilter(PGN_ALLOW_ALL);
//                    else
  //                      COM1939_DelFilter(PGN_ALLOW_ALL);
	}
*/
//////////////////////////////////////////////////////////////////
        if(bExit == true)
            break;

    }// end while

    return 0;

}// end main

//-FUNCTION-----------------------------------------------------------------
// Routine     : kbhit
// Description : Checks for keyboard input and fetches the code
// Returncode  : OK/ERROR
// -------------------------------------------------------------------------
int kbhit(char* key)
{
    struct termios oldt;
    struct termios newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();
    *key = (char)ch;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        //ungetc(ch, stdin);
        return ERROR;
    }

    return OK;

}// end kbhit
