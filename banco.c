#include "banco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include<locale.h>

void exibirBoasVindas() {
    printf(" =================================\n");
    printf(" Bem-vindo ao Banco Digital CDBank\n");
    printf(" =================================\n");
}

void sair(Banco *banco, const char *nomeArquivo){
    salvarUsuarios(banco, nomeArquivo);

    Usuario *atual = banco->usuarios;
    Usuario *prox;

    while(atual != NULL){
        prox = atual->prox;
        free(atual);
        atual = prox;
    }
    free(banco);
    printf("Memória liberada. Programa enceerrado.\n");
}

void encerrarPrograma(Banco *banco, const char *nomeArquivo){
    salvarUsuarios(banco, nomeArquivo);
    printf("\n===========================================\n");
    printf("Obrigado por utilizar o Banco Digital CDBank!\n");
    printf("Esperamos vê-lo novamente em breve.\n");
    printf("===============================================\n");
    exit(0);
}

Banco* criarBanco() {
    Banco *banco = (Banco*) malloc(sizeof(Banco));
    if (!banco) {
        printf("Erro ao alocar memória para o banco!\n");
        exit(1);
    }
    banco->usuarios = NULL;
    banco->totalUsuarios = 0;
    carregarUsuarios(banco, "usuarios.txt");
    return banco;
}

Usuario* merge(Usuario *esquerda, Usuario *direita){
    if(!esquerda) return direita;
    if(!direita) return esquerda;

    if(strcasecmp(esquerda->nome, direita->nome) < 0){
        esquerda->prox = merge(esquerda->prox, direita);
        return esquerda;
    } else{
        direita->prox = merge(esquerda, direita->prox);
        return direita;
    }
}

void dividir(Usuario *fonte, Usuario **esquerda, Usuario **direita){
    Usuario *rapido, *lento;
    lento = fonte;
    rapido = fonte->prox;

    while(rapido != NULL){
        rapido = rapido->prox;
        if(rapido != NULL){
            lento = lento->prox;
            rapido = rapido->prox;
        }
    }
    *esquerda = fonte;
    *direita = lento->prox;
    lento->prox = NULL;
}

void mergeSort(Usuario **cabecaRef){
    Usuario *cabeca = *cabecaRef;
    Usuario *esquerda = NULL;
    Usuario *direita = NULL;

    if((cabeca == NULL) || (cabeca->prox == NULL)){
        return;
    }
    dividir(cabeca, &esquerda, &direita);
    mergeSort(&esquerda);
    mergeSort(&direita);

    *cabecaRef = merge(esquerda, direita);
}

void ordenarUsuariosPorNome(Banco *banco){
    mergeSort(&(banco->usuarios));
}

void carregarUsuarios(Banco *banco, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de usuários.\n");
        return;
    }
    
    Usuario *novoUsuario;
    while(1){
        novoUsuario = (Usuario*) malloc(sizeof(Usuario));
        if(!novoUsuario){
        printf("Erro ao alocar memória para o novo usuário!\n");
        fclose(arquivo);
        return;
        }

        if(fscanf(arquivo, "%[^|],%[^|],%[^|],%f\n", novoUsuario->nome, novoUsuario->email, novoUsuario->senha, &novoUsuario->saldo) == 4) {
            if(validarEmail(banco, novoUsuario->email, NULL)){
                inserirUsuario(banco, novoUsuario);
            } else {
                free(novoUsuario);
        }
        } else {
            free(novoUsuario);
            break;
        }
    }
    fclose(arquivo); 
}

void salvarUsuarios(Banco *banco, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");     
    if (arquivo != NULL) {
        Usuario *atual = banco->usuarios;
        while(atual != NULL){
            int tamanhoSenha = strlen(atual->senha);
            char senhaAsteriscos[tamanhoSenha + 1];
            for(int j = 0; j < tamanhoSenha; j++){
                senhaAsteriscos[j] = '*';
            }
            senhaAsteriscos[tamanhoSenha] = '\0';
            
            fprintf(arquivo, "Nome: %s | Email: %s | Senha: %s | Saldo: %.2f\n", atual->nome, atual->email, senhaAsteriscos, atual->saldo);
            atual = atual->prox;
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para salvar os usuários.\n");
    }
}

void inserirUsuario(Banco *banco, Usuario *novoUsuario){
    novoUsuario->prox = NULL;
    if (banco->usuarios == NULL) {
        banco->usuarios = novoUsuario;
    } else {
        Usuario *atual = banco->usuarios;
        while (atual->prox != NULL){
            atual = atual->prox;
        }
        atual->prox = novoUsuario;
    }
    banco->totalUsuarios++;
}

void cadastrarUsuario(Banco *banco, const char *nomeArquivo) {
    Usuario *novoUsuario = (Usuario*) malloc(sizeof(Usuario));
    novoUsuario->prox = NULL;
    if (!novoUsuario) {
        printf("Erro ao alocar memória para o novo usuário!\n");
        return;
    }
    novoUsuario->saldo = 0.0f;

    printf("Digite seu nome: ");
    fgets(novoUsuario->nome, 50, stdin);
    novoUsuario->nome[strcspn(novoUsuario->nome, "\n")] = 0;

    do {
        printf("Digite seu email: ");
        scanf("%s", novoUsuario->email);
    } while (!validarEmail(banco, novoUsuario->email, NULL));

    printf("Digite sua senha: ");
    capturarSenha(novoUsuario->senha);

    inserirUsuario(banco, novoUsuario);

    printf("Usuário cadastrado com sucesso!\n\n");

    salvarUsuarios(banco, nomeArquivo);
}

void listarUsuarios(Banco *banco){
    if(banco->usuarios == NULL){
        printf("Nenhum usuário cadastrado.\n");
        return;
    }

    printf("Lista de usuários cadastrados:\n");
    Usuario *atual = banco->usuarios;
    while (atual != NULL){     
        printf("Nome: %s | Email: %s | Saldo: R$ %.2f\n", atual->nome, atual->email, atual->saldo);
        atual = atual->prox;
    }
    printf("\n");
}

void editarUsuario(Banco *banco, const char *nomeArquivo){
    char email[80];
    listarUsuarios(banco);
    printf("Digite o email do usuário que deseja editar: ");
    scanf("%s", email);

    Usuario *usuario = NULL;
    Usuario *atual = banco->usuarios;
    
    while (atual != NULL){     
        if(strcmp(atual->email, email) == 0){
            usuario = atual;
            break;
        }
        atual = atual->prox;
    }

    if (usuario == NULL){
        printf("Usuário não encontrado.\n");
        return;
    }
    
    printf("Digite o novo nome: ");
    getchar(); 
    fgets(usuario->nome, 50, stdin);
    usuario->nome[strcspn(usuario->nome, "\n")] = 0;
    
    do{
        printf("Digite o novo email: ");
        scanf("%s", usuario->email);
    } while (!validarEmail(banco, usuario->email, usuario));

    printf("Digite a nova senha: ");
    capturarSenha(usuario->senha);

    printf("Usuário editado com sucesso!\n\n");

    salvarUsuarios(banco, nomeArquivo);
}

void excluirUsuario(Banco *banco, const char *nomeArquivo){
    char email[80];
    listarUsuarios(banco);        
    printf("Digite o email do usuário que deseja excluir: ");
    scanf("%s", email);

    Usuario *anterior = NULL;
    Usuario *atual = banco->usuarios;

    while(atual != NULL && strcmp(atual->email, email) != 0){
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL){
        printf("Usuário não encontrado.\n");
        return;
    }
    
    if(anterior == NULL){
        banco->usuarios = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    banco->totalUsuarios--;
    printf("Usuário excluído com sucesso!\n\n");

    salvarUsuarios(banco, nomeArquivo);
}

int realizarLogin(Banco *banco) {
    char email[80];
    char senha[20];
    Usuario *atual = banco->usuarios;
    int usuarioID = 0;
    
    printf("Digite seu email: ");
    scanf("%s", email);
    printf("Digite sua senha: ");
    capturarSenha(senha);

    if(strchr(email, '@') == NULL){
        printf("Erro: Email inválido! O email deve conter o caractere '@'.\n");
        return -1;
    }

    while(atual != NULL){
        if(strcmp(atual->email, email) == 0 && strcmp(atual->senha, senha) == 0){
            printf("Login realizado com sucesso! Bem-vindo, %s!\n\n", atual->nome);
            return usuarioID;
        }
        usuarioID++;
        atual = atual->prox;
    }
    printf("Email ou senha incorretos! Tente novamente.\n");
    return -1;
}

void acessarMenuCadastrado(Banco *banco, int usuarioID){
    menuUsuario(banco, usuarioID);
}

void menuNaoCadastrado(Banco *banco){
    const char *nomeArquivo = "usuarios.txt";
    int opcao;
    int tentativasLogin = 0;

    while(1){
        printf("Menu Principal\n");
        printf("1. Login\n");
        printf("2. Cadastrar usuário\n");
        printf("3. Listar usuários\n");
        printf("4. Editar usuário\n");
        printf("5. Excluir usuário\n");
        printf("6. Tela Inicial\n");
        printf("7. Sair do Banco\n");
        printf("\nEscolha uma opção: ");

        opcao = solicitarOpcaoValida(1, 7);

        switch (opcao){   
            case 1: {
                if(tentativasLogin < 3){
                    int usuarioID = realizarLogin(banco);
                    if(usuarioID != -1){
                    acessarMenuCadastrado(banco, usuarioID);
                    tentativasLogin = 0;
                    } else{
                        tentativasLogin++;
                        printf("Erro: Falha no Login. Tentativas restantes: %d\n", 3 - tentativasLogin);
                    }
                } else{
                    printf("Número máximo de tentativas excedido. Obrigado por usar o Banco Digital CDBank. Tente novamente mais tarde.\n");
                    return;
                }
                break;  
            }
            case 2:                
                cadastrarUsuario(banco, nomeArquivo);
                break;
            case 3: 
                listarUsuarios(banco);
                break;
            case 4:
                editarUsuario(banco, nomeArquivo);
                break;
            case 5: 
                excluirUsuario(banco, nomeArquivo);
                break;
            case 6:         
                return;
            case 7: 
                encerrarPrograma(banco, nomeArquivo);
                break;
            default:
                printf("Opção inválida!\n");
        }
    } 
}

void menuUsuario(Banco *banco, int usuarioID){
    Usuario *atual = banco->usuarios;
    int contador = 0;

    while(atual != NULL && contador != usuarioID){
        contador++;
        atual = atual->prox;
    }

    if(atual == NULL){
        printf("Usuário não encontrado.\n");
        return;
    }
    
    int opcao;
    while(1){
        printf("\nMenu do Usuário: %s\n", atual->nome);
        printf("1. Verificar Saldo\n");
        printf("2. Realizar Depósito\n");
        printf("3. Realizar Saque\n");
        printf("4. Alterar Senha\n");
        printf("5. Realizar Transferências\n");
        printf("6. Acessar o Menu Principal\n");
        printf("\nEscolha uma opção: ");

        opcao = solicitarOpcaoValida(1, 6);

        switch (opcao){
            case 1:
                verificarSaldo(banco, usuarioID);
                break;
            case 2:
                realizarDeposito(banco, usuarioID, "usuarios.txt");
                break;
            case 3:
                realizarSaque(banco, usuarioID, "usuarios.txt");
                break;
            case 4:
                alterarSenha(banco, usuarioID, "usuarios.txt");
                break;
            case 5:
                realizarTransferencia(banco, usuarioID, "usuarios.txt");
                break;
            case 6:
                return;
            default:
                printf("Opção inválida! Tente novamente.\n\n");
        }
    } 
}

int validarEmail(Banco *banco, const char *email, Usuario *usuarioAtual){
    if(strchr(email, '@') == NULL){
        printf("Erro: Email inválido! O email deve conter o caractere '@'.\n");
        return 0;
    }
    
    Usuario *usuario = banco->usuarios;
    while(usuario != NULL){
        if(usuario != usuarioAtual && strcmp(usuario->email, email) == 0){
            printf("Erro: Email já cadastrado!\n");
            return 0;
        }
    
    usuario = usuario->prox;;
    }
    return 1;
}

void capturarSenha(char *senha){
    char ch;
    int i = 0;
    while(1){
        ch = getch();
        if(ch == 13){ 
            senha[i] = '\0';
            break;
        } else if(ch == 8){ 
            if(i > 0){
                i--;
                printf("\b \b");
            }
        } else if(i < 19){
            senha[i++] = ch;
            printf("*");
        }
    }
    printf("\n");
}

float solicitarValorPositivo(){
    float valor;
    int resultado;
    while(1){
        resultado = scanf("%f", &valor);
        if(resultado != 1 || valor <= 0){
            printf("Erro: Valor inválido! Insira um valor positivo.\n");
            while(getchar() != '\n');
        } else {
            while(getchar() != '\n');
            return valor;
        }
    }
}

int solicitarOpcaoValida(int min, int max){
    int opcao;
    int resultado;      
    while(1){
        resultado = scanf("%d", &opcao);
        if(resultado != 1 || opcao < min || opcao > max){
            printf("Erro: Opção inválida! Insira um número entre %d e %d.\n", min, max);
            while(getchar() != '\n');
        } else {
            while(getchar() != '\n');
            return opcao;
        }
    }
}

void verificarSaldo(Banco *banco, int usuarioID){
    Usuario *atual = banco->usuarios;
    int contador = 0;
    
    while(atual != NULL){
        if(contador == usuarioID){
            printf("Seu saldo é: R$ %.2f\n\n", atual->saldo);
            return;
        }
        contador++;
        atual = atual->prox;
    }
    printf("Usuário não encontrado.\n");
}

void realizarDeposito(Banco *banco, int usuarioID, const char *nomeArquivo){
    Usuario *atual = banco->usuarios;
    int contador = 0;

    while(atual != NULL){
        if(contador == usuarioID){
            printf("Digite o valor do depósito: R$ ");
            float valor = solicitarValorPositivo();
            atual->saldo += valor;
            printf("Depósito de R$ %.2f realizado com sucesso!\n\n", valor);
            salvarUsuarios(banco, nomeArquivo);
            return;
        }
        contador++;
        atual = atual->prox;
    }
    printf("Usuário não encontrado.\n");
}

void realizarSaque(Banco *banco, int usuarioID, const char *nomeArquivo){
    Usuario *atual = banco->usuarios;
    int contador = 0;

    while(atual != NULL){
        if(contador == usuarioID){
            printf("Digite o valor do saque: R$ ");
            float valor = solicitarValorPositivo();
            if(valor <= atual->saldo){
                atual->saldo -= valor;
                printf("Saque de R$ %.2f realizado com sucesso!\n\n", valor);
                salvarUsuarios(banco, nomeArquivo);
            } else {
                printf("Saldo insuficiente!\n\n");
            }
            return;
        }
        contador++;
        atual = atual->prox;
    }
    printf("Usuário não encontrado.\n");
}

void alterarSenha(Banco *banco, int usuarioID, const char *nomeArquivo){
    Usuario *atual = banco->usuarios;
    int contador = 0;

    while(atual != NULL){
        if(contador == usuarioID){
            printf("Digite sua nova senha: ");
            capturarSenha(atual->senha);
            printf("Senha alterada com sucesso!\n\n");

            salvarUsuarios(banco, nomeArquivo);
            return;
        }
        contador++;
        atual = atual->prox;
    }
    printf("Usuário não encontrado.\n");
}

void realizarTransferencia(Banco *banco, int usuarioID, const char *nomeArquivo){
    int destinatarioID;
    listarUsuarios(banco);
    printf("Selecione o número do usuário para quem deseja transferir: ");
    scanf("%d", &destinatarioID);

    Usuario *remetente = banco->usuarios;
    int remetenteContador = 0;
    while(remetente != NULL && remetenteContador != usuarioID){
        remetenteContador++;
        remetente = remetente->prox;
    }

    if(remetente == NULL){
        printf("Usuário remetente não encontrado.\n");
        return;
    }

    Usuario *destinatario = banco->usuarios;
    int destinatarioContador = 0;
    while(destinatario != NULL && destinatarioContador != destinatarioID){
        destinatarioContador++;
        destinatario = destinatario->prox;
    }

    if(destinatario == NULL || destinatarioID == usuarioID){
        printf("Erro: Usuário destinatário inválido!\n\n");
        return;
    }
    printf("Digite o valor da transferência: ");
    float valor = solicitarValorPositivo();
    if(valor <= remetente->saldo){
        remetente->saldo -= valor;
        destinatario->saldo += valor;
        printf("Transferência de R$ %.2f realizada com sucesso para %s!\n\n", valor, destinatario->email);
        salvarUsuarios(banco, nomeArquivo);
    } else{
        printf("Saldo Insuficiente!\n\n");
    }       
} 
