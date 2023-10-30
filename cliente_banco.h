#ifndef CLIENTE_BANCO_H
#define CLIENTE_BANCO_H

#include <stdio.h>
#include <time.h>

struct Cliente {
  char nome[50];
  char cpf[12];
  int tipo;
  double saldo;
  char senha[20];
};

struct Extrato {
  char cpf[12];
  char data[20];
  double valor;
  double tarifa;
  double saldo;
};

void arquivo_clientes();
void arquivo_extrato();
void ler_arquivo_clientes();
void ler_arquivo_extrato();
struct Cliente *buscar_cliente(const char *cpf);
void novo_cliente();
void apagar_cliente();
void listar_clientes();
void debito();
void deposito();
void extrato();
void transferencia();

#endif // CLIENTE_BANCO_H
