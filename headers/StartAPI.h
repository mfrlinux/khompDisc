#pragma GCC diagnostic ignored "-fpermissive"



using namespace std;

void StartAPI(int aMajor, int aMinor, int aBuild) {
	sbyte *_result = k3lStart(aMajor, aMinor, aBuild);
        
	if (_result != NULL) {
        cout << _result << "\n";
	exit(-1);	
	} else {
            cout << "\n" << _result;
          
	    puts("Conectado");
	}
}

void StopAPI() {
    k3lStop();
}
int CountDevices() {
    return k3lGetDeviceCount();
}




bool GetFreeChannel(int &DevId, int &Channel) {
    for (int d = 0; d < k3lGetDeviceCount(); d++) {
        K3L_DEVICE_CONFIG Cfg;

        if (k3lGetDeviceConfig(d, ksoDevice + d, &Cfg, sizeof (Cfg)) != ksSuccess)
            return false;

        for (int c = 0; c < Cfg.ChannelCount; c++) {
            K3L_CHANNEL_STATUS Stt;

            if (k3lGetDeviceStatus(d, ksoChannel + c, &Stt, sizeof (Stt)) != ksSuccess)
                return false;

            if (Stt.CallStatus == kcsFree) {
                DevId = d;
                Channel = c;
                return true;
            }
        }
    }

    return false;
}


int GetChannelCount() {
    int Total = 0;

    for (int d = 0; d < k3lGetDeviceCount(); d++) {
        K3L_DEVICE_CONFIG Cfg;

        if (k3lGetDeviceConfig(d, ksoDevice + d, &Cfg, sizeof (Cfg)) != ksSuccess) {
            return -1;
        }

        Total += Cfg.ChannelCount;
    }

    return Total;
}

uint32 GetChannelIncomingCalls(int &DevId, int &Channel) {
    uint32 calls =0;
 
    if(k3lGetChannelStats(DevId,  Channel, kcsiInbound, &calls) != ksSuccess)
        return false;
 
    return calls;
}
uint32 GetChannelOutCalls(int &DevId, int &Channel) {
    uint32 calls =0;
 
    if(k3lGetChannelStats(DevId,  Channel, kcsiOutbound, &calls) != ksSuccess)
        return false;
 
    return calls;
}

uint32 GetChannelCompleteCalls(int &DevId, int &Channel) {
    uint32 calls =0;

    if(k3lGetChannelStats(DevId,  Channel, kcsiOutCompleted, &calls) != ksSuccess)
        return false;

    return calls;
}

uint32 GetChannelFailCalls(int &DevId, int &Channel) {
    uint32 calls =0;

    if(k3lGetChannelStats(DevId,  Channel, kcsiOutFailed, &calls) != ksSuccess)
        return false;

    return calls;
}


