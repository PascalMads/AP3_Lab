#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

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

void limparTela() {
    system("clear");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void salvarDados() {
    FILE *arquivo = fopen("alunos.dat", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar dados");
        return;
    }
    if (fwrite(&total_alunos, sizeof(int), 1, arquivo) != 1) {
        perror("Erro ao salvar número de alunos");
        fclose(arquivo);
        return;
    }
    if (fwrite(alunos, sizeof(Aluno), total_alunos, arquivo) != total_alunos) {
        perror("Erro ao salvar dados dos alunos");
    }
    fclose(arquivo);
}

void carregarDados() {
    FILE *arquivo = fopen("alunos.dat", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para carregar dados");
        return;
    }
    if (fread(&total_alunos, sizeof(int), 1, arquivo) != 1) {
        perror("Erro ao carregar número de alunos");
        fclose(arquivo);
        return;
    }
    if (fread(alunos, sizeof(Aluno), total_alunos, arquivo) != total_alunos) {
        perror("Erro ao carregar dados dos alunos");
    }
    fclose(arquivo);
}

int validarEmail(const char *email) {
    return strchr(email, '@') != NULL;
}

int validarTelefone(const char *telefone) {
    if (strlen(telefone) != 11) {
        return 0;
    }
    for (int i = 0; telefone[i] != '\0'; i++) {
        if (!isdigit(telefone[i])) {
            return 0;
        }
    }
    return 1;
}

int validarNome(const char *nome) {
    return strlen(nome) > 0 && strlen(nome) <= NOME_TAM - 1;
}

int validarCurso(const char *curso) {
    return strlen(curso) > 0 && strlen(curso) <= CURSO_TAM - 1;
}

void cadastrarAluno() {
    if (total_alunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido!\n");
        return;
    }
    Aluno novoAluno;
    novoAluno.id = total_alunos + 1;

    do {
        printf("Nome (máximo de 50 caracteres): ");
        fgets(novoAluno.nome, NOME_TAM, stdin);
        novoAluno.nome[strcspn(novoAluno.nome, "\n")] = 0;
    } while (!validarNome(novoAluno.nome));

    do {
        printf("Idade: ");
        char idadeInput[10];
        fgets(idadeInput, sizeof(idadeInput), stdin);
        if (sscanf(idadeInput, "%d", &novoAluno.idade) != 1 || novoAluno.idade < 0) {
            printf("Idade inválida!\n");
        } else break;
    } while (1);

    do {
        printf("Curso: ");
        fgets(novoAluno.curso, CURSO_TAM, stdin);
        novoAluno.curso[strcspn(novoAluno.curso, "\n")] = 0;
    } while (!validarCurso(novoAluno.curso));

    do {
        printf("Email: ");
        fgets(novoAluno.email, EMAIL_TAM, stdin);
        novoAluno.email[strcspn(novoAluno.email, "\n")] = 0;
    } while (!validarEmail(novoAluno.email));

    do {
        printf("Telefone (11 dígitos): ");
        fgets(novoAluno.telefone, TELEFONE_TAM, stdin);
        novoAluno.telefone[strcspn(novoAluno.telefone, "\n")] = 0;
    } while (!validarTelefone(novoAluno.telefone));

    alunos[total_alunos++] = novoAluno;
    salvarDados();
    printf("Aluno cadastrado!\n");
    limparBuffer();
    limparTela();
}

void listarAlunos() {
    if (total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
    } else {
        for (int i = 0; i < total_alunos; i++) {
            printf("ID: %d\nNome: %s\nIdade: %d\nCurso: %s\nEmail: %s\nTelefone: %s\n\n",
                   alunos[i].id, alunos[i].nome, alunos[i].idade, alunos[i].curso, alunos[i].email, alunos[i].telefone);
        }
    }
    printf("Pressione Enter...");
    limparBuffer();
    limparTela();
}

void editarAluno() {
    int id;
    printf("ID do aluno: ");
    scanf("%d", &id);
    limparBuffer();

    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id == id) {
            do {
                printf("Novo nome: ");
                fgets(alunos[i].nome, NOME_TAM, stdin);
                alunos[i].nome[strcspn(alunos[i].nome, "\n")] = 0;
            } while (!validarNome(alunos[i].nome));

            do {
                printf("Nova idade: ");
                char idadeInput[10];
                fgets(idadeInput, sizeof(idadeInput), stdin);
                if (sscanf(idadeInput, "%d", &alunos[i].idade) != 1 || alunos[i].idade < 0) {
                    printf("Idade inválida!\n");
                } else break;
            } while (1);

            do {
                printf("Novo curso: ");
                fgets(alunos[i].curso, CURSO_TAM, stdin);
                alunos[i].curso[strcspn(alunos[i].curso, "\n")] = 0;
            } while (!validarCurso(alunos[i].curso));

            do {
                printf("Novo email: ");
                fgets(alunos[i].email, EMAIL_TAM, stdin);
                alunos[i].email[strcspn(alunos[i].email, "\n")] = 0;
            } while (!validarEmail(alunos[i].email));

            do {
                printf("Novo telefone: ");
                fgets(alunos[i].telefone, TELEFONE_TAM, stdin);
                alunos[i].telefone[strcspn(alunos[i].telefone, "\n")] = 0;
            } while (!validarTelefone(alunos[i].telefone));

            salvarDados();
            printf("Aluno atualizado!\n");
            limparBuffer();
            limparTela();
            return;
        }
    }
    printf("Aluno não encontrado.\n");
    limparBuffer();
    limparTela();
}

void excluirAluno() {
    int id;
    printf("ID do aluno: ");
    scanf("%d", &id);
    limparBuffer();

    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id == id) {
            printf("Excluir %s? (S/N): ", alunos[i].nome);
            char confirmacao = getchar();
            if (tolower(confirmacao) == 's') {
                for (int j = i; j < total_alunos - 1; j++) {
                    alunos[j] = alunos[j + 1];
                }
                total_alunos--;
                salvarDados();
                printf("Aluno excluído!\n");
            }
            limparBuffer();
            limparTela();
            return;
        }
    }
    printf("Aluno não encontrado.\n");
    limparBuffer();
    limparTela();
}

void configurarTerminal(struct termios *original) {
    struct termios novo;
    tcgetattr(STDIN_FILENO, original);
    novo = *original;
    novo.c_lflag &= ~(ICANON | ECHO);
    novo.c_cc[VMIN] = 1;
    novo.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &novo);
    printf("\033[?25l");
    fflush(stdout);
}

void restaurarTerminal(struct termios *original) {
    tcsetattr(STDIN_FILENO, TCSANOW, original);
    printf("\033[?25h");
    fflush(stdout);
}

int menuInterativo() {
    struct termios original;
    configurarTerminal(&original);

    int opcao = 1;
    unsigned char buf[3];

    while (1) {
        system("clear");
        printf("=== Sistema de Cadastro ===\n");
        printf("%s1. Cadastrar Aluno\n", opcao == 1 ? "> " : "  ");
        printf("%s2. Listar Alunos\n", opcao == 2 ? "> " : "  ");
        printf("%s3. Editar Aluno\n", opcao == 3 ? "> " : "  ");
        printf("%s4. Excluir Aluno\n", opcao == 4 ? "> " : "  ");
        printf("%s5. Sair\n", opcao == 5 ? "> " : "  ");
        printf("Use setas ↑↓ e Enter\n");

        int n = read(STDIN_FILENO, buf, 1);
        if (n == 1) {
            if (buf[0] == 27) {
                if (read(STDIN_FILENO, buf + 1, 2) == 2) {
                    if (buf[1] == '[') {
                        switch(buf[2]) {
                            case 'A': if (opcao > 1) opcao--; break;
                            case 'B': if (opcao < 5) opcao++; break;
                        }
                    }
                }
            } else if (buf[0] == '\n') {
                restaurarTerminal(&original);
                return opcao;
            }
        }
    }
}

int main() {
    carregarDados();
    int opcao;
    do {
        opcao = menuInterativo();
        switch(opcao) {
            case 1: cadastrarAluno(); break;
            case 2: listarAlunos(); break;
            case 3: editarAluno(); break;
            case 4: excluirAluno(); break;
            case 5: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 5);
    return 0;
}