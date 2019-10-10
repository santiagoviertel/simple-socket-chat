#include "Conexao.h"

int main() {
    char ip[10]="localhost",porta[6]="27015";
    Conexao* con;
    int op;
    char msg[100];
#ifdef __MINGW32__
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) {
        printf("Socket nao iniciado.\n");
        return 0;
    }
    printf("WSAStartup\n");
#endif
    if(Conexao::cria(&con,ip,porta)==Conexao::FRACASSO)
        printf("Ocorreu um erro ao criar a conexao.\n");
    else {
        printf("Conexao criada com sucesso.\n");
        do {
            printf("Digite uma opcao:\n");
            printf("0 - Mandar mensagem;\n");
            printf("1 - Sair.\n");
            scanf("%i",&op);
            if(op==0) {
                printf("Digite a mensagem:\n");
                fflush(stdin);
                gets(msg);
                con->sendMensagem(msg);
            }
        } while(op!=1);
        Conexao::destroi(&con);
    }
#ifdef __MINGW32__
    WSACleanup();
#endif
    printf("Programa finalizado.\n");
    return 0;
}
