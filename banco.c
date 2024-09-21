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
    for(int i = 0; i < banco->totalUsuarios; i++){
        free(banco->usuarios[i]);
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
    banco->totalUsuarios = 0;
    carregarUsuarios(banco, "usuarios.txt");
    return banco;
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
            banco->usuarios[banco->totalUsuarios] = novoUsuario;
            banco->totalUsuarios++;
        } else {
        free(novoUsuario);
        break;
        }
    }
    fclose(arquivo);

    quicksortUsuarioPorNome(banco->usuarios, 0, banco->totalUsuarios - 1);
}

void salvarUsuarios(Banco *banco, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo != NULL) {
        for (int i = 0; i < banco->totalUsuarios; i++) {
            Usuario *usuario = banco->usuarios[i];

            int tamanhoSenha = strlen(usuario->senha);
            char senhaAsteriscos[tamanhoSenha + 1];
            for(int j = 0; j < tamanhoSenha; j++){
                senhaAsteriscos[j] = '*';
            }
            senhaAsteriscos[tamanhoSenha] = '\0';
            
            fprintf(arquivo, "Nome: %s | Email: %s | Senha: %s | Saldo: %.2f\n", usuario->nome, usuario->email, senhaAsteriscos, usuario->saldo);
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para salvar os usuários.\n");
    }
}

void cadastrarUsuario(Banco *banco, const char *nomeArquivo) {
    if (banco->totalUsuarios >= QUANTIDADE_USUARIOS) {
        printf("Erro: Limite de usuários alcançado!\n");
        return;
    }
    Usuario *novoUsuario = (Usuario*) malloc(sizeof(Usuario));
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
    } while (!validarEmail(banco, novoUsuario->email, -1));

    printf("Digite sua senha: ");
    capturarSenha(novoUsuario->senha);

    banco->usuarios[banco->totalUsuarios] = novoUsuario;
    banco->totalUsuarios++;

    quicksortUsuarioPorNome(banco->usuarios, 0, banco->totalUsuarios - 1);

    printf("Usuário cadastrado com sucesso!\n\n");

    salvarUsuarios(banco, nomeArquivo);
}

void listarUsuarios(Banco *banco){
    if(banco->totalUsuarios == 0){
        printf("Nenhum usuário cadastrado.\n");
        return;
    }

    printf("Lista de usuários cadastrados:\n");
    for(int i = 0; i < banco->totalUsuarios; i++){
        Usuario *usuario = banco->usuarios[i];
        printf("Nome: %s | Email: %s | Saldo: R$ %.2f\n", usuario->nome, usuario->email, usuario->saldo);
    }
    printf("\n");
}

void editarUsuario(Banco *banco, const char *nomeArquivo){
    listarUsuarios(banco);
    int indice;
    printf("Selecione o número do usuário que deseja editar: ");
    scanf("%d", &indice);
    indice--; 
    if(indice >= 0 && indice < banco -> totalUsuarios){
        Usuario *usuario = banco->usuarios[indice];
        printf("Digite o novo nome: ");
        getchar(); 
        fgets(banco -> usuarios[indice]->nome, 50, stdin);
        usuario->nome[strcspn(usuario->nome, "\n")] = 0;
        
        do{
            printf("Digite o novo email: ");
            scanf("%s", usuario->email);
        } while (!validarEmail(banco, usuario->email, indice));

        printf("Digite a nova senha: ");
        capturarSenha(usuario->senha);

        printf("Usuário editado com sucesso!\n\n");
    } else {
        printf("Erro: Índice inválido!\n\n");
    }

    salvarUsuarios(banco, nomeArquivo);
}

void excluirUsuario(Banco *banco, const char *nomeArquivo){
    int indice;
    listarUsuarios(banco);        
    printf("Selecione o número do usuário que deseja excluir: ");
    scanf("%d", &indice);
    indice--; 
    
    if(indice >= 0 && indice < banco -> totalUsuarios){
        free(banco -> usuarios[indice]);
        
        for(int i = indice; i < banco -> totalUsuarios - 1; i++){
            banco -> usuarios[i] = banco -> usuarios[i + 1];
        }
        
        banco -> totalUsuarios--;
        printf("Usuário excluído com sucesso!\n\n");
    } else {
        printf("Erro: Índice inválido!\n\n");
    }

    salvarUsuarios(banco, nomeArquivo);
}

int realizarLogin(Banco *banco) {
    char email[80];
    char senha[20];
    for (int tentativas = 3; tentativas > 0; tentativas--){
        printf("Digite seu email: ");
        scanf("%s", email);
        printf("Digite sua senha: ");
        capturarSenha(senha);
        for(int i = 0; i < banco -> totalUsuarios; i++){
            if(strcmp(banco -> usuarios[i]->email, email) == 0 && strcmp(banco -> usuarios[i]->senha, senha) == 0){
                printf("Login realizado com sucesso! Bem-vindo, %s!\n\n", banco -> usuarios[i]->nome);
                return i;
            }
        }
        printf("Email ou senha incorretos! Você tem %d tentativa(s) restante(s).\n\n", tentativas -1);
    }
    printf("Login falhou. Retornando ao menu principal.\n");
    return -1;
}

void acessarMenuCadastrado(Banco *banco, int usuarioID){
    menuUsuario(banco, usuarioID);
}

void menuNaoCadastrado(Banco *banco){
    const char *nomeArquivo = "usuarios.txt";
    int opcao;
    while(1){
        printf("Menu Principal\n");
        printf("1. Cadastrar usuário\n");
        printf("2. Listar usuários\n");
        printf("3. Editar usuário\n");
        printf("4. Excluir usuário\n");
        printf("5. Tela Inicial\n");
        printf("6. Sair do Banco\n");
        printf("\nEscolha uma opção: ");

        opcao = solicitarOpcaoValida(1, 6);

        switch (opcao){     
            case 1: 
                cadastrarUsuario(banco, nomeArquivo);
                break;
            case 2: 
                listarUsuarios(banco);
                break;
            case 3:
                editarUsuario(banco, nomeArquivo);
                break;
            case 4: 
                excluirUsuario(banco, nomeArquivo);
                break;
            case 5:         
                return;
            case 6: 
                encerrarPrograma(banco, nomeArquivo);
                break;
            default:
                printf("Opção inválida!\n");
        }
    } 
}

void menuUsuario(Banco *banco, int usuarioID){
    const char *nomeArquivo = "usuarios.txt";
    int opcao;
    while(1){
        printf("Menu do Usuário\n");
        printf("1. Verificar Saldo\n");
        printf("2. Realizar Depósito\n");
        printf("3. Realizar Saque\n");
        printf("4. Alterar Senha\n");
        printf("5. Realizar Transferências\n");
        printf("6. Buscar Usuário por Email\n");
        printf("7. Acessar o Menu Principal\n");
        printf("\nEscolha uma opção: ");

        opcao = solicitarOpcaoValida(1, 7);

        switch (opcao){
            case 1:
                verificarSaldo(banco, usuarioID);
                break;
            case 2:
                realizarDeposito(banco, usuarioID, nomeArquivo);
                break;
            case 3:
                realizarSaque(banco, usuarioID, nomeArquivo);
                break;
            case 4:
                alterarSenha(banco, usuarioID, nomeArquivo);
                break;
            case 5:
                realizarTransferencia(banco, usuarioID, nomeArquivo);
                break;
            case 6:
                buscarUsuarioPorEmailComQuicksort(banco);
                break;
            case 7:
                return;
            default:
                printf("Opção inválida! Tente novamente.\n\n");
        }
    } 
}

int validarEmail(Banco *banco, const char *email, int indiceUsuarioAtual){
    
    if(strchr(email, '@') == NULL){
        printf("Erro: Email inválido, deve conter '@'!\n");
        return 0;
    }

    for (int i = 0; i < banco -> totalUsuarios; i++){
        if (i != indiceUsuarioAtual && strcmp (banco->usuarios[i]->email, email) == 0){
            printf("Erro: Email já cadastrado!\n");
            return 0;
        }
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

void quicksortUsuarioPorNome(Usuario **usuarios, int inicio, int fim){
    if(inicio < fim){
        int indicePivo = particionarPorNome(usuarios, inicio, fim);

        quicksortUsuarioPorNome(usuarios, inicio, indicePivo - 1);
        quicksortUsuarioPorNome(usuarios, indicePivo + 1, fim);
    }
}

void ordenarUsuarioPorNome(Banco *banco){
    quicksortUsuarioPorNome(banco->usuarios, 0, banco->totalUsuarios - 1);
}

void trocar(Usuario **a, Usuario **b){
    Usuario *temp = *a;
    *a = *b;
    *b = temp;
}

int particionarPorNome(Usuario **usuarios, int inicio, int fim){
    Usuario *pivo = usuarios[fim];
    int i = inicio - 1;

    for(int j = inicio; j < fim; j++){
        if (strcasecmp(usuarios[j]->nome, pivo->nome) < 0){
            i++;
            trocar(&usuarios[i], &usuarios[j]);
        }
    }
    trocar(&usuarios[i + 1], &usuarios[fim]);
    return i + 1;
} 

void buscarUsuarioPorEmailComQuicksort(Banco *banco){
    if (banco->totalUsuarios == 0){
        printf("Nenhum usuário cadastrado.\n");
        return;
    } 

    quicksortUsuarioPorNome(banco->usuarios, 0, banco->totalUsuarios - 1);

    char email[80];
    printf("Digite o email do usuário que deseja buscar: ");
    scanf("%s", email);

    int indice = buscaBinariaRecursiva(banco, email, 0, banco->totalUsuarios - 1);

    if(indice != -1){
        Usuario *usuario = banco->usuarios[indice];
        printf("Usuário encontrado:\n");
        printf("Nome: %s\nEmail: %s\nSaldo: R$%.2f\n", usuario->nome, usuario->email, usuario->saldo);
    } else {
        printf("Usuário com o email %s nao encontrado.\n", email);
    }
}

int buscaBinariaRecursiva(Banco *banco, const char *email, int inicio, int fim){
    if (inicio > fim){
        return -1;
    }

    int meio = (inicio + fim) / 2;
    int comparacao = strcmp(banco->usuarios[meio]-> email, email);

    if(comparacao == 0) {
        return meio;
    } else if (comparacao < 0){
        return buscaBinariaRecursiva(banco, email, meio + 1, fim);
    } else {
        return buscaBinariaRecursiva(banco, email, inicio, meio -1);
    }
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
    printf("Seu saldo é: R$ %.2f\n\n", banco->usuarios[usuarioID]->saldo);
}

void realizarDeposito(Banco *banco, int usuarioID, const char *nomeArquivo){
    printf("Digite o valor do depósito: R$ ");
    float valor = solicitarValorPositivo();
    banco -> usuarios[usuarioID]->saldo += valor;
    printf("Depósito de R$ %.2f realizado com sucesso!\n\n", valor);

    salvarUsuarios(banco, nomeArquivo);
}

void realizarSaque(Banco *banco, int usuarioID, const char *nomeArquivo){
    printf("Digite o valor do saque: R$ ");
    float valor = solicitarValorPositivo();
    if(valor <= banco -> usuarios[usuarioID]->saldo){
        banco -> usuarios[usuarioID]->saldo -= valor;
        printf("Saque de R$ %.2f realizado com sucesso!\n\n", valor);
    } else {
        printf("Saldo insuficiente!\n\n");
    }

    salvarUsuarios(banco, nomeArquivo);
}

void alterarSenha(Banco *banco, int usuarioID, const char *nomeArquivo){
    printf("Digite sua nova senha: ");
    capturarSenha(banco -> usuarios[usuarioID]->senha);
    printf("Senha alterada com sucesso!\n\n");

    salvarUsuarios(banco, nomeArquivo);
}

void realizarTransferencia(Banco *banco, int usuarioID, const char *nomeArquivo){
    int destinatarioID;
    listarUsuarios(banco);
    printf("Selecione o número do usuário para quem deseja transferir: ");
    scanf("%d", &destinatarioID);
    destinatarioID--; 
    if(destinatarioID >= 0 && destinatarioID < banco -> totalUsuarios && destinatarioID != usuarioID){     
        printf("Digite o valor da transferência: ");
        float valor = solicitarValorPositivo();
    if(valor <= banco -> usuarios[usuarioID]->saldo){
        banco -> usuarios[usuarioID]->saldo -=valor;
        banco -> usuarios[destinatarioID]->saldo += valor;
        printf("Transferência de R$ %.2f realizada com sucesso para %s!\n\n", valor, banco -> usuarios[destinatarioID]->email);
    } else{
        printf("Saldo Insuficiente!\n\n");
        }       
    } else{
        printf("Erro: Usuário inválido!\n\n");
    }

    salvarUsuarios(banco, nomeArquivo);
}




