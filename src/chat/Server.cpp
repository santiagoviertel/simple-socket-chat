#include <iostream>
#include "Server.hpp"

using namespace boost::asio;
using namespace std;

Server::Server(unsigned short port) {
	try {
		acceptor = make_unique<ip::tcp::acceptor>(io_service,ip::tcp::endpoint(ip::tcp::v4(),port));
		listening = true;
		thread = make_unique<std::thread>(run,this);
	} catch(exception& e) {
		cerr << e.what() << endl;
	}
}

Server::~Server() {
	listening = false;
//	thread->join();
	for(list<Connection*>::iterator i=cons.begin();i!=cons.end();++i)
		delete *i;
}

void Server::run(Server *arg) {
	while(arg->listening) {
		try {
			unique_ptr<ip::tcp::socket> socket(new ip::tcp::socket(arg->io_service));
			arg->acceptor->accept(*socket);
			Connection *con = new Connection(move(socket));
			arg->cons.push_back(con);
			cout << "Connection with " << con->getRemoteIP() << " established." << endl;
		} catch(exception& e) {
			cout << "Connection attempt not established." << endl;
			cerr << e.what() << endl;
		}
	}
}

bool Server::isListening() {
	return listening;
}

void Server::removeConnection(Connection *con) {
	cons.remove(con);
}

void Server::sendBroadcastMessage(string msg) {
	for(list<Connection*>::iterator i=cons.begin();i!=cons.end();++i)
		(*i)->sendMessage(msg);
}