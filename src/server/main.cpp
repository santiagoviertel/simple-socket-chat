#include <iostream>
#include "Server.hpp"

using namespace std;

int main(int argc,char *argv[]) {
	int op;
	string msg;
	if(argc>=2) {
		Server serv(strtoul(argv[1],NULL,0));
		if(serv.isListening()) {
			cout << "Server successfully created." << endl;
			do {
				cout << "Type an option:" << endl;
				cout << "0 - Send broadcast message;" << endl;
				cout << "1 - Exit." << endl;
				cin >> op;
				if(op==0) {
					cout << "Type the message:" << endl;
					cin.ignore();
					getline(cin,msg);
					serv.sendBroadcastMessage(msg);
				}
			} while(op!=1);
			cout << "Program finished." << endl;
		} else
			cout << "An error occurred in the server creation." << endl;
	} else
		cout << "Port number is necessary." << endl;
	return 0;
}