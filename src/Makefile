all: 
	g++ Client/CalcClient.cpp Client/Client.cpp libcommon/Libcommon.cpp -o client_exe -lpthread
	g++ Server/CalcServer.cpp Server/Server.cpp libcommon/Libcommon.cpp -o server_exe -lpthread
	
	
clear: 
	ipcrm -M 56
	rm client_exe
	rm server_exe