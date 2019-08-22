/**********************************
*******socket base class, send and receive data by tcp sync
*******¹ùÐËÔª 2019-8-22*/


#pragma once

#include <string>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

class MSyncSocket
{
public:

	MSyncSocket(const std::string& ip, int port);

	MSyncSocket(const std::string& domain);

	~MSyncSocket();

	bool send(const char* data, size_t len);

	std::string receive();

	int getErrorCode() const noexcept;

	bool connect();
private:

	void closeConnect();
private:

	SOCKET _socketClient;

	sockaddr_in _serverAddr;

	int _errorCode;
 };