#include <iostream>
#include "Connection.hpp"

using namespace std;

int main(int argc,char *argv[]) {
	int op;
	string msg;
	if(argc>=3) {
		Connection::create(argv[1],argv[2]);
		cout << "Connection successfully created." << endl;
		do {
			cout << "Type an option:" << endl;
			cout << "0 - Send message;" << endl;
			cout << "1 - Exit." << endl;
			cin >> op;
			if(op==0) {
				cout << "Type the message:" << endl;
				cin.ignore();
				getline(cin,msg);
				Connection::sendMessage(msg);
			}
		} while(op!=1);
//		Connection::destroi();
		cout << "Program finished." << endl;
	} else
		cout << "IP address and service name are necessary." << endl;
	return 0;
}