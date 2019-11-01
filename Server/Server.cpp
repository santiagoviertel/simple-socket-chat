#include <iostream>
#include "Server.hpp"

using namespace boost::asio;
using namespace std;

io_service Server::io_service;
ip::tcp::acceptor *Server::acceptor;
thread *Server::thread;
list<Connection*> Server::cons;

short Server::create(unsigned short port) {
	try {
		acceptor = new ip::tcp::acceptor(io_service,ip::tcp::endpoint(ip::tcp::v4(),port));
		thread = new std::thread(run);
		return SUCCESS;
	} catch(exception& e) {
		cerr << e.what() << endl;
	}
	return FAILURE;
}

void Server::destroy() {
	acceptor->close();
	delete thread;
}

void Server::run() {
	try {
		do {
			shared_ptr<ip::tcp::socket> socket(new ip::tcp::socket(io_service));
			acceptor->accept(*socket);
			Connection *con = new Connection(socket);
			cout << "Connection with " << con->getRemoteIP() << " established." << endl;
		} while(1);
	} catch(exception& e) {
		cerr << e.what() << endl;
	}
}

void Server::addConnection(Connection *con) {
	cons.push_back(con);
}

void Server::removeConnection(Connection *con) {
	cons.remove(con);
}

void Server::sendBroadcastMessage(string msg) {
	for(list<Connection*>::iterator i=cons.begin();i!=cons.end();++i)
		(*i)->sendMessage(msg);
}