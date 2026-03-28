#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#define BUFFER_SIZE 5       /* máximo de páginas no buffer */
#define MAX_CONTENT 512     /* tamanho máximo do texto de uma página */

int lerpagina(int key, char *dest);
/* Políticas de substituição */
typedef enum {
    LRU,
    FIFO,
    CLOCK,
    MRU
} Policy;

/* Estrutura de uma página no buffer */
typedef struct {
    int  pageId;               /* page# = número da linha no arquivo */
    char data[MAX_CONTENT];    /* conteúdo da linha */
    int  dirty;                /* variável de atualização: 1=TRUE, 0=FALSE */
    int refbit;                /*bit de referência, usado pelo CLOCK */
} BufferFrame;

/* Assinaturas das funções */
char* fetch(int key);
void  evict();
void  displayCache();
void  displayStats();

#endif