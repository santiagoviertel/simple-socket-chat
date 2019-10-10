#include <vector>

class Servidor {

private:
    pthread_t threadOuvinteNovasConexoes;
    SOCKET canalPrincipal;
    std::vector<Conexao*> conexoes;

public:
    static const int FRACASSO = 0;
    static const int SUCESSO = 1;

	static int cria(Servidor **ser,char *porta);
	static void destroi(Servidor **ser);

	void addConexao(Conexao *con);
	void remConexao(Conexao *con);
	void sendMensagemBroadcast(char* msg);
	SOCKET getSocketCanalPrincipal();
	int getQuantidadeConexoes();
};

void *runThreadOuvinteNovasConexoes(void* arg);
