#ifndef __Connection_H__
#define __Connection_H__

#include <boost/asio.hpp>
#include <thread>

class Connection {
	private:
		bool connected = false;
		std::unique_ptr<boost::asio::ip::tcp::socket> socket;
		std::unique_ptr<std::thread> thread;

	public:
		Connection(std::string ip,std::string service);
		Connection(std::unique_ptr<boost::asio::ip::tcp::socket> socket);
		~Connection();

		static void run(Connection *arg);
		bool isConnected();
		void sendMessage(std::string msg);
		std::string getRemoteIP();
};

#endif