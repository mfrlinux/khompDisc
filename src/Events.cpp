#include "../headers/Channel.h"
#pragma GCC diagnostic ignored "-fpermissive"

char *params;
int cont = 0;

stt_code Kstdcall EventHandler(int32 channel, K3L_EVENT *Event) {
    int32 dev = Event->DeviceId;
    if (!Controller::getInstance()->hasDevice(dev)) {
        return;
    }
    Channel * chan = Controller::getInstance()->getDevice(dev)->getChannel(channel);

    switch (Event->Code) {
        case EV_SEIZURE_START:

            break;
        case EV_SEIZE_FAIL:
            break;
        case EV_CALL_SUCCESS:
            break;
        case EV_NEW_CALL:
            if (chan->isGSM()) {
                chan->sendPreconnect();
                chan->play();
            }
            
            if (chan->isSIP()) {
                chan->sendPreconnect();
                chan->play();
                //chan->connect(NULL);
            }

            if (chan->isFXO())
                chan->connect(NULL);

            if (chan->isISDN()) {
             //   chan->sendRingback(NULL);
                chan->sendPreconnect();
                chan->play();
                //chan->connect(NULL);
            }
            if (chan->isR2()) {
                chan->sendRingback(NULL);
                chan->sendPreconnect();
                chan->play();
                //chan->connect(NULL);
            }
            break;
        case EV_CONNECT:
            if (chan->getId() <= 5 || (chan->getId() >= 30 && chan->getId() <= 35) || (chan->getId() >= 50 && chan->getId() <= 59)  || (chan->getId() >= 60 && chan->getId() <= 65) || (chan->getId() >= 80 && chan->getId() <= 85)
                    || (chan->getId() >= 90 && chan->getId() <= 95) || (chan->getId() >= 145 && chan->getId() <= 150) || (chan->getId() >= 1000 && chan->getId() <= 1010) || (chan->getId() >= 4990 && chan->getId() <= 5000))
            {
                chan->record();
            }
              chan->play();
              totalChamEntrantes++;

            break;
        case EV_DTMF_DETECTED:
            break;
        case EV_DTMF_SEND_FINISH:
            break;
        case EV_SIP_REGISTER:
            char toHost[15];
            char toPortK3l[5];
            char toPort[6];
            char toIP[15];
            k3lGetEventParam(Event, "to_host", toHost, 15);
            k3lGetEventParam(Event, "to_port", toPortK3l, 6);
            k3lGetEventParam(Event, "contact_port", toPort, 6);
            k3lGetEventParam(Event, "orig_via_addr", toIP, 15);
            chan = Controller::getInstance()->getDevice(dev)->getChannel(cont++);
            if (cont == 20) {
                cont = 0;
            }            
            printf("%s %s\n", toPort, toIP);
            chan->setToPort(toPort, toIP);
            chan->registerSip(toHost, toPortK3l);
            break;
        case EV_END_OF_STREAM:
           if (chan->isIncoming()) {                
                chan->disconnect(NULL);                
            }
            break;
        case EV_DISCONNECT:            
            if (Event->AddInfo != 0) {
                totalDisc++;
            } else {
                totalChamComp++;
            }
            break;
        case EV_CHANNEL_FREE:
            chan->stopRecord();
            chan->stopPlay();
            break;
        case EV_CALL_FAIL:
            totalChamFalhas++;
            break;
    }
}
