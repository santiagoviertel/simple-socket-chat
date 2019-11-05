#include <iostream>
#include "Connection.hpp"

using namespace boost::asio;
using namespace std;

Connection::Connection(string ip,string service) {
	try {
		io_service io_service;
		ip::tcp::resolver resolver(io_service);
		ip::tcp::resolver::query query(ip,service);
		ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		socket = make_unique<ip::tcp::socket>(io_service);
		connect(*socket,iterator);
		connected = true;
		thread = make_unique<std::thread>(run,this);
	} catch(exception& e) {
		cerr << e.what() << endl;
	}
}

Connection::Connection(unique_ptr<ip::tcp::socket> socket) {
	try {
		this->socket = move(socket);
		connected = true;
		thread = make_unique<std::thread>(run,this);
	} catch(exception& e) {
		cerr << e.what() << endl;
	}
}

Connection::~Connection() {


	
	cout << "Destruiu conexao com " << getRemoteIP() << "." << endl;
	


	socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	socket->close();
	thread->join();
}

void Connection::run(Connection *arg) {
	char message[128];
	size_t length;
	boost::system::error_code error;
	string ip = arg->getRemoteIP();
	while(1) {
		length = arg->socket->read_some(buffer(message),error);
		if(error==error::eof) {
			cout << "Connection closed with " << ip << "." << endl;
			break;
		} else if(error) {
			cout << "An error occurred with the connection with " << ip << "." << endl;
			break;
		} else {
			message[length] = 0;
			cout << "Message \"" << message << "\" received from " << ip << "." << endl;
		}
	}
	arg->connected = false;
}

bool Connection::isConnected() {
	return connected;
}

void Connection::sendMessage(string msg) {
	const char *message = msg.c_str();
	boost::system::error_code error;
	write(*socket,buffer(message,strlen(message)),error);
	cout << "Message sent to " << getRemoteIP() << "." << endl;
}

string Connection::getRemoteIP() {
	return socket->remote_endpoint().address().to_string();
}