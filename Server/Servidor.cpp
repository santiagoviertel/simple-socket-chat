#include "Headers.h"

int Servidor::cria(Servidor **ser,char *porta) {
    struct addrinfo *result,hints;
    //Criando a conexao
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    if(getaddrinfo(NULL,porta,&hints,&result)!=0) {
        *ser = NULL;
        return Servidor::FRACASSO;
    }
    printf("getaddrinfo\n");
    *ser = new Servidor();
    (*ser)->canalPrincipal = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
    if((*ser)->canalPrincipal==INVALID_SOCKET) {
        freeaddrinfo(result);
        delete(*ser);
        *ser = NULL;
        return Servidor::FRACASSO;
    }
    printf("socket\n");
    if(bind((*ser)->canalPrincipal,result->ai_addr,(int)result->ai_addrlen)==SOCKET_ERROR) {
        closesocket((*ser)->canalPrincipal);
        freeaddrinfo(result);
        delete(*ser);
        *ser = NULL;
        return Servidor::FRACASSO;
    }
    printf("bind\n");
    freeaddrinfo(result);
    if(listen((*ser)->canalPrincipal,SOMAXCONN)==SOCKET_ERROR) {
        closesocket((*ser)->canalPrincipal);
        delete(*ser);
        *ser = NULL;
        return Servidor::FRACASSO;
    }
    printf("listen\n");
    //A partir daqui precisa-se de uma thread com a funcao de criar varias conexoes
    if(pthread_create(&((*ser)->threadOuvinteNovasConexoes),NULL,runThreadOuvinteNovasConexoes,*ser)) {
        closesocket((*ser)->canalPrincipal);
        delete(*ser);
        *ser = NULL;
        return Servidor::FRACASSO;
    }
    printf("pthread_create: criada thread que ouve novas conexoes\n");
    return Servidor::SUCESSO;
}

void Servidor::destroi(Servidor **ser) {
    closesocket((*ser)->canalPrincipal);
    pthread_cancel((*ser)->threadOuvinteNovasConexoes);
    delete(*ser);
    *ser = NULL;
}

void Servidor::addConexao(Conexao *con) {
    Servidor::conexoes.push_back(con);
}

void Servidor::remConexao(Conexao *con) {
    Conexao* aux;
    for(unsigned int i=0;i<Servidor::conexoes.size();i++) {
        aux = static_cast<Conexao*>(Servidor::conexoes[i]);
        if(con==aux)
            Servidor::conexoes.erase(Servidor::conexoes.begin()+i);
    }
}

void Servidor::sendMensagemBroadcast(char* msg) {
    for(unsigned int i=0;i<Servidor::conexoes.size();i++)
        static_cast<Conexao*>(Servidor::conexoes[i])->sendMensagem(msg);
}

SOCKET Servidor::getSocketCanalPrincipal() {
    return Servidor::canalPrincipal;
}

int Servidor::getQuantidadeConexoes() {
    return Servidor::conexoes.size();
}

void *runThreadOuvinteNovasConexoes(void* arg) {
    Servidor *serv = static_cast<Servidor*>(arg);
    char ip[20];
    SOCKET soc;
    Conexao *con;
    while(serv->getQuantidadeConexoes()<SOMAXCONN) {
        soc = accept(serv->getSocketCanalPrincipal(),NULL,NULL);
        //Cria-se um novo canal de comunicacao com um novo cliente
        if(soc!=INVALID_SOCKET) {
            if(Conexao::cria(&con,serv,soc)==Conexao::SUCESSO) {
                con->getIP(ip);
                printf("Conexao com %s estabelecida\n",ip);
            }
            else {
                printf("Houve uma conexao perdida\n");
                closesocket(soc);
            }
        }
    }
    return NULL;
}
