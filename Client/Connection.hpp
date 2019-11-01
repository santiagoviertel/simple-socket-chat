#include <boost/asio.hpp>
#include <thread>

class Connection {

private:
	static boost::asio::ip::tcp::socket *socket;
	static std::thread *thread;

public:
	static void create(std::string ip,std::string service);
	static void destroi();
	static void run();

	static void sendMessage(std::string msg);
	static std::string getRemoteIP();
};