void desconexaoFunc(int dev, int aux) {
	Disconect(dev, aux);
	StopRecord(dev, aux);
	if (chanConfig.Signaling == ksigAnalog) {
		Cadencia(dev, 0,2);
	}
	if (chanConfig.Signaling == ksigAnalogTerminal) {
		puts("Desligando FXS");
		PararPlay(dev, aux);
		Cadencia(dev, aux,2);
		sleep(5);
		PararCadencia(dev, aux);
	}
	totalChamEntrantes=totalChamEntrantes+GetChannelIncomingCalls(dev, aux);
	totalChamSaida=totalChamSaida+GetChannelOutCalls(dev, aux);
	totalChamComp=totalChamComp + GetChannelCompleteCalls(dev, aux);
	totalChamFalhas=totalChamFalhas + GetChannelFailCalls(dev, aux);
	puts("-----------------------------");
	puts("-----------------------------");
	printf("Total de chamadas Completadas no Sistema: %i \n", totalChamComp);
	printf("Total de chamadas falhadas: %i \n", totalChamFalhas);
	printf("Total de chamadas entrantes: %i \n", totalChamEntrantes);
	printf("Total de chamadas saintes: %i \n", totalChamSaida);
	puts("-----------------------------");
	puts("-----------------------------");
}
