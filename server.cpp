
#include"server.h"
#include<Windows.h>
#include"server_error.h"
#include<assert.h>

using namespace std;

void log_data(char s[], int n) {
	for (int i = 0; i < n; i++) {
		putchar(s[i]);
	}
}

int server::setup() {
	WORD requested_version = MAKEWORD(2, 2);
	WSADATA res_struct;
	int res = WSAStartup(requested_version, &res_struct);

	if (res != 0) {
		wprintf(L"The WSA startup failed with error code %d\n", res);
		SERVER_ERROR_THROW(wstring(L"The WSA startup failed with error code") + to_wstring(WSAGetLastError()) + L'\n');
		return 1;
	}
	
	WORD resulting_version = res_struct.wVersion;
	if (requested_version != resulting_version) {
		wprintf(L"Could not find the requested version of Winsock.dll\n");
		SERVER_ERROR_THROW(wstring(L"Could not find the requested version of Winsock.dll\n"));
		return 1;
	}
	else {
		wprintf(L"The Winsock 2.2 dll was found okay\n");
	}

	return 0;
}

int server::initiate() {
	//Creating the main socket
	this->main_connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->main_connection == INVALID_SOCKET) {
		wprintf(L"Socket function failed with error = %d\n", WSAGetLastError());
		SERVER_ERROR_THROW(wstring(L"Socket function failed with error = ") + to_wstring(WSAGetLastError()) + L'\n');
		return 1;
	}
	wprintf(L"Main socket creation succeeded!\n");

	//Binding the main socket
	sockaddr_in adress_info;
	adress_info.sin_addr.s_addr = inet_addr("192.168.0.62");
	adress_info.sin_family = AF_INET;
	adress_info.sin_port = htons(27015);

	int res = bind(this->main_connection, (SOCKADDR*) & adress_info, sizeof(adress_info));
	if (res != 0) {
		SERVER_ERROR_THROW(wstring(L"The main socket failed to be bound with net error: ") + to_wstring(WSAGetLastError()) + L'\n');
		return 1;
	}
	else {
		wprintf(L"The main socket was successfully bound!\n");
	}

	//Listen to incoming traffic
	res = listen(this->main_connection, 1);
	if (res == SOCKET_ERROR) {
		SERVER_ERROR_THROW(wstring(L"Failed to listen to socket with net error: ") + to_wstring(WSAGetLastError()) + L'\n');
		return 1;
	}
	else {
		wprintf(L"Successfully listening to the main socket\n");
	}

	return 0;
}

int server::accept_connection() {
	assert(this->main_connection != INVALID_SOCKET);
	wprintf(L"Waiting for connection\n");

	//establishing the connection
	sockaddr_in client_info;
	int struct_sz = sizeof(client_info);
	SOCKET new_connection = accept(this->main_connection, (sockaddr*) &client_info, nullptr);
	if (new_connection == INVALID_SOCKET) {
		SERVER_ERROR_THROW(wstring(L"Failed to establish a connection with net error: ") + to_wstring(WSAGetLastError()) + L'\n');
		return 1;
	}
	wprintf((wstring(L"Successfully established a new connection: ") + to_wstring(this->active_connection_cnt) + L'\n').c_str());

	//Reading the passed data
	char data_buffer[1000];
	int bytes = recv(new_connection, data_buffer, sizeof(data_buffer), 0);
	if (bytes == SOCKET_ERROR) {
		SERVER_ERROR_THROW(wstring(L"The client failed to provide data: ") + to_wstring(WSAGetLastError()) + L'\n');
		closesocket(new_connection);
		return 1;
	}
	wprintf(L"Server just recieved %d bytes!\n", bytes);
	log_data(data_buffer, bytes);

	send_data(new_connection, "HTTP/1.1 200\r\nContent-Length: 2\r\n\r\nOK");
	closesocket(new_connection);

	return 0;
}

int server::send_data(const SOCKET& soc, std::string s) {
	int send_res = send(soc, s.c_str(), int(s.size()) * sizeof(char), 0);
	if (send_res == SOCKET_ERROR) {
		wprintf(L"Failed to send data! %d\n", WSAGetLastError());
		return 1;
	}
	wprintf(L"Successfully sent the data!\n");
	return 0;
}

int server::close_server() {
	wprintf(L"Closing the main socket and deregestering it!\n");
	if (this->main_connection != INVALID_SOCKET) {
		int code = closesocket(this->main_connection);
		if (code == SOCKET_ERROR) {
			SERVER_ERROR_THROW(wstring(L"Could not close the main socket!") + to_wstring(WSAGetLastError()) + L'\n');
			return 1;
		}
	}
	WSACleanup();
	return 0;
}

int server::add_socket(const SOCKET& to_add) {
	if (this->active_connection_cnt >= server::MAX_CONNECTIONS) {
		SERVER_ERROR_THROW(wstring(L"Cannot handle additional connections!"));
		return 1;
	}

	for (int i = 0; i < server::MAX_CONNECTIONS; i++) {
		if (this->active_connections[i] == INVALID_SOCKET) {
			this->active_connections[i] = to_add;
			this->active_connection_cnt++;
			return 0;
		}
	}
	return 1;
}




