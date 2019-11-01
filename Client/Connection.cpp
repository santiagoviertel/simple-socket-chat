#include <iostream>
#include "Connection.hpp"

using namespace boost::asio;
using namespace std;

ip::tcp::socket *Connection::socket;
thread *Connection::thread;

void Connection::create(string ip,string service) {
	try {
		io_service io_service;
		ip::tcp::resolver resolver(io_service);
		ip::tcp::resolver::query query(ip,service);
		ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		socket = new ip::tcp::socket(io_service);
		connect(*socket,iterator);
		thread = new std::thread(run);
	} catch(exception& e) {
		cerr << e.what() << endl;
	}
}

void Connection::destroi() {
	socket->close();
	delete thread;
}

void Connection::run() {
	char message[128];
	size_t length;
	boost::system::error_code error;
	string ip = getRemoteIP();
	while(1) {
		length = socket->read_some(buffer(message),error);
		if(error==error::eof) {
			cout << "Connection closed by server " << ip << "." << endl;
			break;
		} else if(error) {
			cout << "An error occurred with the connection to server " << ip << "." << endl;
			break;
		} else {
			message[length] = 0;
			cout << "Message \"" << message << "\" received from server " << ip << "." << endl;
		}
	}
}

void Connection::sendMessage(string msg) {
	const char *message = msg.c_str();
	boost::system::error_code error;
	write(*socket,buffer(message,strlen(message)),error);
	cout << "Message sent to server " << getRemoteIP() << "." << endl;
}

string Connection::getRemoteIP() {
	return socket->remote_endpoint().address().to_string();
}