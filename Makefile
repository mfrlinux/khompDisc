DIR=./src
all:
	g++ -c $(DIR)/Channel.cpp -o Channel.o -fpermissive
	
	g++ Channel.o -c $(DIR)/Device.cpp -o Device.o -fpermissive
	g++ Device.o -c $(DIR)/Controller.cpp -o Controller.o -fpermissive
	g++ Device.o Controller.o Channel.o -c $(DIR)/Gateway.cpp -o Gateway.o -lkl3client -fpermissive	
	g++ Device.o Controller.o Channel.o Gateway.o $(DIR)/main.cpp -lk3lclient -o disc -fpermissive
clean:
	rm disc