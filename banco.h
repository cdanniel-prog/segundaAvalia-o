#define BANCO_H

#define QUANTIDADE_USUARIOS 100

typedef struct Usuario {
    char nome[50];
    char email[80];
    char senha[20];
    float saldo;
    struct Usuario *prox;
} Usuario;

typedef struct Banco {
    Usuario *usuarios;
    int totalUsuarios;
} Banco;

void exibirBoasVindas();
void encerrarPrograma();
Banco* criarBanco();
Usuario* merge(Usuario *esquerdo, Usuario *direita);
void dividir(Usuario *fonte, Usuario **esquerda, Usuario **direita);
void mergeSort(Usuario **cabecaRef);
void ordenarUsuariosPorNome(Banco *banco);
void carregarUsuarios(Banco *banco, const char *nomeArquivo);
void salvarUsuarios(Banco *banco, const char *nomeArquivo);
void inserirUsuario(Banco *banco, Usuario *novoUsuario);
void cadastrarUsuario(Banco *banco, const char *nomeArquivo);
void listarUsuarios(Banco *banco);
void editarUsuario(Banco *banco, const char *nomeArquivo);
void excluirUsuario(Banco *banco, const char *nomeArquivo);
int realizarLogin(Banco *banco);
void acessarMenuCadastrado(Banco *banco, int usuarioID);
void menuNaoCadastrado(Banco *banco);
void menuUsuario(Banco *banco, int usuarioID);
void capturarSenha(char *senha);
float solicitarValorPositivo();
int solicitarOpcaoValida(int min, int max);
void verificarSaldo(Banco *banco, int usuarioID);
void realizarDeposito(Banco *banco, int usuarioID, const char *nomeArquivo);
void realizarSaque(Banco *banco, int usuarioID, const char *nomeArquivo);
void alterarSenha(Banco *banco, int usuarioID, const char *nomeArquivo);
void realizarTransferencia(Banco *banco, int usuarioID, const char *nomeArquivo);
int validarEmail(Banco *banco, const char *email, Usuario *usuarioAtual);


