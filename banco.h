#define BANCO_H

#define QUANTIDADE_USUARIOS 100

typedef struct {
    char nome[50];
    char email[80];
    char senha[20];
    float saldo;
} Usuario;

typedef struct {
    Usuario *usuarios[QUANTIDADE_USUARIOS];
    int totalUsuarios;
} Banco;

// Declarações das funções
void exibirBoasVindas();
void encerrarPrograma();
Banco* criarBanco();
void carregarUsuarios(Banco *banco, const char *nomeArquivo);
void salvarUsuarios(Banco *banco, const char *nomeArquivo);
void cadastrarUsuario(Banco *banco, const char *nomeArquivo);
void listarUsuarios(Banco *banco);
void editarUsuario(Banco *banco, const char *nomeArquivo);
void excluirUsuario(Banco *banco, const char *nomeArquivo);
int realizarLogin(Banco *banco);
void acessarMenuCadastrado(Banco *banco, int usuarioID);
void menuNaoCadastrado(Banco *banco);
void menuUsuario(Banco *banco, int usuarioID);
int validarEmail(Banco *banco, const char *email, int indiceUsuarioAtual);
void capturarSenha(char *senha);
void quicksortUsuarioPorNome(Usuario **usuarios, int inicio, int fim);
void ordenarUsuarioPorNome(Banco *banco);
void trocar(Usuario **a, Usuario **b);
int particionarPorNome(Usuario **usuarios, int inicio, int fim);
void buscarUsuarioPorEmailComQuicksort(Banco *banco);
int buscaBinariaRecursiva(Banco *banco, const char *email, int inicio, int fim);
float solicitarValorPositivo();
int solicitarOpcaoValida(int min, int max);
void verificarSaldo(Banco *banco, int usuarioID);
void realizarDeposito(Banco *banco, int usuarioID, const char *nomeArquivo);
void realizarSaque(Banco *banco, int usuarioID, const char *nomeArquivo);
void alterarSenha(Banco *banco, int usuarioID, const char *nomeArquivo);
void realizarTransferencia(Banco *banco, int usuarioID, const char *nomeArquivo);