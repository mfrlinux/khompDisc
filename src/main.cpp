/*ksigInactive        = 0,
  ksigR2Digital       = 1,
  ksigContinuousEM    = 2,
  ksigPulsedEM        = 3,
  ksigUserR2Digital   = 4,
  ksigAnalog          = 5,
  ksigOpenCAS         = 6,
  ksigOpenR2          = 7,
  ksigSIP             = 8,
  ksigOpenCCS         = 9,
  ksigPRI_EndPoint    = 10,
  ksigAnalogTerminal  = 11,
  ksigPRI_Network     = 12,
  ksigPRI_Passive     = 13,
  ksigLineSide        = 14,
  ksigCAS_EL7         = 15,
  ksigGSM             = 16,
  ksigE1LC            = 17,
  ksigISUP            = 18,
  ksigISUPPassive     = 19,

  kcsFree     = 0x00,                // Channel is free for use
  kcsIncoming = 0x01,                // Channel in incoming call
  kcsOutgoing = 0x02,                // Channel in outgoing call
  kcsFail     = 0x04                 // Channel fail
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <k3l.h>
#include <k3lVersion.h>
#include <k3lAudio.h>
#include <KTypeDefs.h>
#include <KDefs.h>
#include <KStats.h>
#include <cstdlib>
#include <string>
#include <curses.h>



//Variables Globals
int dev;
int channel = 0;
int aux = 1;
int aux2 = 0;
int aux3 = 0;
int qtdChannels = 1;
int channels = 0;
int i = 0;
int totalChamEntrantes = 0;
int totalChamSaida = 0;
int totalChamFalhas = 0;
int totalChamComp = 0;
int totalDisc = 0;


K3L_DEVICE_CONFIG Cfg;
K3L_COMMAND cmd;
K3L_CHANNEL_CONFIG chanConfig;

//Include
#include "../headers/StartAPI.h"
#include "../headers/Controller.h"
#include "../headers/Gateway.h"
#include "../src/Events.cpp"

using namespace std;

int DevOrigem[] = {};
int DevDestino[] = {};

void shooting(int bullets) {
    while (bullets != 0) {
        for (int i = 0; i < Controller::getInstance()->getDevice(DevDestino[0])->getChannelCount(); i++) {
            //        for (int i = 0; i < 30; i++) {
            Controller::getInstance()->getDevice(DevOrigem[0])->getChannel(i)->makeCall();
        }

        bullets--;
        sleep(70);
    }
}

int main(int argc, char** argv) {

    FILE * pFile;
    char mystring [100];
    char calls;
    char option[100];
    int c;
    pFile = fopen("devices.cfg", "r");

    if (pFile == NULL) perror("Error opening file");
    else {
        while (fscanf(pFile, "%s %s", option, mystring) == 2) {
            //aux2 = 0;
            if (!feof(pFile)) {
                if (strcmp(option, "Entrada:") == 0) {
                    char DeviceEntrada[100];
                    strcpy(DeviceEntrada, mystring);
                    char* chars_array = strtok(DeviceEntrada, ",");
                    while (chars_array) {
                        std::cout << chars_array << '\n';
                        DevOrigem[aux2] = atoi(chars_array);

                        chars_array = strtok(NULL, ",");
                        printf("Add Origem : %d\n", DevOrigem[aux2]);
                        aux2++;
                    }
                }
                if (strcmp(option, "Saida:") == 0) {
                    //aux3 = 0;
                    char DeviceSaida[100];
                    strcpy(DeviceSaida, mystring);
                    char *chars_array = NULL;
                    chars_array = strtok(DeviceSaida, ",");

                    while (chars_array) {
                        std::cout << chars_array << '\n';
                        DevDestino[aux3] = atoi(chars_array);
                        chars_array = strtok(NULL, ",");
                        printf("Add Destino : %d\n", DevDestino[aux3]);
                        aux3++;
                    }
                }
            }
        }
        fclose(pFile);
    }
    StartAPI(3, 2, 0);
    k3lSetGlobalParam(klpReportHeaderExtensions, TRUE);
    //   for (int i = 0; i <= ((int) (&DevOrigem + 1)-(int) DevOrigem) / (sizeof (DevOrigem[0])); i++) {
    //    for (int i = 0; i < 2; i++) { 
    printf("AuX2: %d\n", aux2);
    printf("AuX3: %d\n", aux3);
    int i = 0;
    while (i < aux2) {
        Device * Dev = new Device(DevOrigem[i]);
        Controller::getInstance()->addDeviceIn(Dev);
        printf("%d - Device Origem ADD: %d\n", DevOrigem[i], Controller::getInstance()->getDeviceIn(DevOrigem[i])->getDevId());
        i++;
    }
    //   for (int i = 0; i <= ((int) (&DevDestino + 1)-(int) DevDestino) / (sizeof (DevDestino[0])); i++) {
    int x = 0;
    while (x < aux3) {
        Device * Dev = new Device(DevDestino[x]);
        Controller::getInstance()->addDeviceOut(Dev);
        printf("%d - Device Destino ADD: %d\n", DevDestino[x], Controller::getInstance()->getDeviceOut(DevDestino[x])->getDevId());
        x++;
    }
    int opt;
    puts(" 1) Gateway On \n 2) Dialing On \n *) Exit");
    fflush(stdin);
    scanf("%i", &opt);
    fflush(stdin);
    if (opt == 1) {
        printf("%i", opt);
        Gateway *gw = Gateway::getInstance();
        gw->registerEventHandler();
    }
    if (opt == 2) {
        k3lRegisterEventHandler(EventHandler);
        printf("%i", opt);
        int call = 0;
        call = atoi(argv[1]);
        int maxCall = 0;
        int y = 0;
        int j=0;
        int chama=1;
        maxCall = atoi(argv[2]);
        int devCall = 0;
        while (call != 0) {          
            for (j = 1; j <= maxCall; j++) {
                if (chama == 119) {
                    devCall++;
                    chama=1;
                    printf("%d\n", devCall);
                }
                if (Controller::getInstance()->getDevice(DevDestino[devCall]) != NULL) {
                    
                    if (Controller::getInstance()->getDevice(DevDestino[devCall])->getChannel((Controller::getInstance()->getDevice(DevDestino[devCall])->getChannelCount() - chama))->isFree())
                        sleep(30);
                        Controller::getInstance()->getDevice(DevDestino[devCall])->getChannel((Controller::getInstance()->getDevice(DevDestino[devCall])->getChannelCount() - chama))->makeCall();
                    totalChamSaida++;
                    chama++;
                    
                }
            }
            call--;
            sleep(50);
        }
    }
    char dig[255];
    puts("Digite algo e dê enter para finalizar");
    fflush(stdin);
    scanf("%s", &dig);

    fflush(stdin);
    printf("Calls Completed: %d\n", totalChamComp);
    printf("Call Fail: %d\n", totalChamFalhas);
    printf("Inbound Call: %d\n", totalChamEntrantes);
    printf("Outbound Call: %d\n", totalChamSaida);
    printf("Cause Diconnect > 0: %d\n", totalDisc);
    puts("finalizando...");

    list<Device*> listoff = Controller::getInstance()->getAllDevices();
    for (list<Device*>::iterator it = listoff.begin(); it != listoff.end(); it++) {
        if ((*it) != NULL) {
            (*it)->disconnectAll();
        }
    }
}