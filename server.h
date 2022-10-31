#pragma once

#include<winsock.h>
#include<string>

const int MAX_CONNECTIONS = 300;

class server {
public:
	static const int MAX_CONNECTIONS = 300;

private:

	SOCKET active_connections[MAX_CONNECTIONS] = {INVALID_SOCKET};
	int active_connection_cnt = 0;
	SOCKET main_connection = INVALID_SOCKET;

private:

	int add_socket(const SOCKET& to_add);
	int send_data(const SOCKET& soc, std::string s);

public:
	server(){};

	int setup();
	int initiate();
	int accept_connection();
	int close_server();




};


