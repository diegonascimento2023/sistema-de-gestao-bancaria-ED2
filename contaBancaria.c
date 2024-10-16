
#include "contaBancaria.h"

ContaBancaria *criar_conta(int numero, int agenciaNumero, const char *nomeCliente, const char *dataAbertura, float saldo, const char *status)
{
    ContaBancaria *novaConta = (ContaBancaria *)malloc(sizeof(ContaBancaria));
    if (novaConta == NULL)
    {
        printf("Erro ao alocar memoria para nova conta.\n");
        return NULL;
    }
    novaConta->numero = numero;
    novaConta->agenciaNumero = agenciaNumero;
    strcpy(novaConta->nomeCliente, nomeCliente);
    strcpy(novaConta->dataAbertura, dataAbertura);
    novaConta->saldo = saldo;
    strcpy(novaConta->status, status);
    novaConta->esquerda = NULL;
    novaConta->direita = NULL;
    return novaConta;
}


void inserir_conta(ContaBancaria **raiz, ContaBancaria *novaConta)
{
    // Verifica se novaConta é NULL
    if (novaConta == NULL)
    {
        printf("Erro: nova conta é NULL.\n");
        return; // Sai da função se novaConta for NULL
    }

    // Inserção da nova conta
    if (*raiz == NULL)
    {
        *raiz = novaConta; // Se a raiz é NULL, insere a nova conta aqui
    }
    else if (novaConta->numero < (*raiz)->numero)
    {
        inserir_conta(&(*raiz)->esquerda, novaConta); // Insere na subárvore esquerda
    }
    else if (novaConta->numero > (*raiz)->numero)
    {
        inserir_conta(&(*raiz)->direita, novaConta); // Insere na subárvore direita
    }
    else
    {
        printf("A conta com numero %d ja existe.\n", novaConta->numero); // Conta já existente
        free(novaConta);                                                 // Libera a memória da nova conta
    }
}


// Função para salvar as contas no arquivo contas.txt
void salvar_contas(ContaBancaria *raiz, FILE *file)
{
    if (raiz != NULL)
    {
        salvar_contas(raiz->esquerda, file); // Salvar contas à esquerda

        // Salvar a conta atual
        fprintf(file, "Agencia %d\tConta %d\tCliente:%s\tData:%s\tSaldo:%.2f\tStatus:%s\n",
                raiz->agenciaNumero, raiz->numero, raiz->nomeCliente, raiz->dataAbertura,
                raiz->saldo, raiz->status);

        salvar_contas(raiz->direita, file); // Salvar contas à direita
    }
}

// Função para liberar as contas  
void liberar_contas(ContaBancaria *raiz)
{
    if (raiz != NULL)
    {
        liberar_contas(raiz->esquerda);
        liberar_contas(raiz->direita);
        free(raiz);
    }
}

char le_opcao(int menorvalor, int maiorvalor)
{
   int op;
   char entrada[51];
   while (1)
   {
      printf("Opcao: ");
      scanf(" %[^\n]", entrada);
      op = entrada[0];
      if (op >= menorvalor && op <= maiorvalor && strlen(entrada) == 1)
      {
         limpa_buffer();
         break;
      }
      else
      {
         printf("Opção inválida!\nDigite uma opção entre %c e %c.\n", menorvalor, maiorvalor);
         ;
         limpa_buffer();
      }
   }
   return op;
}


void limpa_buffer(void)
{
   int valorlido;
   do
   {
      valorlido = getchar();
   } while ((valorlido != '\n') && (valorlido != EOF));
}


// Função para verificar se a entrada contém apenas dígitos (números)
bool entradaContemApenasDigitos(const char *entrada)
{
   for (int i = 0; entrada[i] != '\0'; i++)
   {
      if (!isdigit(entrada[i]))
      {
         return false;
      }
   }
   return true;
}


void percorrer_arvore(ContaBancaria *raiz, Heap *heap) {
    if (raiz != NULL) {
        inserir_heap(heap, raiz); // Insere a conta na heap
        percorrer_arvore(raiz->esquerda, heap); // Percorre a subárvore esquerda
        percorrer_arvore(raiz->direita, heap);  // Percorre a subárvore direita
    }
}

// Função para inicializar a heap
void inicializar_heap(Heap *heap) {
    heap->tamanho = 0;
}

// Função para inserir uma conta na heap
void inserir_heap(Heap *heap, ContaBancaria *conta) {
    if (heap->tamanho < MAX_HEAP_SIZE) {
        heap->contas[heap->tamanho] = conta;
        int i = heap->tamanho;
        heap->tamanho++;

        // Ajusta a heap para manter a propriedade de max-heap
        while (i > 0 && heap->contas[i]->saldo > heap->contas[(i - 1) / 2]->saldo) {
            // Troca com o pai
            ContaBancaria *temp = heap->contas[i];
            heap->contas[i] = heap->contas[(i - 1) / 2];
            heap->contas[(i - 1) / 2] = temp;
            i = (i - 1) / 2;
        }
    }
}

// Função para obter a conta com o maior saldo (a raiz da heap)
ContaBancaria *obter_maior_saldo(Heap *heap) {
    if (heap->tamanho > 0) {
        return heap->contas[0];
    }
    return NULL; // Heap vazia
}

