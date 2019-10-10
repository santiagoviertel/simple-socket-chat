#include "Conexao.h"

int Conexao::cria(Conexao **con,char *ip,char *porta) {
    struct addrinfo *ptr,*result,hints;
    //Criando a conexao
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if(getaddrinfo(ip,porta,&hints,&result)!=0) {
        *con = NULL;
        return Conexao::FRACASSO;
    }
    printf("getaddrinfo\n");
    *con = new Conexao();
    for(ptr=result;ptr!=NULL;ptr=ptr->ai_next) {
        (*con)->canal = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
        if((*con)->canal==INVALID_SOCKET) {
            freeaddrinfo(result);
            delete(*con);
            *con = NULL;
            return Conexao::FRACASSO;
        }
        if(connect((*con)->canal,ptr->ai_addr,(int)ptr->ai_addrlen)==SOCKET_ERROR) {
            closesocket((*con)->canal);
            (*con)->canal = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    if((*con)->canal==INVALID_SOCKET) {
        delete(*con);
        *con = NULL;
        return Conexao::FRACASSO;
    }
    printf("connect\n");
    //Criando uma thread
    if(pthread_create(&((*con)->threadOuvinte),NULL,run,*con)) {
        shutdown((*con)->canal,SD_BOTH);
        closesocket((*con)->canal);
        delete(*con);
        *con = NULL;
        return Conexao::FRACASSO;
    }
    printf("pthread_create\n");
    return Conexao::SUCESSO;
}

void Conexao::destroi(Conexao **con) {
    shutdown((*con)->canal,SD_BOTH);
    closesocket((*con)->canal);
    pthread_cancel((*con)->threadOuvinte);
    delete(*con);
    *con = NULL;
}

void Conexao::sendMensagem(char* msg) {
    char ip[20];
    int tam = send(Conexao::canal,msg,strlen(msg),0);
    Conexao::getIP(ip);
    if(tam==SOCKET_ERROR)
        printf("Houve um erro ao enviar a mensagem para %s\n",ip);
    else
        printf("Mensagem enviada para %s\n",ip);
}

void Conexao::getIP(char *ret) {
    sockaddr_in endereco;
    socklen_t tamanho = sizeof(sockaddr_in);
    if(!getpeername(Conexao::canal,(struct sockaddr *)&endereco,&tamanho))
        sprintf(ret,"%i.%i.%i.%i",
            int(endereco.sin_addr.s_addr&0xFF),
            int((endereco.sin_addr.s_addr&0xFF00)>>8),
            int((endereco.sin_addr.s_addr&0xFF0000)>>16),
            int((endereco.sin_addr.s_addr&0xFF000000)>>24));
}

SOCKET Conexao::getSocket() {
    return Conexao::canal;
}

void *run(void* arg) {
    Conexao *con = static_cast<Conexao*>(arg);
    int tam;
    char ret[200],ip[20];
    con->getIP(ip);
    do {
        tam = recv(con->getSocket(),ret,200,0);
        if(tam>0) {
            ret[tam] = '\0';
            printf("Mensagem recebida %s de %s\n",ret,ip);
        } else
            printf("Servidor desconectado %s\n",ip);
    }while(tam>0);
    return NULL;
}
