#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include "iostream"

using namespace std;
//making a comms class with code copied from the server 1.cpp and refactored
class Comms {
public:
	Comms();
	void errmsg(int e, char* buffer, int len);
	void connectSocket();
	void bindSocket();
	void acceptConnection();
	void sendBuffer(const char* buffer, int len, int flags);
	void receiveBuffer(char* buffer, int len, int flags);

protected:
	//Instead of acceptSocket & Service, it has been replaced with mySocket & myService
	SOCKET mySocket;
	sockaddr_in myService;
	int port = 55555;

};

/// <summary>
/// In the Comms constructor we create the Socket and initialise the service
/// </summary>
Comms::Comms() {
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		//replaced return with throw
		throw 1;
	}
	else {
		cout << "The Winsock dll found!" << endl;
		cout << "The status: " << wsaData.szSystemStatus << endl;
	}

	mySocket = INVALID_SOCKET;
	mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mySocket == INVALID_SOCKET) {
		//Instead of returing i am throwing an execption
		throw 2;
	}
	else {
		cout << "socket() is OK!" << endl;
	}

	myService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &myService.sin_addr.s_addr);
	myService.sin_port = htons(port);
}

/// <summary>
/// Connect the Socket to the service
/// </summary>
void Comms::connectSocket()
{
	if (connect(mySocket, (SOCKADDR*)&myService, sizeof(myService)) == SOCKET_ERROR) {
		throw 3;
	}
	else {
		cout << "Client: connect() is OK." << endl;
		cout << "Client: Can start sending and recieving data..." << endl;
	}
}

/// <summary>
/// Bind the Socket with a connection
/// </summary>
void Comms::bindSocket() {
	if (bind(mySocket, (SOCKADDR*)&myService, sizeof(myService)) == SOCKET_ERROR) {
		closesocket(mySocket);
		throw 4;
	}
	else {
		cout << "bind() is OK!" << endl;
	}
	if (listen(mySocket, 1) == SOCKET_ERROR)
		cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
	else
		cout << "listen() is OK, I'm waiting for connections..." << endl;
}

/// <summary>
/// Accepting a conncetion of the Socket
/// </summary>
void Comms::acceptConnection() {
	SOCKADDR_STORAGE from;
	int retval, fromlen;
	char servstr[NI_MAXSERV], hoststr[NI_MAXHOST];

	fromlen = sizeof(from);
	mySocket = accept(mySocket, (SOCKADDR*)&from, &fromlen);

	if (mySocket == INVALID_SOCKET) {
		throw 5;
	}
	char receiveBuffer[200];
	retval = getnameinfo((SOCKADDR*)&from,
		fromlen,
		hoststr,
		NI_MAXHOST,
		servstr,
		NI_MAXSERV,
		NI_NUMERICHOST | NI_NUMERICSERV);
	if (retval != 0) {
		throw 6;
	}

	cout << "Accepted connection from host " << hoststr << " and port " << servstr << endl;

	cout << "Accepted connection" << endl;

}

void Comms::sendBuffer(const char* buffer, int len, int flags) {
	int byteCount = send(mySocket, buffer, len, flags);
	if (byteCount == SOCKET_ERROR) {
		throw 7;
	}
}

void Comms::receiveBuffer(char* buffer, int len, int flags) {
	int byteCount = recv(mySocket, buffer, len, flags);
	if (byteCount < 0) {
		throw 8;
	}
}

/// <summary>
/// This function translates exceptions thrown by this class to error strings
/// which it copies into the parameter buffer, which should be of length len
/// </summary>
void Comms::errmsg(int e, char* buffer, int len) {
	switch (e) {
	case 1:strncpy(buffer, "The Winsock dll not found!", len);
		break;
	case 2:
		snprintf(buffer, len, "Error at socket(): %ld.", WSAGetLastError());
		break;
	case 3: strncpy(buffer, "Client: connect() - Failed to connect.", len);
		break;
	case 4: snprintf(buffer, len, "bind() failed: %ld.", WSAGetLastError());
		break;
	case 5: snprintf(buffer, len, "accept failed: %ld.", WSAGetLastError());
		break;
	case 6: strncpy(buffer, "getnameinfo failed", len);
		break;
	case 7: snprintf(buffer, len, "Server send error %ld.\n", WSAGetLastError());
		break;
	case 8: snprintf(buffer, len, "Client: error %ld.\n", WSAGetLastError());
		break;
	}
}