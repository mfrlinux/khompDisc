
/* WARNING _ CODE V1 C based*/
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#pragma GCC diagnostic ignored "-fpermissive"

void EnableAnswerInfo (int dev, int channel) {
	cmd.Cmd = CM_ENABLE_CALL_ANSWER_INFO;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(dev, &cmd);
}

void sendDTMF(int dev, int channel, char dtmf[20]) {
	cmd.Cmd = CM_DIAL_DTMF;
	cmd.Object = channel;
	cmd.Params = (byte*) dtmf;
	k3lSendCommand(dev, &cmd);
}

void Cadencia (int dev, int channel, int type) {
    //type: ringback
	char cadence[255];
	//ringback
	if (type == 1) {
		sprintf(cadence, "cadence_times=\"1000,100, 200,4000\" mixer_track=\"0\" ");
	}	
	//busy
	if (type == 2) {
		sprintf(cadence, "cadence_times=\"250, 250\" mixer_track=\"0\" ");
	}
	//dialtone
	if (type == 3) {
		sprintf(cadence, "cadence_times=\"1000, 90\" mixer_track=\"0\" ");
	}
	//fastbusy
	if (type == 4) {
		sprintf(cadence, "cadence_times=\"125, 125\" mixer_track=\"0\" ");
	}

	cmd.Cmd = CM_START_CADENCE;
	cmd.Object = channel;
	cmd.Params = cadence;
	k3lSendCommand(dev, &cmd);
}

void SendPlay (int32 dev, int32 channel) {
	cmd.Cmd = CM_PLAY_FROM_FILE;
	cmd.Object = channel;
	char params[ 255 ] = "/home/mfr/Documentos/DiscadorC/betinhoPCM16.wav";
	cmd.Params = params;
	k3lSendCommand(dev, &cmd);
}

void PararPlay(int32 dev, int32 channel) {
	cmd.Cmd = CM_STOP_PLAY;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(dev, &cmd);
}

void Record(int dev, int channel) {
	//Codecs Disponiveis:  kci8kHzALAW, kci8kHzGSM, kci8kHzADPCM, kci8kHzPCM e kci8kHzPCM16	

	time_t     now = time(0);
	struct tm  tstruct;
	char       date[80];
	tstruct = *localtime(&now);
	strftime(date, sizeof(date), "-%Y-%m-%d.%X", &tstruct);

	cmd.Cmd = CM_RECORD_TO_FILE_EX;
	cmd.Object = channel;        
	char arch[ 255 ];
	sbyte params[ 255 ];
	sprintf( params, "file_name=\"%s\" codec=\"%d\" append=\"false\" ", arch, kci8kHzALAW );
	cmd.Params = params;
	k3lSendCommand(dev, &cmd);
}

void StopRecord(int dev, int channel) {
	cmd.Cmd = CM_STOP_RECORD;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(dev, &cmd); 
}
void PararCadencia (int dev, int channel) {
	cmd.Cmd = CM_STOP_CADENCE;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(dev, &cmd);
}

void Seize(int dev) {
	char numeroA[] = "1234"; // numero de origem
	cmd.Cmd = CM_SEIZE;
	cmd.Object = 0;
	cmd.Params = (byte*) numeroA;	
	k3lSendCommand(dev, &cmd);
}

void Disconect(int dev, int32 channel) {
	cmd.Cmd = CM_DISCONNECT;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(dev, &cmd);
}

void ClearSwitch(int32 dev, int32 channel) {
	bool en = true;
	char params[ 255 ];
	sprintf(params,"use_dsp\"%d\"",en);
	cmd.Cmd = CM_CLEAR_SWITCH;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(dev, &cmd);
}

void Switch(int32 dev, int32 channel) {
	char params[ 255 ];
	bool enable = true;
	sprintf(params, "device=\"%i\" channel=\"%i\" enable=\"true\" use_dsp=\"true\"", dev, channel);
	cmd.Cmd = CM_SWITCH;
	cmd.Object = channel;
	cmd.Params = (byte*) params;
	k3lSendCommand(dev, &cmd);
}

void CallAnalyser(int32 dev, int32 channel) {
	cmd.Cmd = CM_ENABLE_CALL_ANSWER_INFO;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(dev, &cmd);
}

void Make_Call(int32 sinal, int32 dev, int32 channel) {
	char net[15] = "10.100.10.151";
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
	char numeroA[] = "1234"; // numero de origem
	char numeroB[] = "5678"; // numero de destino

	// cria a ''string'' que erá enviada como parâmetro
	char params[ 255 ];

	//R2 = 0 ; ISDN = 1 ; Analogia=2
	if (sinal == 0) {
		sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" r2_categ_a=\"%i\"", numeroB, numeroA, categoria);
	}
	if (sinal == 1) {
		sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" ", numeroB, numeroA);
	}
	if (sinal == 2) {
		PararCadencia(0,0);
		int ringon = 1000;
		int ringoff = 4000;
		sprintf(params, "ring_on=\"%i\" ring_off=\"%i\"", ringon, ringoff);
		char dtmf[25] = "1";
		sendDTMF(dev, channel, dtmf);
	}
	if (sinal == 3) {
		sprintf(params, "dest_addr=\"%s\" orig_addr=\"%s\" network_dest_addr=\"%s\" voip_codecs=0", numeroB, numeroA, net);

	}

	cmd.Cmd = CM_MAKE_CALL;
	cmd.Object = channel;
	cmd.Params = (byte*) params;
	//sendAnswerInfo(dev, channel);
	k3lSendCommand(dev, &cmd);
	/*Teste mandando cadendia busy logo apos make call*/
	//        sleep(19);
	//	Disconect(0);
}
void Connect(int dev, int channel) {
	cmd.Cmd = CM_CONNECT;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(&dev, &cmd);
}

void PreConnect(int dev, int channel) {
	cmd.Cmd = CM_PRE_CONNECT;
	cmd.Object = channel;
	cmd.Params = NULL;
	k3lSendCommand(&dev, &cmd);
}

void RingBack(int dev, int channel) {
	char str[255];
	cmd.Cmd = CM_RINGBACK;
	sprintf(str, "r2_categ_b=\"%d\" ", kgbLineFreeCharged );
	cmd.Object = channel;
	cmd.Params = ( byte* ) str;    //envia o comando de ringback,
	k3lSendCommand( dev, &cmd ); //aceitando a ligação
}

void LoopCall(int qtdChannels, int sig) {
	while (aux < qtdChannels) {     
		printf("Ligação no Canal: %i\n", aux);
		EnableAnswerInfo(dev, aux);
		Make_Call(sig, dev,  aux);
		Record(dev, aux);
		aux++;
	}
}
