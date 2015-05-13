/* 
 * File:   Device.h
 * Author: mfr
 *
 * Created on 27 de Janeiro de 2015, 14:43
 */
#include <k3l.h>
#include "Channel.h"
#include <list>



using namespace std;
#ifndef DEVICE_H
#define	DEVICE_H

class  Device {
private:
    K3L_DEVICE_CONFIG Cfg;
    int devId;
    int channelCount;    
    //Channel channel[];
    list<Channel*> channel;
       
public:
    Device();
    Device(int id);
    
    void populateChannels();
    
    int getChannelCount()  {
        setDeviceConfig();
        channelCount = Cfg.ChannelCount;
        return channelCount;
    }
    int getDevId()  {
        return devId;
    }
    int setDeviceConfig() {
        k3lGetDeviceConfig(devId, ksoDevice + devId, &Cfg, sizeof (Cfg));
    }
    Channel* getChannel(int);
    Channel* getNextChannelFree();
    void disconnectAll();

    

};

#endif	/* DEVICE_H */

