#include <iostream>
#include "Connection.hpp"

using namespace std;

int main(int argc,char *argv[]) {
	int op;
	string msg;
	if(argc>=3) {
		Connection con(argv[1],argv[2]);
		if(con.isConnected()) {
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
					con.sendMessage(msg);
				}
			} while(op!=1);
		} else
			cout << "An error occurred in the connection creation." << endl;
	} else
		cout << "IP address and port number are necessary." << endl;
	return 0;
}