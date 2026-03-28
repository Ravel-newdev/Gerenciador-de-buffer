#include "buffer_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void evict();
int clockhand = 0; //poteiro do relógio, usado pelo CLOCK
//Estado do buffer
BufferFrame buffer[BUFFER_SIZE]; //5 páginas em memória
int bufferCount = 0; //Conta as paginas que estão no buffer
int cacheMiss = 0; //Conta as faltas
int cacheHit = 0; //Conta os acertos
Policy policy = LRU; //politica padrão do LRU



/* Lê a linha correspondente ao page# no arquivo.
   Retorna 1 se encontrou, 0 se não encontrou. */
int lerpagina(int key, char *dest) {
    FILE *file = fopen("bancodedados(1).csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char line[MAX_CONTENT];
    int  lineNum = 0;

    while (fgets(line, MAX_CONTENT, file)) {
        lineNum++;

        /* pula o cabeçalho (linha 1) */
        if (lineNum == 1) continue;

        /* remove \n do final */
        line[strcspn(line, "\n")] = '\0';

        /* linha 2 do arquivo = page# 1, linha 3 = page# 2... */
        if (lineNum - 1 == key) {
            strcpy(dest, line);
            fclose(file);
            return 1;
        }
        
    }

    fclose(file);
    return 0;
}

/* Buscar uma página pelo page#
   Consulta o buffer, SE não encontrar, procura no arquivo */
char* fetch(int key){
    /* declarações todas no topo */
    int i, j;
    char pagedata[MAX_CONTENT];
    int dirtyflag;
    BufferFrame temp;

    /* Procurar no buffer */
    for(i = 0; i < bufferCount; i++){
        if(buffer[i].pageId == key){
            cacheHit++;
            printf("[HIT] Pagina #%d encontrada no buffer!\n", key);
            if(policy == LRU){
                temp = buffer[i];
                for(j = i; j < bufferCount - 1; j++){
                    buffer[j] = buffer[j + 1];
                }
                buffer[bufferCount - 1] = temp;
                return buffer[bufferCount - 1].data;
            }
            if(policy == CLOCK){
                buffer[i].refbit = 1;
            }
            return buffer[i].data;
        }
    }

    /* Cache miss */
    cacheMiss++;
    printf("[MISS] Pagina #%d nao esta no buffer!\n", key);

    if(bufferCount == BUFFER_SIZE){
        evict();
    }

    if(!lerpagina(key, pagedata)){
        printf("[ERRO] Pagina #%d nao encontrada no arquivo.\n", key);
        return NULL;
    }

    dirtyflag = rand() % 2;
    buffer[bufferCount].pageId = key;
    strcpy(buffer[bufferCount].data, pagedata);
    buffer[bufferCount].dirty = dirtyflag;
    bufferCount++;

    printf("[INFO] Pagina #%d carregada! | dirty=%s\n", key, dirtyflag ? "TRUE" : "FALSE");
    return buffer[bufferCount - 1].data;
}


// Remove uma página do buffer. Exibe a página removida e "W" se dirty == TRUE.
void evict() {
    int victim = 0; // índice da página a ser removida 
    int i;
    // Escolhe a vítima conforme a política 
    switch (policy) {

        case FIFO:
            // remove o mais antigo, que é sempre o índice 0
            victim = 0;
            break;

        case LRU:
            /* o menos recente fica no índice 0
               (fetch move o acessado para o final) */
            victim = 0;
            break;

        case MRU:
            // o mais recente fica no final 
            victim = bufferCount - 1;
            break;

        case CLOCK:
            // gira o ponteiro até achar refbit == 0 
            while (buffer[clockhand].refbit == 1) {
                buffer[clockhand].refbit = 0; // segunda chance 
                clockhand = (clockhand + 1) % bufferCount;
            }
            victim = clockhand;
            clockhand = (clockhand + 1) % bufferCount;
            break;
    }

    // Exibe a página que será removida 
    printf("[EVICT] Removendo pagina #%d | %s",
           buffer[victim].pageId, buffer[victim].data);
    if (buffer[victim].dirty == 1) {
        printf("W");
    }
    printf("\n");

    // Remove do buffer, desloca os elementos seguintes 
    for (i = victim; i < bufferCount - 1; i++) {
        buffer[i] = buffer[i + 1];
    }
    bufferCount--;
}