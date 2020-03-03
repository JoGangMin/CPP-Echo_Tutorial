#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMassage(string message) {
	cout << "[오류발생]" << message << "\n";
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
	if (WSAStartup(MAKEWORD(2,2), &wsaData )!=0){ //winsock 추가
		ShowErrorMassage("WSAStartup()");
	}
	
	//soket()
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);// tcp 소켓 생성

	if (serverSocket == INVALID_SOCKET) {
		ShowErrorMassage("socket()");
	}

	//바인드
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; //IPV4 채계를 사용한다.
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4바이트 정수를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort); //2바이트 정수를 네트워크 형식으로
	if (bind(serverSocket,(SOCKADDR*)&serverAddress, sizeof(serverAddress))==SOCKET_ERROR) {
		ShowErrorMassage("bond()");
		cout << "[현제 상태] listen() \n";
	 }
	
	//listen()
	if (listen(serverSocket, 5) == SOCKET_ERROR) // 클라이언트가 5개 까지 접속하는것으로 설정
		ShowErrorMassage("lIsten()");
	cout << "[현제 상태] listen()\n";

	//accept()
	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMassage("accept()");
	
	//recv()
	while (1){ //클라이언트의 메시지를 받아서 그대로 다시 전달합니다.
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[클라이언트 메시지]" << received << "\n";
		cout << "[메시지 전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0){
			send(clientSocket, received, sizeof(received) - 1, 0); //엔터로 인한 공백문자 제거
			cout << "[서버 종료]";
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