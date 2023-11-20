#include "cliente_banco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void arquivo_clientes(struct Cliente *clientes, int numClientes) {
  FILE *arquivo = fopen("clientesbanco.bin", "wb");
  if (arquivo == NULL) {
    perror("Erro ao abrir o arquivo");
    return;
  }

  for (int i = 0; i < numClientes; i++) {
    fprintf(arquivo, "%s,%s,%d,%.2lf,%s\n", clientes[i].nome,
            clientes[i].cpf, clientes[i].tipo, clientes[i].saldo,
            clientes[i].senha);
  }

  fclose(arquivo);
}

void arquivo_extrato(struct Extrato *extratos, int numExtratos) {
  FILE *arquivo = fopen("extrato.txt", "w");
  if (arquivo == NULL) {
    perror("Erro ao abrir o arquivo");
    return;
  }

  for (int i = 0; i < numExtratos; i++) {
    fprintf(arquivo, "%s,%s,%.2lf,%.2lf,%.2lf\n", extratos[i].cpf,
            extratos[i].data, extratos[i].valor,
            extratos[i].tarifa, extratos[i].saldo);
  }

  fclose(arquivo);
}


void ler_arquivo_clientes(struct Cliente *clientes, int *numClientes) {
  FILE *arquivo = fopen("clientesbanco.bin", "rb");
  if (arquivo == NULL) {
    perror("Erro ao abrir o arquivo");
    return;
  }

  while (fscanf(arquivo, "%[^,],%[^,],%d,%lf,%s\n",
                clientes[*numClientes].nome, clientes[*numClientes].cpf,
                &clientes[*numClientes].tipo,
                &clientes[*numClientes].saldo,
                clientes[*numClientes].senha) != EOF) {
    (*numClientes)++;
  }

  fclose(arquivo);
}


void ler_arquivo_extrato(struct Extrato *extratos, int *numExtratos) {
  FILE *arquivo = fopen("extrato.txt", "r");
  if (arquivo == NULL) {
    perror("Erro ao abrir o arquivo");
    return;
  }

  while (fscanf(arquivo, "%[^,],%[^,],%lf,%lf,%lf\n",
                extratos[*numExtratos].cpf, extratos[*numExtratos].data,
                &extratos[*numExtratos].valor,
                &extratos[*numExtratos].tarifa,
                &extratos[*numExtratos].saldo) != EOF) {
    (*numExtratos)++;
  }

  fclose(arquivo);
}


struct Cliente *buscar_cliente(const char *cpf, struct Cliente *clientes, int numClientes) {
  for (int i = 0; i < numClientes; i++) {
    if (strcmp(cpf, clientes[i].cpf) == 0) {
      return &clientes[i];
    }
  }
  return NULL;
}


struct Cliente *buscar_senha(const char *senha, struct Cliente *clientes, int numClientes) {
  for (int i = 0; i < numClientes; i++) {
    if (strcmp(senha, clientes[i].senha) == 0) {
      return &clientes[i];
    }
  }
  return NULL;
}



void novo_cliente(struct Cliente *clientes, int *numClientes, struct Extrato *extratos, int *numExtratos) {
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

  strcpy(extratos[*numExtratos].cpf, cliente.cpf);
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  strftime(extratos[*numExtratos].data, 20, "%d/%m/%Y %H:%M:%S", tm_info);
  extratos[*numExtratos].valor = cliente.saldo;
  extratos[*numExtratos].tarifa = 0.0;
  extratos[*numExtratos].saldo = cliente.saldo;
  (*numExtratos)++;

  clientes[*numClientes] = cliente;
  (*numClientes)++;

  arquivo_clientes(clientes, *numClientes);
  arquivo_extrato(extratos, *numExtratos);
  printf("Cliente cadastrado com sucesso.\n");
}




void apagar_cliente(struct Cliente *clientes, int *numClientes) {
  char cpf[12];
  char senha[20];
  printf("Digite seu CPF: ");
  scanf("%s", cpf);
  printf("Digite sua senha: ");
  scanf("%s", senha);

  for (int i = 0; i < *numClientes; i++) {
    if (strcmp(cpf, clientes[i].cpf) == 0 &&
        strcmp(senha, clientes[i].senha) == 0) {
      for (int j = i; j < *numClientes - 1; j++) {
        clientes[j] = clientes[j + 1];
      }
      (*numClientes)--;
      arquivo_clientes(clientes, *numClientes);
      printf("\nCliente apagado\n\n");
      return;
    }
  }
  printf("CPF ou senha incorretos\n");
}

void listar_clientes(struct Cliente *clientes, int numClientes) {
  for (int i = 0; i < numClientes; i++) {
    printf("\n%s | %s | %d | %.2lf | %s\n", clientes[i].nome,
           clientes[i].cpf, clientes[i].tipo, clientes[i].saldo,
           clientes[i].senha);
  }
}


void deposito(struct Cliente *clientes, int numClientes, struct Extrato *extratos, int *numExtratos) {
  char cpf[12];
  double valor;
  printf("Digite seu CPF: ");
  scanf("%s", cpf);
  printf("Digite o valor que será depositado: ");
  scanf("%lf", &valor);

  struct Cliente *cliente = buscar_cliente(cpf, clientes, numClientes);
  if (cliente == NULL) {
    printf("Cliente não encontrado\n");
    return;
  }

  cliente->saldo += valor;

  strcpy(extratos[*numExtratos].cpf, cpf);
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  strftime(extratos[*numExtratos].data, 20, "%d/%m/%Y %H:%M:%S", tm_info);
  extratos[*numExtratos].valor = valor;
  extratos[*numExtratos].tarifa = 0.0;
  extratos[*numExtratos].saldo = cliente->saldo;
  (*numExtratos)++;

  arquivo_clientes(clientes, numClientes);
  arquivo_extrato(extratos, *numExtratos);
  printf("O valor foi depositado com sucesso\n");
}



void debito(struct Cliente *clientes, int numClientes, struct Extrato *extratos, int *numExtratos) {
  char cpf[12];
  char senha[20];
  double valor;
  printf("Digite seu CPF: ");
  scanf("%s", cpf);
  printf("Digite sua senha: ");
  scanf("%s", senha);
  printf("Digite o valor que será debitado: ");
  scanf("%lf", &valor);

  struct Cliente *cliente = buscar_cliente(cpf, clientes, numClientes);
  if (cliente == NULL) {
    printf("Cliente não encontrado\n");
    return;
  }

  if (cliente->tipo == 2 && cliente->saldo - (valor * 1.05) >= -1000) {
    cliente->saldo -= (valor * 1.05);
  } else if (cliente->tipo == 1 && cliente->saldo - (valor * 1.03) >= -5000) {
    cliente->saldo -= (valor * 1.03);
  } else {
    printf("Valor além do limite da conta\n");
    return;
  }

  strcpy(extratos[*numExtratos].cpf, cpf);
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  strftime(extratos[*numExtratos].data, 20, "%d/%m/%Y %H:%M:%S", tm_info);
  extratos[*numExtratos].valor = -valor;
  extratos[*numExtratos].tarifa = cliente->tipo == 2 ? 0.05 * valor : 0.03 * valor;
  extratos[*numExtratos].saldo = cliente->saldo;
  (*numExtratos)++;

  arquivo_clientes(clientes, numClientes);
  arquivo_extrato(extratos, *numExtratos);
  printf("Ação bem sucedida\n");
}



void extrato(struct Cliente *clientes, int numClientes, struct Extrato *extratos, int numExtratos) {
  char cpf[12];
  char senha[20];
  printf("Digite seu CPF: ");
  scanf("%s", cpf);
  printf("Digite sua senha: ");
  scanf("%s", senha);

  struct Cliente *cliente = buscar_cliente(cpf, clientes, numClientes);
  if (cliente == NULL || strcmp(senha, cliente->senha) != 0) {
    printf("CPF ou senha incorretos\n");
    return;
  }

  for (int i = 0; i < numExtratos; i++) {
    if (strcmp(cpf, extratos[i].cpf) == 0) {
      printf("Data: %s, Valor: %.2lf, Tarifa: %.2lf, Saldo: %.2lf\n",
             extratos[i].data, extratos[i].valor,
             extratos[i].tarifa, extratos[i].saldo);
    }
  }
}



void transferencia(struct Cliente *clientes, int numClientes, struct Extrato *extratos, int *numExtratos) {
  char cpf[12];
  char senha[20];
  char cpf_dest[12];
  printf("Digite seu CPF: ");
  scanf("%s", cpf);

  struct Cliente *cliente = buscar_cliente(cpf, clientes, numClientes);
  if (cliente == NULL) {
    printf("CPF não encontrado\n");
    return;
  }

  printf("Digite sua senha: ");
  scanf("%s", senha);

  printf("Digite o CPF do destinatário: ");
  scanf("%s", cpf_dest);

  struct Cliente *cliente_dest = buscar_cliente(cpf_dest, clientes, numClientes);
  if (cliente_dest == NULL) {
    printf("Destinatário não encontrado\n");
    return;
  }

  double valor;
  printf("Digite o valor que será transferido: ");
  scanf("%lf", &valor);

  if (cliente->tipo == 2 && cliente->saldo - valor > -1000) {
    cliente->saldo -= valor;
    cliente_dest->saldo += valor;
  } else if (cliente->tipo == 1 && cliente->saldo - valor > -5000) {
    cliente->saldo -= valor;
    cliente_dest->saldo += valor;
  } else {
    printf("Valor além do limite da conta\n");
    return;
  }

  strcpy(extratos[*numExtratos].cpf, cpf);
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  strftime(extratos[*numExtratos].data, 20, "%d/%m/%Y %H:%M:%S", tm_info);
  extratos[*numExtratos].valor = -valor;
  extratos[*numExtratos].tarifa = 0.0;
  extratos[*numExtratos].saldo = cliente->saldo;
  (*numExtratos)++;

  strcpy(extratos[*numExtratos].cpf, cpf_dest);
  extratos[*numExtratos].valor = valor;
  extratos[*numExtratos].tarifa = 0.0;
  extratos[*numExtratos].saldo = cliente_dest->saldo;
  (*numExtratos)++;

  arquivo_clientes(clientes, numClientes);
  arquivo_extrato(extratos, *numExtratos);
  printf("Ação bem sucedida\n");
}
