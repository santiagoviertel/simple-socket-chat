#include <iostream>
#include "Server.hpp"

using namespace boost::asio;
using namespace std;

Connection::Connection(shared_ptr<ip::tcp::socket> socket) {
	this->socket = socket;
	thread = new std::thread(run,this);
}

Connection::~Connection() {
	socket->close();
	delete(thread);
}

void Connection::run(Connection *arg) {
	char message[128];
	size_t length;
	boost::system::error_code error;
	string ip = arg->getRemoteIP();
	Server::addConnection(arg);
	while(1) {
		length = arg->socket->read_some(buffer(message),error);
		if(error==boost::asio::error::eof) {
			cout << "Connection closed by client " << ip << "." << endl;
			break;
		} else if(error) {
			cout << "An error occurred with the connection to client " << ip << "." << endl;
			break;
		} else {
			message[length] = 0;
			cout << "Message \"" << message << "\" received from client " << ip << "." << endl;
		}
	}
//	con->destroi();
	Server::removeConnection(arg);
}

void Connection::sendMessage(string msg) {
	const char *message = msg.c_str();
	boost::system::error_code error;
	write(*socket,buffer(message,strlen(message)),error);
	cout << "Message sent to client " << getRemoteIP() << "." << endl;
}

string Connection::getRemoteIP() {
	return socket->remote_endpoint().address().to_string();
}