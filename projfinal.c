#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <ncurses.h> // Biblioteca para interface no Linux

#define MAX_ALUNOS 100
#define NOME_TAM 50
#define CURSO_TAM 50
#define EMAIL_TAM 50
#define TELEFONE_TAM 15

typedef struct {
    int id;
    char nome[NOME_TAM];
    int idade;
    char curso[CURSO_TAM];
    char email[EMAIL_TAM];
    char telefone[TELEFONE_TAM];
} Aluno;

Aluno alunos[MAX_ALUNOS];
int total_alunos = 0;

// Função para limpar a tela
void limparTela() {
    system("clear"); // Para Linux
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para salvar os dados no arquivo binário
void salvarDados() {
    FILE *arquivo = fopen("alunos.dat", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar dados");
        return;
    }
    // Salva o número total de alunos
    if (fwrite(&total_alunos, sizeof(int), 1, arquivo) != 1) {
        perror("Erro ao salvar número de alunos");
        fclose(arquivo);
        return;
    }
    // Salva os dados dos alunos
    if (fwrite(alunos, sizeof(Aluno), total_alunos, arquivo) != total_alunos) {
        perror("Erro ao salvar dados dos alunos");
    }
    fclose(arquivo);
}

// Função para carregar os dados do arquivo binário
void carregarDados() {
    FILE *arquivo = fopen("alunos.dat", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para carregar dados");
        return;
    }
    // Carrega o número total de alunos
    if (fread(&total_alunos, sizeof(int), 1, arquivo) != 1) {
        perror("Erro ao carregar número de alunos");
        fclose(arquivo);
        return;
    }
    // Carrega os dados dos alunos
    if (fread(alunos, sizeof(Aluno), total_alunos, arquivo) != total_alunos) {
        perror("Erro ao carregar dados dos alunos");
    }
    fclose(arquivo);
}

// Função para validar o email (simples)
int validarEmail(const char *email) {
    return strchr(email, '@') != NULL; // Verifica se há um '@' no email
}

// Função para validar o telefone (simples)
int validarTelefone(const char *telefone) {
    if (strlen(telefone) != 11) { // Telefone deve ter 11 dígitos
        return 0;
    }
    for (int i = 0; telefone[i] != '\0'; i++) {
        if (!isdigit(telefone[i])) {
            return 0; // Telefone deve conter apenas dígitos
        }
    }
    return 1;
}

// Função para validar o nome (permite caracteres especiais)
int validarNome(const char *nome) {
    if (strlen(nome) == 0 || strlen(nome) > NOME_TAM - 1) { // Verifica se está em branco ou excede o tamanho
        return 0;
    }
    return 1; // Permite qualquer caractere, exceto deixar em branco
}

// Função para validar o curso (permite caracteres especiais)
int validarCurso(const char *curso) {
    if (strlen(curso) == 0 || strlen(curso) > CURSO_TAM - 1) { // Verifica se está em branco ou excede o tamanho
        return 0;
    }
    return 1; // Permite qualquer caractere, exceto deixar em branco
}

// Função para cadastrar um novo aluno
void cadastrarAluno() {
    if (total_alunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido!\n");
        return;
    }
    Aluno novoAluno;
    novoAluno.id = total_alunos + 1;

    // Validação do nome
    do {
        printf("Nome (máximo de 50 caracteres, pode conter caracteres especiais): ");
        fgets(novoAluno.nome, NOME_TAM, stdin);
        novoAluno.nome[strcspn(novoAluno.nome, "\n")] = 0; // Remove o '\n'
        if (!validarNome(novoAluno.nome)) {
            printf("Nome inválido! Não pode ser deixado em branco e deve ter até 50 caracteres.\n");
        }
    } while (!validarNome(novoAluno.nome));

    // Validação da idade
    do {
        printf("Idade: ");
        char idadeInput[10];
        fgets(idadeInput, sizeof(idadeInput), stdin);
        if (strlen(idadeInput) == 1) { // Se o usuário pressionar Enter sem digitar nada
            printf("Idade não pode ser deixada em branco! Tente novamente.\n");
            continue;
        }
        if (sscanf(idadeInput, "%d", &novoAluno.idade) != 1 || novoAluno.idade < 0) {
            printf("Idade inválida! Digite um número positivo.\n");
        } else {
            break;
        }
    } while (1);

    // Validação do curso
    do {
        printf("Curso (máximo de 50 caracteres, pode conter caracteres especiais): ");
        fgets(novoAluno.curso, CURSO_TAM, stdin);
        novoAluno.curso[strcspn(novoAluno.curso, "\n")] = 0;
        if (!validarCurso(novoAluno.curso)) {
            printf("Curso inválido! Não pode ser deixado em branco e deve ter até 50 caracteres.\n");
        }
    } while (!validarCurso(novoAluno.curso));

    // Validação do email
    do {
        printf("Email: ");
        fgets(novoAluno.email, EMAIL_TAM, stdin);
        novoAluno.email[strcspn(novoAluno.email, "\n")] = 0;
        if (!validarEmail(novoAluno.email)) {
            printf("Email inválido! Deve conter '@'. Tente novamente.\n");
        }
    } while (!validarEmail(novoAluno.email));

    // Validação do telefone
    do {
        printf("Telefone (11 dígitos, apenas números): ");
        fgets(novoAluno.telefone, TELEFONE_TAM, stdin);
        novoAluno.telefone[strcspn(novoAluno.telefone, "\n")] = 0;
        if (!validarTelefone(novoAluno.telefone)) {
            printf("Telefone inválido! Deve conter exatamente 11 dígitos e apenas números.\n");
        }
    } while (!validarTelefone(novoAluno.telefone));

    alunos[total_alunos++] = novoAluno;
    salvarDados();
    printf("Aluno cadastrado com sucesso!\n");
    printf("Pressione Enter para voltar ao menu...");
    limparBuffer(); // Aguarda o usuário pressionar Enter
    limparTela(); // Limpa a tela após o cadastro
}

// Função para listar todos os alunos
void listarAlunos() {
    if (total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
    } else {
        for (int i = 0; i < total_alunos; i++) {
            printf("ID: %d, Nome: %s, Idade: %d, Curso: %s, Email: %s, Telefone: %s\n",
                   alunos[i].id, alunos[i].nome, alunos[i].idade, alunos[i].curso, alunos[i].email, alunos[i].telefone);
        }
    }
    printf("Pressione Enter para voltar ao menu...");
    limparBuffer(); // Aguarda o usuário pressionar Enter
    limparTela(); // Limpa a tela após listar
}

// Função para editar um aluno existente
void editarAluno() {
    int id;
    printf("Insira o ID do aluno que você deseja editar: ");
    scanf("%d", &id);
    limparBuffer();

    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id == id) {
            // Validação do nome
            do {
                printf("Novo nome (máximo de 50 caracteres, pode conter caracteres especiais): ");
                fgets(alunos[i].nome, NOME_TAM, stdin);
                alunos[i].nome[strcspn(alunos[i].nome, "\n")] = 0;
                if (!validarNome(alunos[i].nome)) {
                    printf("Nome inválido! Não pode ser deixado em branco e deve ter até 50 caracteres.\n");
                }
            } while (!validarNome(alunos[i].nome));

            // Validação da idade
            do {
                printf("Nova idade: ");
                char idadeInput[10];
                fgets(idadeInput, sizeof(idadeInput), stdin);
                if (strlen(idadeInput) == 1) { // Se o usuário pressionar Enter sem digitar nada
                    printf("Idade não pode ser deixada em branco! Tente novamente.\n");
                    continue;
                }
                if (sscanf(idadeInput, "%d", &alunos[i].idade) != 1 || alunos[i].idade < 0) {
                    printf("Idade inválida! Digite um número positivo.\n");
                } else {
                    break;
                }
            } while (1);

            // Validação do curso
            do {
                printf("Novo curso (máximo de 50 caracteres, pode conter caracteres especiais): ");
                fgets(alunos[i].curso, CURSO_TAM, stdin);
                alunos[i].curso[strcspn(alunos[i].curso, "\n")] = 0;
                if (!validarCurso(alunos[i].curso)) {
                    printf("Curso inválido! Não pode ser deixado em branco e deve ter até 50 caracteres.\n");
                }
            } while (!validarCurso(alunos[i].curso));

            // Validação do email
            do {
                printf("Novo email: ");
                fgets(alunos[i].email, EMAIL_TAM, stdin);
                alunos[i].email[strcspn(alunos[i].email, "\n")] = 0;
                if (!validarEmail(alunos[i].email)) {
                    printf("Email inválido! Deve conter '@'. Tente novamente.\n");
                }
            } while (!validarEmail(alunos[i].email));

            // Validação do telefone
            do {
                printf("Novo telefone (11 dígitos, apenas números): ");
                fgets(alunos[i].telefone, TELEFONE_TAM, stdin);
                alunos[i].telefone[strcspn(alunos[i].telefone, "\n")] = 0;
                if (!validarTelefone(alunos[i].telefone)) {
                    printf("Telefone inválido! Deve conter exatamente 11 dígitos e apenas números.\n");
                }
            } while (!validarTelefone(alunos[i].telefone));

            salvarDados();
            printf("Aluno atualizado com sucesso!\n");
            printf("Pressione Enter para voltar ao menu...");
            limparBuffer(); // Aguarda o usuário pressionar Enter
            limparTela(); // Limpa a tela após a edição
            return;
        }
    }
    printf("Aluno não encontrado.\n");
    printf("Pressione Enter para voltar ao menu...");
    limparBuffer(); // Aguarda o usuário pressionar Enter
    limparTela(); // Limpa a tela após a mensagem
}

// Função para excluir um aluno
void excluirAluno() {
    int id;
    printf("Insira o ID do aluno que você deseja excluir: ");
    scanf("%d", &id);

    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id == id) {
            char confirmacao;
            printf("Você realmente deseja excluir o cadastro do aluno %s? (S/N): ", alunos[i].nome);
            scanf(" %c", &confirmacao);
            if (tolower(confirmacao) == 's') {
                for (int j = i; j < total_alunos - 1; j++) {
                    alunos[j] = alunos[j + 1];
                }
                total_alunos--;
                salvarDados();
                printf("Aluno excluído com sucesso!\n");
            } else {
                printf("Exclusão cancelada.\n");
            }
            printf("Pressione Enter para voltar ao menu...");
            limparBuffer(); // Aguarda o usuário pressionar Enter
            limparTela(); // Limpa a tela após a exclusão
            return;
        }
    }
    printf("Aluno não encontrado.\n");
    printf("Pressione Enter para voltar ao menu...");
    limparBuffer(); // Aguarda o usuário pressionar Enter
    limparTela(); // Limpa a tela após a mensagem
}

// Função para exibir o menu com navegação por setas
int menuInterativo() {
    int opcao = 1;
    int tecla;

    initscr(); // Inicializa a tela do ncurses
    cbreak(); // Desabilita o buffer de linha
    noecho(); // Desabilita a exibição de caracteres digitados
    keypad(stdscr, TRUE); // Habilita a captura de teclas especiais
    curs_set(0); // Esconde o cursor

    // Desenha o menu inicial
    mvprintw(0, 0, "--- Sistema de Cadastro de Alunos ---\n");
    mvprintw(1, 0, "> 1. Cadastrar Aluno\n");
    mvprintw(2, 0, "  2. Listar Alunos\n");
    mvprintw(3, 0, "  3. Editar Aluno\n");
    mvprintw(4, 0, "  4. Excluir Aluno\n");
    mvprintw(5, 0, "  5. Sair\n");
    mvprintw(6, 0, "Use as setas para navegar e Enter para selecionar.\n");
    refresh();

    while (1) {
        tecla = getch();
        switch (tecla) {
            case KEY_UP:
                if (opcao > 1) {
                    opcao--;
                    // Atualiza apenas o indicador
                    for (int i = 1; i <= 5; i++) {
                        if (i == opcao) {
                            mvprintw(i, 0, "> ");
                        } else {
                            mvprintw(i, 0, "  ");
                        }
                    }
                    refresh();
                }
                break;
            case KEY_DOWN:
                if (opcao < 5) {
                    opcao++;
                    // Atualiza apenas o indicador
                    for (int i = 1; i <= 5; i++) {
                        if (i == opcao) {
                            mvprintw(i, 0, "> ");
                        } else {
                            mvprintw(i, 0, "  ");
                        }
                    }
                    refresh();
                }
                break;
            case 10: // Enter
                endwin(); // Finaliza o modo ncurses
                return opcao;
        }
    }
}

int main() {
    carregarDados();
    int opcao;
    do {
        opcao = menuInterativo();
        switch (opcao) {
            case 1:
                cadastrarAluno();
                break;
            case 2:
                listarAlunos();
                break;
            case 3:
                editarAluno();
                break;
            case 4:
                excluirAluno();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                printf("Pressione Enter para voltar ao menu...");
                limparBuffer(); // Aguarda o usuário pressionar Enter
                limparTela(); // Limpa a tela após a mensagem
        }
    } while (opcao != 5);
    return 0;
}