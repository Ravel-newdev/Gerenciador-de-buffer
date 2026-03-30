#include "buffer_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* ponteiro do relógio, usado pela política CLOCK */
int clockhand = 0;

/* Estado do buffer */
BufferFrame buffer[BUFFER_SIZE];
int bufferCount = 0;
int cacheMiss = 0;
int cacheHit = 0;
Policy policy = LRU;

/* Lê a linha correspondente ao page# no arquivo.
   Retorna 1 se encontrou, 0 se não encontrou. */
int lerpagina(int key, char *dest) {
    FILE *file = fopen("bancodedados(1).csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char line[MAX_CONTENT];
    int lineNum = 0;

    while (fgets(line, MAX_CONTENT, file)) {
        lineNum++;

        /* pula o cabeçalho */
        if (lineNum == 1) {
            continue;
        }

        /* remove o \n do final, se existir */
        line[strcspn(line, "\n")] = '\0';

        /* linha 2 do arquivo = página 1, linha 3 = página 2, etc */
        if (lineNum - 1 == key) {
            strcpy(dest, line);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

/* Busca uma página pelo page#
   Primeiro consulta o buffer
   Se não encontrar, procura no arquivo */
char* fetch(int key) {
    int i, j;
    char pagedata[MAX_CONTENT];
    int dirtyflag;
    BufferFrame temp;

    /* procura no buffer */
    for (i = 0; i < bufferCount; i++) {
        if (buffer[i].pageId == key) {
            cacheHit++;
            printf("[HIT] Pagina #%d encontrada no buffer!\n", key);

            /* LRU e MRU:
               move a página acessada para o final */
            if (policy == LRU || policy == MRU) {
                temp = buffer[i];
                for (j = i; j < bufferCount - 1; j++) {
                    buffer[j] = buffer[j + 1];
                }
                buffer[bufferCount - 1] = temp;
                return buffer[bufferCount - 1].data;
            }

            /* CLOCK: página acessada ganha bit de referência 1 */
            if (policy == CLOCK) {
                buffer[i].refbit = 1;
            }

            return buffer[i].data;
        }
    }

    /* se não encontrou no buffer, é miss */
    cacheMiss++;
    printf("[MISS] Pagina #%d nao esta no buffer!\n", key);

    /* se o buffer estiver cheio, remove uma página */
    if (bufferCount == BUFFER_SIZE) {
        evict();
    }

    /* tenta ler a página do arquivo */
    if (!lerpagina(key, pagedata)) {
        printf("[ERRO] Pagina #%d nao encontrada no arquivo.\n", key);
        return NULL;
    }

    /* sorteia o valor dirty: 0 ou 1 */
    dirtyflag = rand() % 2;

    /* coloca a nova página no final do buffer */
    buffer[bufferCount].pageId = key;
    strcpy(buffer[bufferCount].data, pagedata);
    buffer[bufferCount].dirty = dirtyflag;

    /* no CLOCK, a página nova entra com refbit 0.
       Só ganha 1 quando for acessada novamente. */
    buffer[bufferCount].refbit = 0;

    bufferCount++;

    printf("[INFO] Pagina #%d carregada! | dirty=%s\n",
           key, dirtyflag ? "TRUE" : "FALSE");

    return buffer[bufferCount - 1].data;
}

/* remove uma página do buffer
   Exibe a página removida e "W" se dirty == TRUE. */
void evict() {
    int victim = 0;
    int i;

    switch (policy) {
        case FIFO:
            victim = 0;
            break;

        case LRU:
            victim = 0;
            break;

        case MRU:
            victim = bufferCount - 1;
            break;

        case CLOCK:
            while (buffer[clockhand].refbit == 1) {
                buffer[clockhand].refbit = 0;
                clockhand = (clockhand + 1) % bufferCount;
            }
            victim = clockhand;
            clockhand = (clockhand + 1) % bufferCount;
            break;
    }

    printf("[EVICT] Removendo pagina #%d | %s",
           buffer[victim].pageId, buffer[victim].data);

    if (buffer[victim].dirty == 1) {
        printf(" W");
    }
    printf("\n");

    /* desloca os elementos para fechar o espaço removido */
    for (i = victim; i < bufferCount - 1; i++) {
        buffer[i] = buffer[i + 1];
    }

    bufferCount--;

    if (bufferCount > 0) {
        clockhand = clockhand % bufferCount;
    } else {
        clockhand = 0;
    }
}

/* exibindo todas as pags que estao no buffer */
void displayCache() {
    int i;

    printf("\n===== BUFFER =====\n");

    if (bufferCount == 0) {
        printf("Buffer vazio\n");
        return;
    }

    for (i = 0; i < bufferCount; i++) {
        printf("Chave -> %d | Valor -> %s | Atualizacao -> %s\n",
               buffer[i].pageId,
               buffer[i].data,
               buffer[i].dirty ? "TRUE" : "FALSE");
    }
}

/* Exibe os caches do buffer */
void displayStats() {
    printf("Cache hit: %d\n", cacheHit);
    printf("Cache miss: %d\n", cacheMiss);
}