  GERENCIADOR DE BUFFER - TRABALHO 01
  Disciplina: Sistemas de Gerenciamento de Banco de Dados

DUPLA
  - Raissa Costa Sousa
  - Ravel Costa de Araújo

DESCRICAO
  Implementacao de um gerenciador de buffer simples em C com
  suporte a quatro politicas de substituicao de pagina:
  LRU, FIFO, CLOCK e MRU.

  O buffer comporta no maximo 5 paginas simultaneamente.
  Cada pagina corresponde a uma linha do arquivo bancodedados(1).csv.

ARQUIVOS
  buffer_manager.h  -> definicoes, structs e assinaturas
  buffer_manager.c  -> implementacao das funcoes
  main.c            -> menu interativo e ponto de entrada
  bancodedados(1).csv  -> arquivo de paginas (base de dados)
  Readme.txt        -> este arquivo

COMO COMPILAR
  gcc -std=c99 buffer_manager.c main.c -o buffer_manager.exe

COMO EXECUTAR
  ./buffer_manager.exe

  1. Selecione a politica de substituicao (1-4)
  2. Use o menu para buscar paginas com Fetch
  3. Visualize o buffer com DisplayCache
  4. Veja as estatisticas com DisplayStats

FUNCOES IMPLEMENTADAS
  fetch(key)      -> busca pagina no buffer; se nao encontrar,
                     carrega do arquivo e chama evict() se cheio
  evict()         -> remove pagina conforme a politica ativa;
                     exibe "W" se dirty == TRUE
  displayCache()  -> exibe todas as entradas do buffer
  displayStats()  -> exibe contadores de cache hit e miss

POLITICAS DE SUBSTITUICAO
  LRU   -> remove a pagina menos recentemente usada
  FIFO  -> remove a pagina que entrou primeiro no buffer
  CLOCK -> ponteiro circular com bit de referencia
  MRU   -> remove a pagina mais recentemente usada
