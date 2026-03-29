#include "buffer_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern Policy policy;

int main() {
    int opcao;
    int key;
    srand(time(NULL)); /* sem isso o padrao pode se repetir */

    printf("Alunos: Raissa Costa e Ravel Costa \n");
    printf("=== Gerenciador de Buffer ===\n");
    printf("Selecione a politica de substituicao: \n");
    printf("1. LRU\n");
    printf("2. FIFO \n");
    printf("3. CLOCK\n");
    printf("4. MRU\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch(opcao) {
        case 1: policy = LRU;   break;
        case 2: policy = FIFO;  break;
        case 3: policy = CLOCK; break;
        case 4: policy = MRU;   break;
        default:
            printf("Opcao invalida! Usando LRU.\n");
            policy = LRU;
    }

    do {
        printf("\n--- Menu ---\n");
        printf("1. Fetch (buscar pagina)\n");
        printf("2. DisplayCache\n");
        printf("3. DisplayStats\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: {
                char *resultado;
                printf("Digite o page#: ");
                scanf("%d", &key);
                resultado = fetch(key); /* no pdf pede que fetch retorne o conteúdo */
                if (resultado != NULL) {
                    printf("Conteudo da pagina: %s\n", resultado);
                }
                break;
            }
            case 2:
                displayCache();
                break;
            case 3:
                displayStats();
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 0);

    return 0;
}