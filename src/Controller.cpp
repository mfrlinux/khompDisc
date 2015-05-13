/* 
 * File:   Controller.cpp
 * Author: mfr
 * 
 * Created on 28 de Janeiro de 2015, 15:12
 */

#include "../headers/Controller.h"

Controller* Controller::instance = 0;

Controller::Controller() {
}

int Controller::getCountDevicesOut() {
    return countDevicesOut;
}

Controller* Controller::getInstance() {
    if (instance == 0) {
        instance = new Controller();
    }
    return instance;

}

void Controller::addDevice(Device* Dev) {   
    if (countDevices == 0) {
        countDevices++;
        listDevice.push_back(Dev);
    } else {
        if (!hasDevice(Dev->getDevId())) {
                listDevice.push_back(Dev);
                countDevices++;
        }

    }
}

Device* Controller::getDevice(int index) {
    for (list<Device*>::iterator it = listDevice.begin(); it != listDevice.end(); it++) {
        if ((*it)->getDevId() == index) {
            return *it;
        }
    }
    return NULL;
}

void Controller::addDeviceIn(Device* Dev) {
    listDeviceIn.push_back(Dev);
    addDevice(Dev);
}

Device* Controller::getDeviceIn(int index) {
    for (list<Device*>::iterator it = listDeviceIn.begin(); it != listDeviceIn.end(); it++) {
        if ((*it)->getDevId() == index) {
            return *it;
        }

    }
    return NULL;
}

void Controller::addDeviceOut(Device* Dev) {
    listDeviceOut.push_back(Dev);
    addDevice(Dev);
    countDevicesOut++;
}

Device* Controller::getDeviceOut(int index) {

    for (list<Device*>::iterator it = listDeviceOut.begin(); it != listDeviceOut.end(); it++) {
        if ((*it)->getDevId() == index) {
            return *it;
        }

    }
    return NULL;
}

Channel* Controller::getOutgoingChannelFree() {
    for (list<Device*>::iterator it = listDeviceOut.begin(); it != listDeviceOut.end(); it++) {
        Channel* chan = (*it)->getNextChannelFree();
        if (chan != NULL) {
            return chan;
        } 
    }
    return NULL;
}

list<Device*> Controller::getAllDevices() {
    return listDevice;
}

bool Controller::hasDevice(int index) {
    if (getDevice(index) != NULL) {
        return true;
    }
    return false;
}

int Controller::getCountDevices() {
    return countDevices;
}