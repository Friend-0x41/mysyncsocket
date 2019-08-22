#include "msyncsocket.h"

MSyncSocket::MSyncSocket(const std::string& ip, int port)
	:_errorCode(0)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	_socketClient = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	_serverAddr.sin_family = PF_INET;
	inet_pton(PF_INET, ip.c_str(), &_serverAddr.sin_addr);
	_serverAddr.sin_port = htons(port);
}

MSyncSocket::MSyncSocket(const std::string& domain)
	:_errorCode(0)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	hostent* host = gethostbyname(domain.c_str());
	if (!host)
	{
		_errorCode = -1;
		return;
	}
	std::string ip = host->h_addr_list[0];
	_socketClient = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	_serverAddr.sin_family = PF_INET;
	inet_pton(PF_INET, ip.c_str(), &_serverAddr.sin_addr);
	_serverAddr.sin_port = htons(80);
}

MSyncSocket::~MSyncSocket()
{
	closeConnect();
	WSACleanup();
}

bool MSyncSocket::send(const char* data, size_t len)
{
	_errorCode = ::send(_socketClient, data, len, 0);
	return _errorCode > 0;
}

std::string MSyncSocket::receive()
{
	char buf[1024];
	memset(buf, 0, 1024);
	recv(_socketClient, buf, 1024, 0);
	return buf;
}

int MSyncSocket::getErrorCode() const noexcept
{
	return _errorCode;
}

bool MSyncSocket::connect()
{ 
	_errorCode = ::connect(_socketClient, (sockaddr*)& _serverAddr, sizeof(sockaddr));
	return _errorCode == 0;
}

void MSyncSocket::closeConnect()
{
	closesocket(_socketClient);
	_errorCode = -1;
}
