
#include "../headers/Device.h"
#include <stdio.h>

Device::Device(int id) {

    devId = id;
    populateChannels();


    //channel = new Channel[getChannelCount()]();

    /*    for( int i = 0 ;i < getChannelCount() ; i++){
            channel[i] = new Channel( devId,  i);
            printf("%d\n",channel[i]->getId());
        }
     */

}

void Device::populateChannels() {
    int channel_numbers = getChannelCount();
    for (int i = 0; i < channel_numbers; i++) {
        channel.push_back(new Channel(devId, i));
    }



}

Channel* Device::getChannel(int index) {
    for (list<Channel*>::iterator it = channel.begin(); it != channel.end(); it++) {
        if ((*it)->getId() == index)
            return *it;
    }
    return 0;
}

Channel* Device::getNextChannelFree() {
    list<Channel*>::iterator it = channel.end();
    while(it != channel.begin()){
        it--;
        
        if ((*it)->getStatus() == kcsFree && (*it) != NULL) {
            return *it;
        }
    }
 

    return NULL;
}

void Device::disconnectAll() {
    for (int i=0; i<getChannelCount();i++) {
        
        getChannel(i)->disconnect(NULL);
        getChannel(i)->stopRecord();
        
    }
}


/*
    
for (int i=0; i)
    if((*it)->getId() == index)
        return *it;
 */


