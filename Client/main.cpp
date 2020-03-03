#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMassage(string message) {
	cout << "[�����߻�]" << message << "\n";
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	string sent;

	//WSAStartup()
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { //winsock �߰�
		ShowErrorMassage("WSAStartup()");
	}
	//socket()
	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // tcp ���� ����

	if (clientSocket == INVALID_SOCKET) {
		ShowErrorMassage("socket()");
	}
	//connect()
	memset(&serverAddress, 0, sizeof(serverAddress)); 
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // ���ڿ� IP�� ��Ʈ��ũ ����Ʈ �������� 
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		ShowErrorMassage("connect()");
	}
	cout<< "[���� ����] connect() \n";
	//send() //recv()
	while (1){// �ݺ������� ������ �޽����� �����ϰ� ���� �޽����� ���� �޽��ϴ�.
		cout << "[�޽��� ����]:";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		if (strcmp(received,"[exit]") == 0) {
			cout << "[���� ����]\n";
			break;
		}
		cout << "[���� �޽���]:" << received << "\n";
	}
	//closesocket()
	closesocket(clientSocket);
	//WSACleanup()
	WSACleanup();
	return 0;
}
