#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

//MAXIMO DE TERRITORIOS, QUANTIDADE DE CARACTER DO NOME, MISSOES, CATACTERES COR
#define NUM_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10
#define NUM_MISSOES 2


typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;


Territorio* alocarMapa(int num);
void inicializarTerritorios(Territorio* mapa, int num);
void liberarMemoria(Territorio* mapa);


void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int num);
void exibirMissao(int missaoID);


void faseDeAtaque(Territorio* mapa, int num);
void simularAtaque(Territorio* origem, Territorio* destino);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int num, int missaoID);


void limparBufferEntrada();

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (!mapa) {
        printf("Erro ao alocar memória para o mapa!\n");
        return 1;
    }

    inicializarTerritorios(mapa, NUM_TERRITORIOS);
    int missaoID = sortearMissao();

    int opcao;
    do {
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissao(missaoID);
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS);
                break;
            case 2:
                if(verificarVitoria(mapa, NUM_TERRITORIOS, missaoID))
                    printf("Parabéns! Missão cumprida!\n");
                else
                    printf("Missão ainda não cumprida.\n");
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while(opcao != 0);

    liberarMemoria(mapa);
    return 0;
}


Territorio* alocarMapa(int num) {
    return (Territorio*) calloc(num, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa, int num) {
    for(int i=0; i<num; i++) {
        printf("Cadastro do Territorio %d\n", i+1);
        printf("Nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exercito: ");
        fgets(mapa[i].cor, MAX_COR, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
        printf("\n");
    }
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// Interface
void exibirMenuPrincipal() {
    printf("\n=== Menu ===\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

void exibirMapa(const Territorio* mapa, int num) {
    printf("\n=== Mapa Atual ===\n");
    for(int i=0; i<num; i++) {
        printf("%d - %s | %s | Tropas: %d\n", i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirMissao(int missaoID) {
    printf("\n=== Missão ===\n");
    switch(missaoID) {
        case 0:
            printf("Destruir o exército vermelho.\n");
            break;
        case 1:
            printf("Conquistar 3 territórios.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
    }
}

// Lógica
void faseDeAtaque(Territorio* mapa, int num) {
    int origem, destino;
    printf("Digite o número do território de origem: ");
    scanf("%d", &origem);
    printf("Digite o número do território de destino: ");
    scanf("%d", &destino);
    limparBufferEntrada();

    if(origem<1 || origem>num || destino<1 || destino>num) {
        printf("Território inválido!\n");
        return;
    }

    simularAtaque(&mapa[origem-1], &mapa[destino-1]);
}

void simularAtaque(Territorio* origem, Territorio* destino) {
    printf("Atacando %s de %s!\n", destino->nome, origem->nome);
    if(origem->tropas > destino->tropas) {
        printf("Conquista bem sucedida!\n");
        destino->tropas = origem->tropas - destino->tropas;
        strcpy(destino->cor, origem->cor);
        origem->tropas = 1;
    } else {
        printf("Ataque falhou.\n");
        origem->tropas = 1;
    }
}

int sortearMissao() {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio* mapa, int num, int missaoID) {
    int count = 0;
    switch(missaoID) {
        case 0: 
            for(int i=0;i<num;i++)
                if(strcmp(mapa[i].cor,"vermelho")==0) return 0;
            return 1;
        case 1: 
            for(int i=0;i<num;i++)
                if(strcmp(mapa[i].cor,"azul")==0) count++;
            return (count>=3);
        default:
            return 0;
    }
}

//LIMPAR
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
