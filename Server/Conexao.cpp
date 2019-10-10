#include "Headers.h"

int Conexao::cria(Conexao **con,Servidor* serv,SOCKET socket) {
    *con = new Conexao();
    (*con)->serv = serv;
    (*con)->socket = socket;
    if(pthread_create(&((*con)->thread),NULL,runThreadConexao,*con)) {
        delete *con;
        *con = NULL;
        return Conexao::FRACASSO;
    }
    return Conexao::SUCESSO;
}

void Conexao::destroi(Conexao **con) {
    shutdown((*con)->socket,SD_BOTH);
    closesocket((*con)->socket);
    delete *con;
    *con = NULL;
}

void Conexao::sendMensagem(char* msg) {
    char ip[20];
    int tam = send(Conexao::socket,msg,strlen(msg),0);
    Conexao::getIP(ip);
    if(tam==SOCKET_ERROR)
        printf("Houve um erro ao enviar a mensagem para %s\n",ip);
    else
        printf("Mensagem enviada para %s\n",ip);
}

void Conexao::getIP(char *ret) {
    sockaddr_in endereco;
    socklen_t tamanho = sizeof(sockaddr_in);
    if(!getpeername(Conexao::socket,(struct sockaddr *)&endereco,&tamanho))
        sprintf(ret,"%i.%i.%i.%i",
            int(endereco.sin_addr.s_addr&0xFF),
            int((endereco.sin_addr.s_addr&0xFF00)>>8),
            int((endereco.sin_addr.s_addr&0xFF0000)>>16),
            int((endereco.sin_addr.s_addr&0xFF000000)>>24));
}

SOCKET Conexao::getSocket() {
    return Conexao::socket;
}

Servidor* Conexao::getServidor() {
    return Conexao::serv;
}

void *runThreadConexao(void* arg) {
    Conexao *con = static_cast<Conexao*>(arg);
    int tam;
    char ret[200],ip[20];
    con->getServidor()->addConexao(con);
    con->getIP(ip);
    do {
        tam = recv(con->getSocket(),ret,200,0);
        if(tam>0) {
            ret[tam] = '\0';
            printf("Mensagem recebida %s de %s\n",ret,ip);
        } else
            printf("Cliente desconectado %s\n",ip);
    }while(tam>0);
    con->getServidor()->remConexao(con);
    Conexao::destroi(&con);
    return NULL;
}
