class Servidor;

class Conexao {

private:
	Servidor* serv;
	pthread_t thread;
	SOCKET socket;

public:
	static const int FRACASSO = 0;
	static const int SUCESSO = 1;

	static int cria(Conexao **con,Servidor* serv,SOCKET socket);
	static void destroi(Conexao **con);

	void sendMensagem(char* msg);
	void getIP(char *ret);
	SOCKET getSocket();
	Servidor* getServidor();
};

void *runThreadConexao(void* arg);
