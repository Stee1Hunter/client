#define WIN32_LEAN_AND_MEAN

#include <iostream> 
#include <Windows.h> 
#include <WinSock2.h> 
#include <WS2tcpip.h> 
#include <Windows.h>  
#include <WinSock2.h>  
#include <WS2tcpip.h>  
using namespace std;

int main()
{
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult;
	SOCKET ClientSocket = INVALID_SOCKET;
	SetConsoleCP(1251);
	setlocale(LC_ALL, "Russian");


	const char* sendBuffer1 = "Hello from Client 1";
	const char* sendBuffer2 = "Hello from Client 2";
	char recvBuffer[512];


	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		cout << "WSAStartup failed" << endl;
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo("localhost", "788", &hints, &addrResult);
	if (result != 0) {
		cout << "getaddrinfo failed" << endl;
		return 1;
	}
	ClientSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (ClientSocket == INVALID_SOCKET) {
		cout << "socket creation with error" << endl;
		freeaddrinfo(addrResult);
		return 1;
	}
	result = connect(ClientSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cout << "Unable to connect server" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	result = send(ClientSocket, sendBuffer1, (int)strlen(sendBuffer1), MSG_DONTROUTE);
	if (result == SOCKET_ERROR) {
		cout << "send Failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	cout << "Sent " << result << " bytes" << endl;

	result = send(ClientSocket, sendBuffer2, (int)strlen(sendBuffer2), MSG_DONTROUTE);
	if (result == SOCKET_ERROR) {
		cout << "send Failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	cout << "Sent " << result << " bytes" << endl;

	result = shutdown(ClientSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
		cout << "shutdown error" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	do {
		ZeroMemory(recvBuffer, 512);
		result = recv(ClientSocket, recvBuffer, 512, 0);
		if (result > 0) {
			cout << "Получено " << result << " байт" << endl;
			cout << "Получено " << recvBuffer << endl;
		}
		else if (result == 0) {
			cout << "Connection closed" << endl;
		}
		else {
			cout << "Сообщение не получено" << endl;
		}
	} while (result > 0);
	closesocket(ClientSocket);
	freeaddrinfo(addrResult);
	WSACleanup();
}
