#include "cliente_banco.h"
#include <stdio.h>

struct Cliente clientesbanco[100];
struct Extrato lista_extrato[100];
int numClientes = 0;
int numExtratos = 0;

int main() {
  ler_arquivo_clientes(clientesbanco, &numClientes);
  ler_arquivo_extrato(lista_extrato, &numExtratos);

  int opcao;
  while (1) {
    printf("\n 1 Novo cliente\n 2 Apagar cliente\n 3.Listar clientes\n 4 "
           "Débito\n "
           "5 Depósito\n 6 Extrato\n 7 Transferência entre contas\n 8 Sair\n");
    printf("\nEscolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      novo_cliente(clientesbanco, &numClientes, lista_extrato, &numExtratos);
      break;
    case 2:
      apagar_cliente(clientesbanco, &numClientes);
      break;
    case 3:
      listar_clientes(clientesbanco, numClientes);
      break;
    case 4:
      debito(clientesbanco, numClientes, lista_extrato, &numExtratos);
      break;
    case 5:
      deposito(clientesbanco, numClientes, lista_extrato, &numExtratos);
      break;
    case 6:
      extrato(clientesbanco, numClientes, lista_extrato, numExtratos);
      break;
    case 7:
      transferencia(clientesbanco, numClientes, lista_extrato, &numExtratos);
      break;
    case 8:
      arquivo_clientes(clientesbanco, numClientes);
      arquivo_extrato(lista_extrato, numExtratos);
      printf("\nAção encerrada\n\n");
      return 0;
    default:
      printf("\nOpção inválida\n\n");
    }
  }

  return 0;
}
