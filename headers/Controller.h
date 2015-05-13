/* 
 * File:   Controller.h
 * Author: mfr
 *
 * Created on 28 de Janeiro de 2015, 15:12
 */

#include "Device.h"
#include "Channel.h"
class Device;
 
class Controller {
private:
    static Controller* instance;
    list<Device*> listDevice;
    list<Device*> listDeviceIn;
    list<Device*> listDeviceOut;
    int countDevices = 0;
    int countDevicesOut = 0;
protected:
    Controller();
public:
    
    static Controller* getInstance();
    void addDevice(Device* Dev);
    void addDeviceIn(Device* Dev);
    void addDeviceOut(Device* Dev);
 
    Device* getDevice(int);
    Device* getDeviceIn(int);
    Device* getDeviceOut(int);
    list<Device*> getAllDevices();
    Channel* getOutgoingChannelFree();    
    int getCountDevices();
    int getCountDevicesOut();
    
    bool hasDevice (int);
    ~Controller();
    

};