#ifndef __Connection_H__
#define __Connection_H__

#include <boost/asio.hpp>
#include <thread>

class Connection {

private:
	std::shared_ptr<boost::asio::ip::tcp::socket> socket;
	std::thread *thread;

public:
	Connection(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
	~Connection();

	static void run(Connection *arg);
	void sendMessage(std::string msg);
	std::string getRemoteIP();
};

#endif