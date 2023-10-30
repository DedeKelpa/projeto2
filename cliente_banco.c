#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "clientes.h"

struct Cliente clientesbanco[100];
int numClientes = 0;

void arquivo_clientes() {
  FILE *arquivo = fopen("clientesbanco.txt", "w");
  if (arquivo == NULL) {
      perror("Erro ao abrir o arquivo");
      return;
  }

  for (int i = 0; i < numClientes; i++) {
      fprintf(arquivo, "%s,%s,%d,%.2lf,%s\n", clientesbanco[i].nome, clientesbanco[i].cpf, clientesbanco[i].tipo, clientesbanco[i].saldo, clientesbanco[i].senha);
  }

  fclose(arquivo);
}

void ler_arquivo_clientes() {
   FILE *arquivo = fopen("clientesbanco.txt", "r");
       if (arquivo == NULL) {
           perror("Erro ao abrir o arquivo");
           return;
       }

       while (fscanf(arquivo, "%[^,],%[^,],%d,%lf,%s\n", clientesbanco[numClientes].nome, clientesbanco[numClientes].cpf, &clientesbanco[numClientes].tipo, &clientesbanco[numClientes].saldo, clientesbanco[numClientes].senha) != EOF) {
           numClientes++;
       }

       fclose(arquivo);
   
}

struct Cliente *buscar_cliente(const char *cpf) {
   for (int i = 0; i < numClientes; i++) {
     if (strcmp(cpf, clientesbanco[i].cpf) == 0) {
         return &clientesbanco[i];
     }
   }
   return NULL;
}

void novo_cliente() {
   struct Cliente cliente;
   printf("Digite os dados para criar sua conta:\n");
   printf("Digite seu nome: ");
   scanf("%s", cliente.nome);
   printf("Digite seu CPF: ");
   scanf("%s", cliente.cpf);
   printf("Digite 1 para a conta plus ou digite 2 para a conta comum: ");
   scanf("%d", &cliente.tipo);
   printf("Digite o valor da conta: ");
   scanf("%lf", &cliente.saldo);
   printf("Digite a senha do usuário: ");
   scanf("%s", cliente.senha);

   strcpy(lista_extrato[numExtratos].cpf, cliente.cpf);
   time_t now = time(NULL);
   struct tm *tm_info = localtime(&now);
   strftime(lista_extrato[numExtratos].data, 20, "%d/%m/%Y %H:%M:%S", tm_info);
   lista_extrato[numExtratos].valor = cliente.saldo;
   lista_extrato[numExtratos].tarifa = 0.0;
   lista_extrato[numExtratos].saldo = cliente.saldo;
   numExtratos++;

   clientesbanco[numClientes] = cliente;
   numClientes++;

   arquivo_clientes();
   arquivo_extrato();
   printf("Cliente cadastrado com sucesso.\n");
}

void apagar_cliente() {
  char cpf[12];
  char senha[20];
  printf("Digite seu CPF: ");
  scanf("%s", cpf);
  printf("Digite sua senha: ");
  scanf("%s", senha);

  for (int i = 0; i < numClientes; i++) {
      if (strcmp(cpf, clientesbanco[i].cpf) == 0 && strcmp(senha, clientesbanco[i].senha) == 0) {
          for (int j = i; j < numClientes - 1; j++) {
              clientesbanco[j] = clientesbanco[j + 1];
          }
          numClientes--;
          arquivo_clientes();
          printf("Cliente apagado\n");
          return;
      }
  }
  printf("CPF ou senha incorretos\n");
}

void listar_clientes() {
  for (int i = 0; i < numClientes; i++) {
      printf("%s | %s | %d | %.2lf | %s\n", clientesbanco[i].nome, clientesbanco[i].cpf, clientesbanco[i].tipo, clientesbanco[i].saldo, clientesbanco[i].senha);
  } 
}
