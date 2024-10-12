#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#include <stdbool.h>
#include "contaBancaria.h"

int main()
{
    Agencia *raiz_agencias = NULL;
    FILE *file;

    // Abrir o arquivo de agencias
    file = fopen("agencias.txt", "r");
    if (file)
    {
        raiz_agencias = ler_agencias();
        fclose(file);
    }

    // Abrir o arquivo de contas e ler todas as contas
    file = fopen("contas.txt", "r");
    if (file)
    {
        ler_contas(raiz_agencias, file); // Lê todas as contas e associa às agências
        fclose(file);
    }

    char opcao;     // Armazena a opção verificada
    char input[50]; // Armazena número como string

    // Variaveis para criação da agencia
    int codigo;
    char nome[51], localizacao[101], horario[20];

    // Variaveis para criação da agencia
    int numero, agenciaNumero;
    char nomeCliente[51], dataAbertura[11], status[10];
    float saldo;

    do
    {
        printf("\n\t MENU DE OPCOES \n");
        printf("\n1. ADICIONAR NOVA AGENCIA.");
        printf("\n2. ADICIONAR NOVA CONTA BANCARIA.");
        printf("\n3. LISTAR TODAS AS AGENCIAS.");
        printf("\n4. LISTAR TODAS AS CONTAS DE UMA AGENCIA PELO CODIGO.");
        printf("\n5. BUSCAR CONTA COM MAIOR SALDO.");
        printf("\n6. SAIR\n");

        opcao = le_opcao('1', '6');

        switch (opcao)
        {
        case '1':
        {
            bool numValido = false;     // Variável para verificar se o numero do codigo da agencia fornecido é válido
            int nomeValido = 0;         // Variável para verificar se o nome da agência fornecido é válido
            int localizacaoValida = 0;  // Variável para verificar se a localização da agência fornecida é válida
            bool horarioValido = false; // Variável para verificar se o horario de funcionamento fornecida é válido

            // Verifica se o numero do codigo da agencia fornecido é válido
            do
            {
                printf("Informe o codigo da agencia: ");
                if (scanf(" %49s", input) != 1)
                {
                    printf("Entrada invalida. Tente novamente.\n");
                    while (getchar() != '\n')
                    {
                        // Limpa o buffer de entrada para evitar loop infinito
                    }
                }
                else if (!entradaContemApenasDigitos(input))
                {
                    printf("Entrada invalida. Apenas numeros sao permitidos. Tente novamente.\n");
                }
                else
                {
                    sscanf(input, "%d", &codigo);
                    numValido = true;
                }
            } while (!numValido);

            // Verifica se o nome da agência fornecido é válido
            do
            {
                printf("Informe o nome da agencia: ");
                scanf(" %[^\n]", nome);
                getchar(); // Limpar o buffer

                nomeValido = 1;
                for (int i = 0; nome[i] != '\0'; i++)
                {
                    if (!isalpha(nome[i]) && !isspace(nome[i]))
                    {
                        nomeValido = 0;
                        break;
                    }
                }

                if (!nomeValido)
                {
                    printf("O nome digitado contem caracteres invalidos.\n");
                }
            } while (!nomeValido);

            // Verifica se a localização da agência fornecida é válida
            do
            {
                printf("Informe a localizacao da agencia: ");
                scanf(" %[^\n]", localizacao);
                getchar(); // Limpar o buffer

                localizacaoValida = 1;
                for (int i = 0; localizacao[i] != '\0'; i++)
                {
                    if (!isalpha(localizacao[i]) && !isspace(localizacao[i]))
                    {
                        localizacaoValida = 0;
                        break;
                    }
                }

                if (!localizacaoValida)
                {
                    printf("A localizacao digitada contem caracteres invalidos.\n");
                }
            } while (!localizacaoValida);

            // Verifica se o horário de funcionamento da agência fornecido é válido
            while (!horarioValido)
            {
                printf("Informe o horario de funcionamento da agencia (entrada saida, no formato HH:MM HH:MM): ");
                scanf(" %12[^\n]", horario); // Limitar a leitura a 12 caracteres, deixando espaço para o caractere nulo

                int entradaHoras, entradaMinutos, saidaHoras, saidaMinutos;

                // Verificar se a entrada tem o formato correto HH:MM HH:MM
                if (sscanf(horario, "%d:%d %d:%d", &entradaHoras, &entradaMinutos, &saidaHoras, &saidaMinutos) == 4)
                {
                    // Verificar se as horas e minutos estão em faixas válidas
                    if (entradaHoras >= 0 && entradaHoras <= 23 && entradaMinutos >= 0 && entradaMinutos <= 59 &&
                        saidaHoras >= 0 && saidaHoras <= 23 && saidaMinutos >= 0 && saidaMinutos <= 59)
                    {
                        horarioValido = true;
                    }
                    else
                    {
                        printf("Horario invalido. Certifique-se de que as horas estejam entre 0 e 23 e os minutos entre 0 e 59.\n");
                    }
                }
                else
                {
                    printf("Formato invalido. Use o formato HH:MM HH:MM.\n");
                }

                // Limpar o buffer de entrada
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            }

            Agencia *novaAgencia = criar_agencia(codigo, nome, localizacao, horario);
            inserir_agencia(&raiz_agencias, novaAgencia);

            // Salvar as agências no arquivo
            file = fopen("agencias.txt", "w");
            if (file)
            {
                salvar_agencias(raiz_agencias, file);
                fclose(file);
            }
            break;
        }

        case '2':
        {
            bool numValido = false; // Variável para verificar se o numero do codigo da agencia fornecido é válido
            int numContaValido = 0; // Variável para verificar se o numero da conta fornecido é válidov
            // Verifica se o numero da conta fornecido é válido
            while (1)
            {
                printf("Informe o numero da conta: ");
                if (fgets(input, sizeof(input), stdin))
                {
                    int valido = 1;

                    for (int i = 0; input[i] != '\0'; i++)
                    {
                        if (!isdigit(input[i]) && input[i] != '\n')
                        {
                            valido = 0;
                            break;
                        }
                    }

                    if (valido && sscanf(input, "%d", &numero) == 1)
                    {
                        numContaValido = 1;
                        break; // Saia do loop se o número for válido
                    }
                    else
                    {
                        printf("Entrada invalida. Digite novamente (somente numeros):\n");
                    }
                }
            }

            // Verifica se o numero do codigo da agencia fornecido é válido
            do
            {
                printf("Informe o codigo da agencia: ");
                if (scanf(" %49s", input) != 1)
                {
                    printf("Entrada invalida. Tente novamente.\n");
                    while (getchar() != '\n')
                    {
                        // Limpa o buffer de entrada para evitar loop infinito
                    }
                }
                else if (!entradaContemApenasDigitos(input))
                {
                    printf("Entrada invalida. Apenas numeros sao permitidos. Tente novamente.\n");
                }
                else
                {
                    sscanf(input, "%d", &agenciaNumero);
                    numValido = true;
                }
            } while (!numValido);

            // Verificar se a agência existe
            Agencia *agencia = buscar_agencia(raiz_agencias, agenciaNumero);
            if (agencia == NULL)
            {
                bool numValido = false;     // Variável para verificar se o numero do codigo da agencia fornecido é válido
                int nomeValido = 0;         // Variável para verificar se o nome da agência fornecido é válido
                int localizacaoValida = 0;  // Variável para verificar se a localização da agência fornecida é válida
                bool horarioValido = false; // Variável para verificar se o horario de funcionamento fornecida é válido
                char escolha;
                do
                {
                    printf("Agencia nao encontrada.\n");
                    printf("1. Criar nova agencia\n2. Ver agencias disponiveis\n");
                    escolha = le_opcao('1', '2');

                    switch (escolha)
                    {
                    case '1':
                    {
                        // Lógica para adicionar nova agência (similar ao case 1)
                        // Verifica se o numero do codigo da agencia fornecido é válido
                        do
                        {
                            printf("Informe o codigo da agencia: ");
                            if (scanf(" %49s", input) != 1)
                            {
                                printf("Entrada invalida. Tente novamente.\n");
                                while (getchar() != '\n')
                                {
                                    // Limpa o buffer de entrada para evitar loop infinito
                                }
                            }
                            else if (!entradaContemApenasDigitos(input))
                            {
                                printf("Entrada invalida. Apenas numeros sao permitidos. Tente novamente.\n");
                            }
                            else
                            {
                                sscanf(input, "%d", &codigo);
                                numValido = true;
                            }
                        } while (!numValido);

                        // Verifica se o nome da agência fornecido é válido
                        do
                        {
                            printf("Informe o nome da agência: ");
                            scanf(" %[^\n]", nome);
                            getchar(); // Limpar o buffer

                            nomeValido = 1;
                            for (int i = 0; nome[i] != '\0'; i++)
                            {
                                if (!isalpha(nome[i]) && !isspace(nome[i]))
                                {
                                    nomeValido = 0;
                                    break;
                                }
                            }

                            if (!nomeValido)
                            {
                                printf("O nome digitado contém caracteres inválidos.\n");
                            }
                        } while (!nomeValido);

                        // Verifica se a localização da agência fornecida é válida
                        do
                        {
                            printf("Informe a localizacao da agencia: ");
                            scanf(" %[^\n]", localizacao);
                            getchar(); // Limpar o buffer

                            localizacaoValida = 1;
                            for (int i = 0; localizacao[i] != '\0'; i++)
                            {
                                if (!isalpha(localizacao[i]) && !isspace(localizacao[i]))
                                {
                                    localizacaoValida = 0;
                                    break;
                                }
                            }

                            if (!localizacaoValida)
                            {
                                printf("A localizacao digitada contem caracteres invalidos.\n");
                            }
                        } while (!localizacaoValida);

                        // Verifica se o horário de funcionamento da agência fornecido é válido
                        while (!horarioValido)
                        {
                            printf("Informe o horario de funcionamento da agencia (entrada saida, no formato HH:MM HH:MM): ");
                            scanf(" %12[^\n]", horario); // Limitar a leitura a 12 caracteres, deixando espaço para o caractere nulo

                            int entradaHoras, entradaMinutos, saidaHoras, saidaMinutos;

                            // Verificar se a entrada tem o formato correto HH:MM HH:MM
                            if (sscanf(horario, "%d:%d %d:%d", &entradaHoras, &entradaMinutos, &saidaHoras, &saidaMinutos) == 4)
                            {
                                // Verificar se as horas e minutos estão em faixas válidas
                                if (entradaHoras >= 0 && entradaHoras <= 23 && entradaMinutos >= 0 && entradaMinutos <= 59 &&
                                    saidaHoras >= 0 && saidaHoras <= 23 && saidaMinutos >= 0 && saidaMinutos <= 59)
                                {
                                    horarioValido = true;
                                }
                                else
                                {
                                    printf("Horario invalido. Certifique-se de que as horas estejam entre 0 e 23 e os minutos entre 0 e 59.\n");
                                }
                            }
                            else
                            {
                                printf("Formato invalido. Use o formato HH:MM HH:MM.\n");
                            }

                            // Limpar o buffer de entrada
                            int c;
                            while ((c = getchar()) != '\n' && c != EOF)
                                ;
                        }
                        Agencia *novaAgencia = criar_agencia(codigo, nome, localizacao, horario);
                        inserir_agencia(&raiz_agencias, novaAgencia);

                        // Salvar as agências no arquivo
                        file = fopen("agencias.txt", "w");
                        if (file)
                        {
                            salvar_agencias(raiz_agencias, file);
                            fclose(file);
                        }
                        break;
                    }
                    case '2':
                    {
                        listar_agencias(raiz_agencias);
                        break;
                    }
                    }
                } while (escolha != '2');
            }
            if (agencia)
            {
                int nomeValido = 0;         // Variável para verificar se o nome da agência fornecido é válido
                bool entradaValida = false; // Variável para verificar se a data de abertura fornecida é válida
                int statusValido = 0;       // Variável para verificar se o status da conta fornecido é válido
                int saldoValido = 0;        // Variável para verificar se o saldo da conta fornecido é válido
                // Verifica se o nome do cliente fornecido é válido
                do
                {
                    printf("Informe o nome do cliente: ");
                    scanf(" %[^\n]", nomeCliente);
                    getchar(); // Limpar o buffer

                    nomeValido = 1;
                    for (int i = 0; nomeCliente[i] != '\0'; i++)
                    {
                        if (!isalpha(nomeCliente[i]) && !isspace(nomeCliente[i]))
                        {
                            nomeValido = 0;
                            break;
                        }
                    }

                    if (!nomeValido)
                    {
                        printf("O nome digitado contém caracteres inválidos.\n");
                    }
                } while (!nomeValido);

                // Verifica se a data de abertura fornecida é válida
                do
                {
                    printf("Informe a data de abertura da conta (dd/mm/aaaa): ");
                    scanf(" %11[^\n]", dataAbertura);

                    // Verifica se a entrada tem 10 caracteres, incluindo duas barras
                    if (strlen(dataAbertura) == 10)
                    {
                        // Verifica se os caracteres nas posições 2 e 5 são barras
                        if (dataAbertura[2] == '/' && dataAbertura[5] == '/')
                        {
                            // Verifica se os caracteres restantes são dígitos numéricos
                            bool isNumeric = true;
                            for (int i = 0; i < 10; i++)
                            {
                                if (i != 2 && i != 5 && !isdigit(dataAbertura[i]))
                                {
                                    isNumeric = false;
                                    break;
                                }
                            }
                            if (isNumeric)
                            {
                                entradaValida = true;
                            }
                        }
                    }

                    if (!entradaValida)
                    {
                        printf("Formato de data inválido. Por favor, tente novamente.\n");
                    }
                } while (!entradaValida);

                while (1)
                {
                    printf("Informe o saldo da conta: ");
                    char input[20];
                    if (fgets(input, sizeof(input), stdin))
                    {
                        int valido = 1;
                        char *endptr;

                        // Converte a entrada para um número de ponto flutuante
                        saldo = strtof(input, &endptr);

                        if (endptr == input)
                        {
                            valido = 0;
                        }
                        else
                        {
                            // Verifica se há caracteres não numéricos após o número
                            for (int i = 0; input[i] != '\0'; i++)
                            {
                                if (!isdigit(input[i]) && input[i] != '.' && input[i] != '\n')
                                {
                                    valido = 0;
                                    break;
                                }
                            }
                        }

                        if (valido)
                        {
                            saldoValido = 1;
                            break; // Sai do loop se o saldo for válido
                        }
                        else
                        {
                            printf("Entrada inválida. Digite novamente (somente números):\n");
                        }
                    }
                }

                // Verifica se o status da conta fornecido é válido
                do
                {
                    printf("Informe o status da conta (ativa, desativada, bloqueada): ");
                    scanf(" %10[^\n]", status);
                    getchar(); // Limpar o buffer

                    statusValido = 1;
                    for (int i = 0; status[i] != '\0'; i++)
                    {
                        if (!isalpha(status[i]) && !isspace(status[i]))
                        {
                            statusValido = 0;
                            break;
                        }
                    }

                    if (!statusValido)
                    {
                        printf("O status digitado contém caracteres inválidos.\n");
                    }
                } while (!statusValido);

                // Criar a nova conta
                ContaBancaria *novaConta = criar_conta(numero, agenciaNumero, nomeCliente, dataAbertura, saldo, status);
                inserir_conta(&(agencia->contas), novaConta);
                // Quando for salvar as contas, gravar todas de uma vez no arquivo
                file = fopen("contas.txt", "w"); // Usar "w" para sobrescrever o arquivo
                if (file)
                {
                    Agencia *agenciaAtual = raiz_agencias;
                    while (agenciaAtual != NULL)
                    {
                        salvar_contas(agenciaAtual->contas, file);
                        agenciaAtual = agenciaAtual->direita; // Navegar pela árvore de agências
                    }
                    fclose(file);
                }
            }
            break;
        }
        case '3':
            listar_agencias(raiz_agencias); // Exibir todas as agências
            break;
        case '4':
        {
            bool numValido = false; // Variável para verificar se o numero do codigo da agencia fornecido é válido
            int codigo_agencia;
            // Verifica se o numero do codigo da agencia fornecido é válido
            do
            {
                printf("Informe o codigo da agencia: ");
                if (scanf(" %49s", input) != 1)
                {
                    printf("Entrada invalida. Tente novamente.\n");
                    while (getchar() != '\n')
                    {
                        // Limpa o buffer de entrada para evitar loop infinito
                    }
                }
                else if (!entradaContemApenasDigitos(input))
                {
                    printf("Entrada invalida. Apenas numeros sao permitidos. Tente novamente.\n");
                }
                else
                {
                    sscanf(input, "%d", &codigo_agencia);
                    numValido = true;
                }
            } while (!numValido);

            // Buscar a agência
            Agencia *agencia = buscar_agencia(raiz_agencias, codigo_agencia);
            if (agencia != NULL)
            {
                // Se a agência foi encontrada, listar as contas
                listar_contas_agencia(agencia->contas, codigo_agencia); // Passar as contas da agência
            }
            else
            {
                printf("Agencia nao encontrada.\n");
            }
            break;
        }
        case '5':
        {
            bool numValido = false; // Variável para verificar se o número do código da agência fornecido é válido
            do
            {
                printf("Informe o codigo da agencia: ");
                if (scanf(" %49s", input) != 1)
                {
                    printf("Entrada invalida. Tente novamente.\n");
                    while (getchar() != '\n')
                    {
                        // Limpa o buffer de entrada para evitar loop infinito
                    }
                }
                else if (!entradaContemApenasDigitos(input))
                {
                    printf("Entrada invalida. Apenas numeros sao permitidos. Tente novamente.\n");
                }
                else
                {
                    sscanf(input, "%d", &agenciaNumero);
                    numValido = true;
                }
            } while (!numValido);

            Agencia *agencia = buscar_agencia(raiz_agencias, agenciaNumero);
            if (agencia == NULL)
            {
                printf("Agencia nao existe. Tente novamente.\n");
            }
            else
            {
                buscar_maior_saldo(agencia);
            }
            break; 
        }

        case '6':

            printf("Programa finalizado com sucesso! Obrigado por usar nosso programa.\n");
            break;
        }
    } while (opcao != '6');

    // Liberar a memória alocada para as agências e suas contas
    liberar_agencias(raiz_agencias);

    return 0;
}
