#ifndef __Server_H__
#define __Server_H__

#include <boost/asio.hpp>
#include <thread>
#include <list>
#include "Connection.hpp"

class Server {

private:
	bool listening = false;
	boost::asio::io_service io_service;
	std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;
	std::unique_ptr<std::thread> thread;
	std::list<Connection*> cons;

public:
	Server(unsigned short port);
	~Server();

	static void run(Server *arg);
	bool isListening();
	void removeConnection(Connection *con);
	void sendBroadcastMessage(std::string msg);
};

#endif