#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "agencia.h"

Agencia *criar_agencia(int codigo, const char *nome, const char *localizacao, const char *horario)
{
  Agencia *novaAgencia = (Agencia *)malloc(sizeof(Agencia));
  if (novaAgencia == NULL)
  {
    printf("Erro ao alocar memoria para nova agencia.\n");
    return NULL;
  }
  novaAgencia->codigo = codigo;
  strcpy(novaAgencia->nome, nome);
  strcpy(novaAgencia->localizacao, localizacao);
  strcpy(novaAgencia->horario, horario);
  novaAgencia->contas = NULL;
  novaAgencia->esquerda = NULL;
  novaAgencia->direita = NULL;
  return novaAgencia;
}

void inserir_agencia(Agencia **raiz, Agencia *novaAgencia)
{
  if (*raiz == NULL)
  {
    *raiz = novaAgencia;
  }
  else if (novaAgencia->codigo < (*raiz)->codigo)
  {
    inserir_agencia(&(*raiz)->esquerda, novaAgencia);
  }
  else if (novaAgencia->codigo > (*raiz)->codigo)
  {
    inserir_agencia(&(*raiz)->direita, novaAgencia);
  }
  else
  {
    printf("Agencia com código %d ja existe.\n", novaAgencia->codigo);
    free(novaAgencia);
  }
}

void listar_agencias(Agencia *raiz)
{
  if (raiz != NULL)
  {
    listar_agencias(raiz->esquerda);
    printf("Agencia: %d, Nome: %s, Localizacao: %s, Horario: %s\n",
           raiz->codigo, raiz->nome, raiz->localizacao, raiz->horario);
    listar_agencias(raiz->direita);
  }
}

// Função para salvar as agencias no arquivo agencias.txt
void salvar_agencias(Agencia *raiz, FILE *file)
{
  if (raiz != NULL)
  {
    salvar_agencias(raiz->esquerda, file); // Salvar agências à esquerda
    fprintf(file, "Agencia %d\tNome:%s\tLocalizacao:%s\tHorario:%s\n",
            raiz->codigo, raiz->nome, raiz->localizacao, raiz->horario);
    salvar_agencias(raiz->direita, file); // Salvar agências à direita
  }
}

Agencia *ler_agencias()
{
  // Abre o arquivo agencias.txt para leitura
  FILE *arquivo = fopen("agencias.txt", "r");
  if (arquivo == NULL)
  {
    printf("Erro ao abrir o arquivo agencias.txt.\n");
    exit(1); // Sai do programa se ocorrer erro ao abrir o arquivo
  }

  Agencia *raiz_agencias = NULL;
  char linha[256];

  // Lê cada linha do arquivo e extrai os dados da agência
  while (fgets(linha, sizeof(linha), arquivo) != NULL)
  {
    int codigo;
    char nome[51], localizacao[101], horario[20];

    // Analisa a linha e extrai os dados no formato esperado
    sscanf(linha, "Agencia %d\tNome:%50[^\t]\tLocalizacao:%100[^\t]\tHorario:%19[^\n]",
           &codigo, nome, localizacao, horario);

    // Cria uma nova agência com os dados extraídos
    Agencia *novaAgencia = criar_agencia(codigo, nome, localizacao, horario);

    // Insere a nova agência na árvore de agências
    inserir_agencia(&raiz_agencias, novaAgencia);
  }

  // Fecha o arquivo após a leitura
  fclose(arquivo);

  return raiz_agencias;
}

void liberar_agencias(Agencia *raiz)
{
  if (raiz != NULL)
  {
    liberar_agencias(raiz->esquerda);
    liberar_agencias(raiz->direita);
    liberar_contas(raiz->contas);
    free(raiz);
  }
}

void salvar_arquivo(Agencia *raiz, const char *nome_arquivo)
{
  FILE *file = fopen(nome_arquivo, "w");
  if (file)
  {
    salvar_agencias(raiz, file);
    fclose(file);
  }
  else
  {
    printf("Erro ao abrir o arquivo para salvar.\n");
  }
}

Agencia *buscar_agencia(Agencia *raiz, int codigo)
{
  if (raiz == NULL || raiz->codigo == codigo)
    return raiz;
  if (codigo < raiz->codigo)
    return buscar_agencia(raiz->esquerda, codigo);
  else
    return buscar_agencia(raiz->direita, codigo);
}

void listar_contas_agencia(ContaBancaria *raiz, int agenciaNumero)
{
  if (raiz != NULL)
  {
    listar_contas_agencia(raiz->esquerda, agenciaNumero);
    if (raiz->agenciaNumero == agenciaNumero)
    {
      printf("Numero da Conta: %d\tCliente: %s\tData: %s\tSaldo: %.2f\tStatus: %s\n",
             raiz->numero, raiz->nomeCliente, raiz->dataAbertura, raiz->saldo, raiz->status);
    }
    listar_contas_agencia(raiz->direita, agenciaNumero);
  }
}

int verificar_somente_letras(const char *str)
{
  while (*str)
  {
    if (!isalpha(*str) && !isspace(*str))
    {
      return 0; // Retorna 0 se não for letra ou espaço
    }
    str++;
  }
  return 1; // Retorna 1 se todas forem letras ou espaços
}

// Função para ler todas as contas do arquivo contas.txt
void ler_contas(Agencia *raiz_agencias, FILE *file)
{
  int agenciaNumero, numero;
  char nomeCliente[51], dataAbertura[11], status[10];
  float saldo;

  while (fscanf(file, "Agencia %d\tConta %d\tCliente:%50[^\t]\tData:%10[^\t]\tSaldo:%f\tStatus:%9[^\n]\n",
                &agenciaNumero, &numero, nomeCliente, dataAbertura, &saldo, status) == 6)
  {
    // Criar a nova conta lida do arquivo
    ContaBancaria *novaConta = criar_conta(numero, agenciaNumero, nomeCliente, dataAbertura, saldo, status);

    // Encontrar a agência correspondente
    Agencia *agencia = buscar_agencia(raiz_agencias, agenciaNumero);
    if (agencia != NULL)
    {
      // Inserir a conta na árvore de contas da agência encontrada
      inserir_conta(&(agencia->contas), novaConta);
    }
    else
    {
      printf("Agencia %d nao encontrada para a conta %d.\n", agenciaNumero, numero);
    }
  }
}