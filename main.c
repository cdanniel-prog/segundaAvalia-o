#include "banco.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include<locale.h>


int main() {
    setlocale(LC_ALL, "Portuguese");

    const char *nomeArquivo = "usuarios.txt";
      
    Banco *banco = criarBanco();
    
    carregarUsuarios(banco, nomeArquivo);

    exibirBoasVindas();

    int usuarioID;

    while (1) {

        printf ("Você já tem cadastro? (1 - Sim, 2 - Não): ");
        int resposta;
        scanf("%d", &resposta);

        if (resposta == 1) {
            usuarioID = realizarLogin(banco);
            if (usuarioID != -1) {
                acessarMenuCadastrado(banco, usuarioID);
            } else {
                printf("\nTente novamente mais tarde.\n");
            }
        } else if (resposta == 2) {
            menuNaoCadastrado(banco);
        } else {
            break;
        }
    }

    encerrarPrograma(banco, nomeArquivo);

    return 0;
}
