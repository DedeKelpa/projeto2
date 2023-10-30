#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

struct Cliente clientesbanco[100];
struct Extrato lista_extrato[100];
int numClientes = 0;
int numExtratos = 0;

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

void arquivo_extrato() {
    FILE *arquivo = fopen("extrato.txt", "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    for (int i = 0; i < numExtratos; i++) {
        fprintf(arquivo, "%s,%s,%.2lf,%.2lf,%.2lf\n", lista_extrato[i].cpf, lista_extrato[i].data, lista_extrato[i].valor, lista_extrato[i].tarifa, lista_extrato[i].saldo);
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

void ler_arquivo_extrato() {
    FILE *arquivo = fopen("extrato.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    while (fscanf(arquivo, "%[^,],%[^,],%lf,%lf,%lf\n", lista_extrato[numExtratos].cpf, lista_extrato[numExtratos].data, &lista_extrato[numExtratos].valor, &lista_extrato[numExtratos].tarifa, &lista_extrato[numExtratos].saldo) != EOF) {
        numExtratos++;
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
    } //fazer um if para se n tiver nenhum cliente na lista
}

void debito() {
    char cpf[12];
    char senha[20];
    double valor;
    printf("Digite seu CPF: ");
    scanf("%s", cpf);
    printf("Digite sua senha: ");
    scanf("%s", senha);
    printf("Digite o valor que será debitado: ");
    scanf("%lf", &valor);

    struct Cliente *cliente = buscar_cliente(cpf);
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

    strcpy(lista_extrato[numExtratos].cpf, cpf);
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(lista_extrato[numExtratos].data, 20, "%d/%m/%Y %H:%M:%S", tm_info);
    lista_extrato[numExtratos].valor = -valor;
    lista_extrato[numExtratos].tarifa = cliente->tipo == 2 ? 0.05 * valor : 0.03 * valor;
    lista_extrato[numExtratos].saldo = cliente->saldo;
    numExtratos++;

    arquivo_clientes();
    arquivo_extrato();
    printf("Ação bem sucedida\n");
}

void deposito() {
    char cpf[12];
    double valor;
    printf("Digite seu CPF: ");
    scanf("%s", cpf);
    printf("Digite o valor que será depositado: ");
    scanf("%lf", &valor);

    struct Cliente *cliente = buscar_cliente(cpf);
    if (cliente == NULL) {
        printf("Cliente não encontrado\n");
        return;
    }

    cliente->saldo += valor;

    strcpy(lista_extrato[numExtratos].cpf, cpf);
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(lista_extrato[numExtratos].data, 20, "%d/%m/%Y %H:%M:%S", tm_info);
    lista_extrato[numExtratos].valor = valor;
    lista_extrato[numExtratos].tarifa = 0.0;
    lista_extrato[numExtratos].saldo = cliente->saldo;
    numExtratos++;

    arquivo_clientes();
    arquivo_extrato();
    printf("O valor foi depositado com sucesso\n");
}

void extrato() {
    char cpf[12];
    char senha[20];
    printf("Digite seu CPF: ");
    scanf("%s", cpf);
    printf("Digite sua senha: ");
    scanf("%s", senha);

    struct Cliente *cliente = buscar_cliente(cpf);
    if (cliente == NULL || strcmp(senha, cliente->senha) != 0) {
        printf("CPF ou senha incorretos\n");
        return;
    }

    for (int i = 0; i < numExtratos; i++) {
        if (strcmp(cpf, lista_extrato[i].cpf) == 0) {
            printf("Data: %s, Valor: %.2lf, Tarifa: %.2lf, Saldo: %.2lf\n", lista_extrato[i].data, lista_extrato[i].valor, lista_extrato[i].tarifa, lista_extrato[i].saldo);
        }
    }
}

void transferencia() {
    char cpf[12];
    char senha[20];
    char cpf_dest[12];
    printf("Digite seu CPF: ");
    scanf("%s", cpf);
    printf("Digite sua senha: ");
    scanf("%s", senha);
    printf("Digite o CPF do destinatário: ");
    scanf("%s", cpf_dest);

    struct Cliente *cliente = buscar_cliente(cpf);
    struct Cliente *cliente_dest = buscar_cliente(cpf_dest);

    if (cliente == NULL || cliente_dest == NULL) {
        printf("Cliente não encontrado\n");
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

    strcpy(lista_extrato[numExtratos].cpf, cpf);
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(lista_extrato[numExtratos].data, 20, "%d/%m/%Y %H:%M:%S", tm_info);
    lista_extrato[numExtratos].valor = -valor;
    lista_extrato[numExtratos].tarifa = 0.0;
    lista_extrato[numExtratos].saldo = cliente->saldo;
    numExtratos++;

    strcpy(lista_extrato[numExtratos].cpf, cpf_dest);
    lista_extrato[numExtratos].valor = valor;
    lista_extrato[numExtratos].tarifa = 0.0;
    lista_extrato[numExtratos].saldo = cliente_dest->saldo;
    numExtratos++;

    arquivo_clientes();
    arquivo_extrato();
    printf("Ação bem sucedida\n");
}



///////////////////////////////////////////////////////////////////////////////////////////////



int main() {
    ler_arquivo_clientes();
    ler_arquivo_extrato();

    int opcao;
    while (1) {
        printf("1 Novo cliente\n 2 Apagar cliente\n 3.Listar clientes\n 4 Débito\n 5 Depósito\n 6 Extrato\n 7 Transferência entre contas\n 8 Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                novo_cliente();
                break;
            case 2:
                apagar_cliente();
                break;
            case 3:
                listar_clientes();
                break;
            case 4:
                debito();
                break;
            case 5:
                deposito();
                break;
            case 6:
                extrato();
                break;
            case 7:
                transferencia();
                break;
            case 8:
                arquivo_clientes();
                arquivo_extrato();
                printf("Ação encerrada\n");
                return 0;
            default:
                printf("Opção inválida\n");
        }
    }

    return 0;
}
