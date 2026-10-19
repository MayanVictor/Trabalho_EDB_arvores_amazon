#include "AVL.h"
#include "RubroNegra.h"

#include <iostream>
#include <sstream>
#include <string>

// Le uma linha inteira: rejeita letras, decimais e valores fora de int.
// Retorna false quando a entrada termina, permitindo encerrar o programa.
bool lerInteiro(const std::string& mensagem, int& valor) {
    std::string linha;
    while (true) {
        std::cout << mensagem;
        if (!std::getline(std::cin, linha)) return false;
        std::istringstream entrada(linha);
        char sobra;
        if ((entrada >> valor) && !(entrada >> sobra)) return true;
        std::cout << "Entrada invalida. Digite um numero inteiro.\n";
    }
}

void mostrarRecoloracoes(const AVL&) {
    // AVL nao utiliza cores.
}

void mostrarRecoloracoes(const RubroNegra& arvore) {
    std::cout << "Recoloracoes: " << arvore.obterRecoloracoes() << '\n';
}

// As duas classes possuem as mesmas operacoes publicas.
// O template permite usar este menu com qualquer uma delas.
template <typename TipoArvore>
void executarMenu(TipoArvore& arvore, const std::string& nome) {
    int opcao;
    int valor;
    while (true) {
        std::cout << "\n--- " << nome << " ---\n"
                  << "1 - Inserir\n"
                  << "2 - Remover\n"
                  << "3 - Buscar\n"
                  << "4 - Mostrar em ordem crescente\n"
                  << "5 - Mostrar estrutura\n"
                  << "6 - Mostrar estatisticas\n"
                  << "7 - Validar propriedades\n"
                  << "8 - Limpar arvore\n"
                  << "9 - Zerar contadores\n"
                  << "0 - Voltar para escolha de arvore\n";
        if (!lerInteiro("Opcao: ", opcao)) return;

        switch (opcao) {
        case 1:
            if (!lerInteiro("Identificador para inserir: ", valor)) return;
            std::cout << (arvore.inserir(valor)
                ? "Inserido com sucesso.\n"
                : "Valor repetido: insercao rejeitada.\n");
            break;
        case 2:
            if (!lerInteiro("Identificador para remover: ", valor)) return;
            std::cout << (arvore.remover(valor)
                ? "Removido com sucesso.\n"
                : "Identificador inexistente.\n");
            break;
        case 3:
            if (!lerInteiro("Identificador para buscar: ", valor)) return;
            std::cout << (arvore.buscar(valor)
                ? "Identificador encontrado.\n"
                : "Identificador inexistente.\n");
            break;
        case 4:
            if (arvore.vazia()) std::cout << "Arvore vazia.\n";
            else arvore.imprimirEmOrdem();
            break;
        case 5:
            std::cout << "Direita acima; esquerda abaixo.\n"
                      << "Na rubro-negra: P = preto; V = vermelho.\n";
            arvore.imprimir();
            break;
        case 6:
            std::cout << "Elementos: " << arvore.tamanho() << '\n'
                      << "Altura: " << arvore.altura() << '\n'
                      << "Rotacoes: " << arvore.obterRotacoes() << '\n';
            mostrarRecoloracoes(arvore);
            break;
        case 7:
            std::cout << (arvore.validar()
                ? "Propriedades validas.\n"
                : "ERRO: propriedades invalidas!\n");
            break;
        case 8:
            arvore.limpar();
            std::cout << "Arvore limpa. Os contadores foram preservados.\n";
            break;
        case 9:
            arvore.zerarEstatisticas();
            std::cout << "Contadores zerados.\n";
            break;
        case 0:
            return;
        default:
            std::cout << "Opcao invalida.\n";
        }
    }
}

int main() {
    // Cada arvore preserva seus elementos ao voltar ao menu inicial.
    // Os destrutores liberam a memoria quando o programa termina.
    AVL avl;
    RubroNegra rubroNegra;
    int escolha;

    while (true) {
        std::cout << "\n=== Testes de arvores balanceadas ===\n"
                  << "1 - AVL\n"
                  << "2 - Rubro-negra\n"
                  << "0 - Encerrar\n";
        if (!lerInteiro("Escolha: ", escolha)) break;
        if (escolha == 0) break;
        if (escolha == 1) executarMenu(avl, "Arvore AVL");
        else if (escolha == 2) executarMenu(rubroNegra, "Arvore rubro-negra");
        else std::cout << "Opcao invalida.\n";
        if (!std::cin) break;
    }

    std::cout << "Programa encerrado.\n";
    return 0;
}