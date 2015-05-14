#include <stdio.h>
#include <string.h>

#include "../headers/Channel.h"
char numeroA[] = "1000"; // numero de origem
char numeroB[] = "1000"; // numero de destino
char net[15];

Channel::Channel(int devId, int chanId) {
    getcwd(dir, sizeof (dir));
    this->devId = devId;
    this->id = chanId;
};

int Channel::sendCommand(const int command, char params[255]) {

    cmd.Cmd = command;
    cmd.Object = this->getId();
    cmd.Params = params;
    int ret = k3lSendCommand(this->getDevId(), &cmd);
    return ret;
}

void Channel::setConference(Channel *conference) {
    this->conference = conference;
}

Channel *Channel::getConference() {
    return conference;
}

int Channel::getIncomingCalls() {
    uint32 calls = 0;
    if (k3lGetChannelStats(this->devId, this->id, kcsiInbound, &calls) != ksSuccess)
        return false;
    return calls;
}

int Channel::getOutgoingCalls() {
    uint32 calls = 0;
    if (k3lGetChannelStats(this->devId, this->id, kcsiOutCompleted, &calls) != ksSuccess)
        return false;
    return calls;
}

int Channel::getInbound() {
    uint32 calls = 0;
    if (k3lGetChannelStats(this->devId, this->id, kcsiInbound, &calls) != ksSuccess)
        printf("ErrGetInbound");
    return calls;
}

int Channel::getOutbound() {
    uint32 calls = 0;
    if (k3lGetChannelStats(this->devId, this->id, kcsiOutbound, &calls) != ksSuccess)
        printf("ErrGetOutbound");
    return calls;
}

int Channel::getOutCompleted() {
    uint32 calls = 0;
    if (k3lGetChannelStats(this->devId, this->id, kcsiOutCompleted, &calls) != ksSuccess)
        printf("ErrGetOutCompleted");
    return calls;
}

int Channel::getOutFailed() {
    uint32 calls = 0;
    if (k3lGetChannelStats(this->devId, this->id, kcsiOutFailed, &calls) != ksSuccess)
        printf("ErrGetFailed");
    return calls;
}

int Channel::getRemoteDisc() {
    uint32 calls = 0;
    if (k3lGetChannelStats(this->devId, this->id, kcsiRemoteDisc, &calls) != ksSuccess)
        printf("ErrRemoteDisc");
    return calls;
}

int Channel::getLocalDisc() {
    uint32 calls = 0;
    if (k3lGetChannelStats(this->devId, this->id, kcsiLocalDisc, &calls) != ksSuccess)
        printf("ErrLocalDisc");
    return calls;
}

//
//COMANDOS
//

void Channel::makeCall() {

    enum {
        kg2BrOrdinary = 1, // Subscriber without priority.
        kg2BrPriority = 2, // Subscriber with priority.
        kg2BrMaintenance = 3, // Maintenance equipment.
        kg2BrLocalPayPhone = 4, // Local pay phone.
        kg2BrTrunkOperator = 5, // Trunk operator.
        kg2BrDataTrans = 6, // Data transmission.
        kg2BrNonLocalPayPhone = 7, // Non-local pay phone.
        kg2BrCollectCall = 8, // Collect call.
        kg2BrOrdinaryInter = 9, // International ordinary subscriber.
        kg2BrTransfered = 10, // Transfered call.

    };
    int categoria = kg2BrOrdinary;
    if (isR2()) {
        sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" r2_categ_a=\"%i\"", numeroB, numeroA, categoria);
    }
    if (isISDN()) {
        sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" ", numeroB, numeroA);
    }
    if (isFXO()) {
        sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" ", numeroB, numeroA);
    }
    if (isFXS()) {
        int ringon = 1000;
        int ringoff = 4000;
        sprintf(params, "ring_on=\"%i\" ring_off=\"%i\"", ringon, ringoff);
    }
    if (isSIP()) {
        //	sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" network_dest_addr=\"%s\" voip_codecs=0,1 sip_transport_type=3 voip_audio_transport_type=3 sip_ice_stun_server=\"stun4.l.google.com:19302\" voip_rtcp_mux=\"10.5.29.50\"", numeroB, numeroA, toIP);
        //		sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" network_dest_addr=\"%s\" voip_codecs=0,1 sip_transport_type=3 voip_audio_transport_type=3 sip_stun_server=\"stun4.l.google.com:19302\" voip_rtcp_mux=\"10.5.29.50\"", numeroB, numeroA, toIP);
        //
        sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" network_dest_addr=\"10.5.3.21\" voip_codecs=0,1 sip_transport_type=0 voip_audio_transport_type=0", numeroB, this->getOrigNumber());
        //sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" network_dest_addr=\"10.5.29.55\" voip_codecs=0,1 sip_transport_type=0 voip_audio_transport_type=0", numeroB, this->getOrigNumber());
    }
    sendCommand(CM_MAKE_CALL, params);
}

void Channel::registerSip(char toHost[15], char toPort[5]) {
    sprintf(params, "status_code=200 to_user=\"%s\" to_host=\"%s\" to_port=\"%s\" expires=600", numeroA, toHost, toPort);
    sendCommand(CM_SIP_REGISTER_RESPONSE, params);
}

void Channel::enableAnswerinfo() {
    sendCommand(CM_ENABLE_CALL_ANSWER_INFO, NULL);
}

void Channel::sendDTMF(char dtmf[20]) {
    sendCommand(CM_DIAL_DTMF, dtmf);
}

void Channel::sendSipDTMF(char dtmf[20]) {
    sprintf(params, "type=\"0\" dtmf=\"%s\"", dtmf);
    sendCommand(CM_SIP_DIAL_DTMF, params);
}

void Channel::sendRingback(int cause) {

    if (cause == NULL) {
        sendCommand(CM_RINGBACK, NULL);
        return;
    }
    if (this->isISDN()) {
        sprintf(params, "isdn_cause=\"%d\"", cause);
        sendCommand(CM_RINGBACK, params);
        return;
    }
    if (this->isR2()) {
        sprintf(params, "r2_cond_b=\"%d\"", cause);
        sendCommand(CM_RINGBACK, params);
        return;
    }

    if (this->isSIP()) {
        sprintf(params, "sip_prov_resp=\"%d\"", cause);
        sendCommand(CM_RINGBACK, params);
        return;
    }
}

void Channel::connect(int param) {
    if (param != NULL) {
        if (isSIP()) {
            sprintf(params, "voip_codecs=\"%d\"", param);
            sendCommand(CM_CONNECT, params);
            return;
        }
    } else {
        sendCommand(CM_CONNECT, NULL);
    }
}

/**
 * Recebe inteiro contendo número da causa de desconexao.
 * Recebe parametro contendo a causa da desconexao de acordo com a sinalização.
 * 
 * @param cause
 */
void Channel::disconnect(int cause) {
    if (cause == NULL) {
        sendCommand(CM_DISCONNECT, NULL);
        return;
    }

    if (this->isR2())
        sprintf(params, "r2_cond_b=\"%d\"", cause);
    sendCommand(CM_DISCONNECT, params);
}

void Channel::sendCadence(cadenceType cadence) {


    //type: ringback
    switch (cadence) {

            //ringback
        case RINGBACK:
            sprintf(params, "cadence_times=\"1000,100, 200,4000\" mixer_track=\"0\" ");
            break;
            //busy
        case BUSY:
            sprintf(params, "cadence_times=\"250, 250\" mixer_track=\"0\" ");
            break;
            //dialtone
        case DIALTONE:
            sprintf(params, "cadence_times=\"1000, 90\" mixer_track=\"0\" ");
            break;
            //fastbusy
        case FASTBUSY:
            sprintf(params, "cadence_times=\"125, 125\" mixer_track=\"0\" ");
            break;
    }
    sendCommand(CM_START_CADENCE, params);
}

void Channel::sendPlayA() {
//    sprintf(params, "%s/acobrar/00-acobrar.wav", dir);
    sprintf(params, "%s/betinhoPCM16.wav",dir);
  

    //sprintf(params, "%s/SoundA.kwf", dir);
    sendCommand(CM_PLAY_FROM_FILE, params);
}

void Channel::sendPlayB() {
    //sprintf(params, "%s/AloMarioA.kwf",dir);    
    sprintf(params, "%s/audiosLuciano/caixapostal300hz.wav",dir);    
    //sprintf(params, "%s/acobrar/00-acobrar.wav", dir);
    sendCommand(CM_PLAY_FROM_FILE, params);
}

void Channel::play() {
    if (isOutgoing()) {
        sendPlayA();
        return;
    }
    if (isIncoming()) {
        sendPlayB();
    }
}

void Channel::stopPlay() {
    sendCommand(CM_STOP_PLAY, NULL);
}

void Channel::record() {
    time_t now = time(0);
    struct tm tstruct;
    char date[80];
    tstruct = *localtime(&now);
    strftime(date, sizeof (date), "-%Y-%m-%d.%X", &tstruct);
    char arch[ 255 ];
    sprintf(arch, "%s/Record/Device-%i-Channel-%i%s.wav", dir, this->getDevId(), this->getId(), date);
    sprintf(params, "file_name=\"%s\" codec=\"%d\" append=\"false\" stereo=\"true\"", arch, kci8kHzALAW);
    sendCommand(CM_RECORD_TO_FILE_EX, params);
}

void Channel::stopRecord() {
    sendCommand(CM_STOP_RECORD, NULL);
}

void Channel::sendSwitch() {
    sprintf(params, "device=\"%i\" channel=\"%i\" enable=\"true\"", getConference()->getDevId(), getConference()->getId());


    sendCommand(CM_SWITCH, params);
    return;
}

void Channel::sendClearSwitch() {
    sendCommand(CM_CLEAR_SWITCH, NULL);
    setConference(NULL);
}

void Channel::sendEnableAnswerInfo() {
    sendCommand(CM_ENABLE_CALL_ANSWER_INFO, NULL);
}

void Channel::sendDisableAnswerInfo() {
    sendCommand(CM_DISABLE_CALL_ANSWER_INFO, NULL);
}

void Channel::sendEnableHmpDetection() {
    sendCommand(CM_HMP_ENABLE_DETECTION, NULL);
}

void Channel::sendDisableHmpDetection() {
    sendCommand(CM_HMP_DISABLE_DETECTION, NULL);
}

void Channel::sendPreconnect() {
    sendCommand(CM_PRE_CONNECT, NULL);
}

void Channel::sendHMPAnalyticsCommand() {
    sendCommand(CM_ENABLE_HMP_ANALYTICS , NULL);
    this->sendEnableHmpDetection();
}


//
//Sinalização
//

KSignaling Channel::getSignaling() {
    return getChannelConfig().Signaling;
}

bool Channel::inactive() {
    if (getSignaling() == ksigInactive)
        return true;
    return false;
}

bool Channel::isR2() {
    if (getSignaling() == ksigR2Digital)
        return true;
    return false;
}

bool Channel::isISDN() {
    if (getSignaling() == ksigPRI_EndPoint || getSignaling() == ksigPRI_Network)
        return true;
    return false;
}

bool Channel::isPassiveISDN() {
    if (getSignaling() == ksigPRI_Passive)
        return true;
    return false;
}

bool Channel::isGSM() {
    if (getSignaling() == ksigGSM)
        return true;
    return false;
}

bool Channel::isSIP() {
    if (getSignaling() == ksigSIP)
        return true;
    return false;
}

bool Channel::isFXO() {
    if (getSignaling() == ksigAnalog)
        return true;
    return false;
}

bool Channel::isFXS() {
    if (getSignaling() == ksigAnalogTerminal)
        return true;
    return false;
}

bool Channel::isFree() {

    if (getStatus() == kcsFree)
        return true;
    return false;
}

bool Channel::isIncoming() {
    if (getStatus() == kcsIncoming) {
        return true;
    }
    return false;
}

bool Channel::isOutgoing() {

    if (getStatus() == kcsOutgoing) {
        return true;
    }
    return false;
}

void Channel::setToPort(char * Port, char *IP) {
    memcpy(toPort, Port, sizeof ( toPort));
    memcpy(toIP, IP, sizeof ( toIP));

    strcat(toIP, ":");
    strcat(toIP, toPort);
    printf("%s\n", toIP);
}

char Channel::getToPort() {
    return toPort;
}

char* Channel::getOrigNumber() {
    char * orig;
    orig = new char [255];
    sprintf(orig, "%d%d", this->getSerial(), this->getId());
    return orig;
}

int Channel::getSerial() {
    K3L_DEVICE_CONFIG cfg;
    k3lGetDeviceConfig(this->getDevId(), ksoDevice + this->getId(), &cfg, sizeof (cfg));
    return cfg.SerialNumber;
}

//API

K3L_CHANNEL_CONFIG& Channel::getChannelConfig() {
    k3lGetDeviceConfig(this->getDevId(), ksoChannel + this->getId(), &cfg, sizeof (cfg));
    return cfg;
}
