#ifndef __Server_H__
#define __Server_H__

#include <boost/asio.hpp>
#include <thread>
#include <list>
#include "Connection.hpp"

class Server {

private:
	static boost::asio::io_service io_service;
	static boost::asio::ip::tcp::acceptor *acceptor;
	static std::thread *thread;
	static std::list<Connection*> cons;

public:
	static const short FAILURE = 0;
	static const short SUCCESS = 1;

	static short create(unsigned short port);
	static void destroy();

	static void run();
	static void addConnection(Connection *con);
	static void removeConnection(Connection *con);
	static void sendBroadcastMessage(std::string msg);
};

#endif