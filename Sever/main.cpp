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
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];
	
	//WInSock start up
	if (WSAStartup(MAKEWORD(2,2), &wsaData )!=0){ //winsock �߰�
		ShowErrorMassage("WSAStartup()");
	}
	
	//soket()
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);// tcp ���� ����

	if (serverSocket == INVALID_SOCKET) {
		ShowErrorMassage("socket()");
	}

	//���ε�
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; //IPV4 ä�踦 ����Ѵ�.
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort); //2����Ʈ ������ ��Ʈ��ũ ��������
	if (bind(serverSocket,(SOCKADDR*)&serverAddress, sizeof(serverAddress))==SOCKET_ERROR) {
		ShowErrorMassage("bond()");
		cout << "[���� ����] listen() \n";
	 }
	
	//listen()
	if (listen(serverSocket, 5) == SOCKET_ERROR) // Ŭ���̾�Ʈ�� 5�� ���� �����ϴ°����� ����
		ShowErrorMassage("lIsten()");
	cout << "[���� ����] listen()\n";

	//accept()
	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMassage("accept()");
	
	//recv()
	while (1){ //Ŭ���̾�Ʈ�� �޽����� �޾Ƽ� �״�� �ٽ� �����մϴ�.
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[Ŭ���̾�Ʈ �޽���]" << received << "\n";
		cout << "[�޽��� ����]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0){
			send(clientSocket, received, sizeof(received) - 1, 0); //���ͷ� ���� ���鹮�� ����
			cout << "[���� ����]";
			break;
		}
		//send()
		send(clientSocket, received, sizeof(received) -1, 0);
	}
	
	//closesocket()
	closesocket(clientSocket);
	closesocket(serverSocket);
	
	//WSACleanup()
	WSACleanup();

	return 0;
}