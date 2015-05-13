/* 
 * File:   Channel.h
 * Author: mfr
 *
 * Created on 27 de Janeiro de 2015, 16:12
 */
#include <k3l.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#ifndef CHANNEL_H
#define	CHANNEL_H

enum cadenceType {
    BUSY,
    FASTBUSY,
    RINGBACK,
    DIALTONE
};

class Channel {
private:
    K3L_CHANNEL_CONFIG cfg;
    K3L_CHANNEL_STATUS stt;
   
    
    int id;
    int devId;
    KSignaling signaling;
    KCallStatus status;
    K3L_COMMAND cmd;
    cadenceType cadence;
    char dir[255];
    char params [ 255 ];
    Channel *conference;
    bool isConference;
    char toPort[6];
    char toIP[15];
    int outgoingCalls;
    int incomingCalls;
    int inbound;
    int outbound;
    int outCompleted;
    int OutFailed;
public:
    
    Channel();
    Channel(int devId, int chanId);    
  
    KCallStatus getStatus() {
        setDeviceStatus();
        status = stt.CallStatus;
        return status;
    }
    
    
    KSignaling getSignaling() ;
    
   
    
    int getId() {
        return id;
    }
    int getDevId () {
        return devId;
    }
    int getSerial();
    
    K3L_CHANNEL_CONFIG& getChannelConfig() ;
    //Estado
    bool isFree();
    bool isIncoming();
    bool isOutgoing();
   //Sinalizações
    bool inactive();
    bool isR2();
    bool isISDN();
    bool isPassiveISDN();
    bool isGSM();
    bool isSIP();
    bool isFXO();
    bool isFXS();
    
    void setToPort(char*, char*);
    char getToPort();
    char * getOrigNumber();
    int setDeviceStatus(){
        k3lGetDeviceStatus(devId, ksoChannel + id, &stt, sizeof(stt));
    }
      
   //Info Chan    
    int getOutgoingCalls();
    int getIncomingCalls();
    int getInbound();
    int getOutbound();
    int getOutCompleted();
    int getOutFailed();
    int getRemoteDisc();
    int getLocalDisc();
    
    //Comandos
    void makeCall ();
    void registerSip(char*, char*);
    void enableAnswerinfo (void);
    void sendDTMF(char*);
    void sendSipDTMF(char*);
    void sendRingback(int);
    void connect(int);
    void disconnect(int cause = NULL);
    void sendCadence (cadenceType);
    void sendPlayA();
    void sendPlayB();
    void play();
    void stopPlay();
    void record();
    void stopRecord();
    void sendSwitch();
    void sendClearSwitch();
    void sendEnableAnswerInfo();
    void sendDisableAnswerInfo();
    void sendEnableHmpDetection();
    void sendDisableHmpDetection();
    void sendPreconnect();
    int sendCommand(const int, char*);
    void setConference(Channel* conference);
    Channel* getConference();
  
    
    
};

#endif	/* CHANNEL_H */
