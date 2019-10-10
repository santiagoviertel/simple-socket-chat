#ifdef __MINGW32__
    #define _WIN32_WINNT 0x501
    #include <Ws2tcpip.h>
#endif
#ifdef __linux__
#endif

#include <pthread.h>

#include <stdio.h>

class  Conexao {

private:
	pthread_t threadOuvinte;
	SOCKET canal;

public:
	static const int FRACASSO = 0;
	static const int SUCESSO = 1;

	static int cria(Conexao **con,char *ip,char *porta);
	static void destroi(Conexao **con);

	void sendMensagem(char* msg);
	void getIP(char *ret);
	SOCKET getSocket();
};

void *run(void* arg);
