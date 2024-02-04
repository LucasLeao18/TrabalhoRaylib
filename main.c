// PROFESSOR POR FAVOR LEIA ANTES DE RODAR
// AO RODAR O CIDIGO CERTIFIQUE-SE QUE O ARQUIVO BOUNCE.wav ESTA NA MESMA PASTA DO PROGRAMA
// E TAMBEM QUE O DIRETORIO NO CODIGO FOI ATUALIZADO PARA O DIRETORIO DO SEU COMPUTADOR LINHA 81 DO CODIGO
//
// O JOGO USADO COMO BASE FOI O PONG DA PROPRIA BIBLIOTECA DO RAYLIB
// MODIFICACOES -
//  ADICAO DOS PLACARES DE CADA JOGDAR
//  AUMENTO DO TAMNANHO DA RAQUETE
//  IMPLEMTACAO DAS CORES DE FUNDO
//  PAGINA INICIAL NO JOGO ADICIONADA
//  INVERSAO DO CAMPO ELE ERA NA HORIZONTAL E PASSOUA SER NA VERTICAL
//  ADICINONAMOS UMA LINHA DE MEIO CAMPO PARA PERSNALIZAR O CAMPO
//  REFORMULACAO NA MODULARIZACAO DO CODIGO MELHOR O CODIGO
//  ADICIONAMOS O SOM DA BOLA AO BATER NAS PAREDES OU NOS JOGADORES

#include "raylib.h"

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define VELOCIDADE_RAQUETE 8.0f
#define VELOCIDADE_BOLA 5.0f
#define TAMANHO_RAQUETE_FATOR 1.3f
#define RAIO_BOLA 10

#define COR_FUNDO BLUE
#define COR_TITULO BLACK
#define COR_TEXTO BLACK
#define COR_RAQUETE RED
#define COR_BOLA GREEN
#define COR_LINHA GRAY
#define COR_CIRCULO_OCO RED

typedef struct
{
    Rectangle raqueteTopo;
    Rectangle raqueteBase;
    Vector2 posicaoBola;
    Vector2 velocidadeBola;
    bool jogoIniciado;

    int scoreTopo;
    int scoreBase;

    Sound bounceSound;
} Jogo;

void InicializarJogo(Jogo *jogo);
void AtualizarJogo(Jogo *jogo);
void DesenharJogo(const Jogo *jogo);
void FinalizarJogo(Jogo *jogo);

int main()
{

    InitAudioDevice();

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Pong");
    SetTargetFPS(60);

    Jogo jogo;
    InicializarJogo(&jogo);

    while (!WindowShouldClose())
    {
        AtualizarJogo(&jogo);
        DesenharJogo(&jogo);
    }


    CloseAudioDevice();
    CloseWindow();
    FinalizarJogo(&jogo);
    return 0;
}

void InicializarJogo(Jogo *jogo)
{
    float tamanhoRaquete = 120 * TAMANHO_RAQUETE_FATOR;
    float raqueteY = 10;

    jogo->raqueteTopo = (Rectangle){LARGURA_TELA / 2 - tamanhoRaquete / 2, raqueteY, tamanhoRaquete, 20};
    jogo->raqueteBase = (Rectangle){LARGURA_TELA / 2 - tamanhoRaquete / 2, ALTURA_TELA - 30, tamanhoRaquete, 20};
    jogo->posicaoBola = (Vector2){LARGURA_TELA / 2, ALTURA_TELA / 2};
    jogo->velocidadeBola = (Vector2){VELOCIDADE_BOLA, VELOCIDADE_BOLA};
    jogo->jogoIniciado = false;

    jogo->scoreTopo = 0;
    jogo->scoreBase = 0;


    jogo->bounceSound = LoadSound("C:\\Users\\AzupixLS\\Desktop\\Programing\\testersy\\bounce.wav");
}

void AtualizarJogo(Jogo *jogo)
{
    if (!jogo->jogoIniciado)
    {
        if (IsKeyPressed(KEY_ENTER))
            jogo->jogoIniciado = true;
    }
    else
    {
        if (IsKeyDown(KEY_LEFT) && jogo->raqueteTopo.x > 0)
            jogo->raqueteTopo.x -= VELOCIDADE_RAQUETE;
        if (IsKeyDown(KEY_RIGHT) && jogo->raqueteTopo.x < LARGURA_TELA - jogo->raqueteTopo.width)
            jogo->raqueteTopo.x += VELOCIDADE_RAQUETE;

        if (IsKeyDown(KEY_A) && jogo->raqueteBase.x > 0)
            jogo->raqueteBase.x -= VELOCIDADE_RAQUETE;
        if (IsKeyDown(KEY_D) && jogo->raqueteBase.x < LARGURA_TELA - jogo->raqueteBase.width)
            jogo->raqueteBase.x += VELOCIDADE_RAQUETE;

        jogo->posicaoBola.x += jogo->velocidadeBola.x;
        jogo->posicaoBola.y += jogo->velocidadeBola.y;

        if (jogo->posicaoBola.x + RAIO_BOLA >= LARGURA_TELA || jogo->posicaoBola.x - RAIO_BOLA <= 0)
        {
            jogo->velocidadeBola.x *= -1;
            PlaySound(jogo->bounceSound);
        }

        if (CheckCollisionCircleRec(jogo->posicaoBola, RAIO_BOLA, jogo->raqueteTopo) || CheckCollisionCircleRec(jogo->posicaoBola, RAIO_BOLA, jogo->raqueteBase))
        {
            jogo->velocidadeBola.y *= -1;
            PlaySound(jogo->bounceSound);
        }

        if (jogo->posicaoBola.y + RAIO_BOLA <= 0)
        {
            jogo->posicaoBola = (Vector2){LARGURA_TELA / 2, ALTURA_TELA / 2};
            jogo->scoreBase++;
        }
        else if (jogo->posicaoBola.y - RAIO_BOLA >= ALTURA_TELA)
        {
            jogo->posicaoBola = (Vector2){LARGURA_TELA / 2, ALTURA_TELA / 2};
            jogo->scoreTopo++;
        }
    }
}

void DesenharJogo(const Jogo *jogo)
{
    BeginDrawing();
    ClearBackground(COR_FUNDO);

    if (!jogo->jogoIniciado)
    {
        DrawText("PONG GAME", 260, 260, 42, COR_TITULO);
        DrawText("Pressione Enter para jogar", 180, 310, 30, COR_TEXTO);
    }
    else
    {
        DrawCircleLines(LARGURA_TELA / 2, ALTURA_TELA / 2, 50, COR_CIRCULO_OCO);
        DrawCircleV(jogo->posicaoBola, RAIO_BOLA, COR_BOLA);
        DrawRectangleRec(jogo->raqueteTopo, COR_RAQUETE);
        DrawRectangleRec(jogo->raqueteBase, COR_RAQUETE);

        DrawLine(0, ALTURA_TELA / 2, LARGURA_TELA, ALTURA_TELA / 2, COR_LINHA);


        DrawText(TextFormat("Score Topo: %d", jogo->scoreTopo), 10, 10, 20, COR_TEXTO);
        DrawText(TextFormat("Score Base: %d", jogo->scoreBase), 10, ALTURA_TELA - 30, 20, COR_TEXTO);
    }

    EndDrawing();
}

void FinalizarJogo(Jogo *jogo)
{

    UnloadSound(jogo->bounceSound);
}
