#include "../headers/Gateway.h"
#include "../headers/Controller.h"
#include <string.h>
#pragma GCC diagnostic ignored "-fpermissive"

Gateway* Gateway::instance = 0;

Gateway::Gateway() {
    registerEventHandler();
}

stt_code Kstdcall Gateway::GwEventHandler(int32 channel, K3L_EVENT *Event) {
    int32 dev = Event->DeviceId;
    Controller * control = Controller::getInstance();

    if (!control->hasDevice(dev)) {
        return;
    }
    switch (Event->Code) {
        case EV_SEIZURE_START:
            break;
        case EV_SEIZE_FAIL:
            break;
        case EV_CALL_SUCCESS:
            if (control->getDeviceOut(dev) == NULL)
                control->getDeviceOut(dev)->getChannel(channel)->getConference()->sendRingback(NULL);
            break;
        case EV_NEW_CALL:
            char numeroB[ KMAX_DIAL_NUMBER + 1 ];
            k3lGetEventParam(Event, "dest_addr", numeroB, sizeof ( numeroB));


            //if (strcmp(numeroB, "1000") == 0) {
            if (control->getDeviceIn(dev) != NULL) {
                if (Gateway::getInstance()->createConference(dev, channel)) {
                    if (control->getDeviceIn(dev)->getChannel(channel)->getConference()->isFree()) {
                        if (control->getDeviceIn(dev)->getChannel(channel)->getConference()->isR2()) {
                            control->getDeviceIn(dev)->getChannel(channel)->getConference()->sendEnableAnswerInfo();
                        }
                        if (control->getDeviceIn(dev)->getChannel(channel)->getConference()->isISDN()) {
                            control->getDeviceIn(dev)->getChannel(channel)->getConference()->sendEnableAnswerInfo();
                        }
                        if (control->getDeviceIn(dev)->getChannel(channel)->getConference()->isSIP()) {
                            control->getDeviceIn(dev)->getChannel(channel)->getConference()->sendEnableHmpDetection();
                            control->getDeviceIn(dev)->getChannel(channel)->getConference()->sendEnableAnswerInfo();

                        }
                        control->getDeviceIn(dev)->getChannel(channel)->getConference()->makeCall();
                        control->getDeviceIn(dev)->getChannel(channel)->getConference()->sendSwitch();
                        control->getDeviceIn(dev)->getChannel(channel)->sendSwitch();
                    }
                }
            }
            /*} else {
                puts("ROTA NÂO CONFIGURADA");                
            }*/

            break;
        case EV_CONNECT:
            if (control->getDeviceOut(dev) == NULL) {
                break;
            }
            if (control->getDeviceOut(dev)->getChannel(channel)->isOutgoing()) {
                if (control->getDeviceOut(dev)->getChannel(channel)->getConference() != NULL) {
                    control->getDeviceOut(dev)->getChannel(channel)->getConference()->connect(6);
                }
            }

            break;
        case EV_CALL_ANSWER_INFO:
            if (control->getDevice(dev) != NULL) {
                control->getDevice(dev)->getChannel(channel)->sendDisableAnswerInfo();
                if (control->getDevice(dev)->getChannel(channel)->isSIP()) {
                    control->getDevice(dev)->getChannel(channel)->sendDisableHmpDetection();
                }
            }
            break;
        case EV_DTMF_DETECTED:
            break;
        case EV_DTMF_SEND_FINISH:
            break;
        case EV_END_OF_STREAM:
            break;
        case EV_DISCONNECT:        
            if (control->getDeviceOut(dev) == NULL && control->getDeviceIn(dev) == NULL) {
                break;
            }
            if (control->getDeviceOut(dev) != NULL) {
                if (control->getDeviceOut(dev)->getChannel(channel)->isOutgoing()) {
                    if (control->getDeviceOut(dev)->getChannel(channel)->getConference() != NULL) {
                        control->getDeviceOut(dev)->getChannel(channel)->getConference()->disconnect(NULL);
                        break;
                    }
                }
            }
            if (control->getDeviceIn(dev) != NULL) {
                if (control->getDeviceIn(dev)->getChannel(channel)->isIncoming()) {
                    if (control->getDeviceIn(dev)->getChannel(channel)->getConference() != NULL) {
                        control->getDeviceIn(dev)->getChannel(channel)->getConference()->disconnect(NULL);
                    }
               
                }
            }    
            break;
        case EV_CHANNEL_FREE:
            if (control->getDevice(dev) != NULL) {
                control->getDevice(dev)->getChannel(channel)->sendClearSwitch();
            }
            break;
    }
}

char Gateway::getDest(K3L_EVENT *Event) {
    char numeroB[ KMAX_DIAL_NUMBER + 1 ];
    k3lGetEventParam(Event, "dest_addr", numeroB, sizeof ( numeroB));
    return numeroB;
}

bool Gateway::createConference(int devId, int chanId) {
    Controller * control = Controller::getInstance();
    Channel * chanDest;
    Channel * chanOrig;
    chanDest = control->getOutgoingChannelFree();

    if (chanDest == NULL) {
        control->getDeviceIn(devId)->getChannel(chanId)->sendRingback(403);
        return false;
    }

    control->getDeviceIn(devId)->getChannel(chanId)->setConference(control->getDeviceOut(chanDest->getDevId())->getChannel(chanDest->getId()));
    control->getDeviceOut(chanDest->getDevId())->getChannel(chanDest->getId())->setConference(control->getDeviceIn(devId)->getChannel(chanId));
    return true;
}

void Gateway::registerEventHandler() {
    k3lRegisterEventHandler(GwEventHandler);
}