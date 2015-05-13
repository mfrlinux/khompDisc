/* 
 * File:   Gateway.h
 * Author: mfr
 *
 * Created on 30 de Janeiro de 2015, 09:54
 */
#include <k3l.h>
#ifndef GATEWAY_H
#define	GATEWAY_H

class Gateway {

    static stt_code Kstdcall GwEventHandler(int32 channel, K3L_EVENT *Event);
private:
    static Gateway* instance;
    Gateway();

public:    
    static Gateway* getInstance(){
        if (instance == 0) {
            instance = new Gateway();
        }
        return instance;
    }
    char getDest (K3L_EVENT *Event);
    bool createConference(int devId, int chanId);
    void registerEventHandler();

};


#endif	/* GATEWAY_H */

