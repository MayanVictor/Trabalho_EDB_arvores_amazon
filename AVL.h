#ifndef AVL_H
#define AVL_H

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// Conjunto de identificadores inteiros, sem valores repetidos.
// Altura: arvore vazia = 0; folha = 1.
class AVL {
public:
    AVL();
    ~AVL();

    // Evita copias superficiais dos ponteiros e dupla liberacao de memoria.
    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    bool inserir(int chave); // false se a chave ja existe
    bool remover(int chave); // false se a chave nao existe
    bool buscar(int chave) const;
    std::vector<int> emOrdem() const;
    void imprimirEmOrdem(std::ostream& saida = std::cout) const;
    // Arvore deitada: filho direito acima, filho esquerdo abaixo.
    void imprimir(std::ostream& saida = std::cout) const;

    bool vazia() const;
    std::size_t tamanho() const;
    int altura() const; // O(1), pois a altura fica armazenada nos nos
    bool validar() const; // O(n): ordem BST, alturas, balanceamento e tamanho
    void limpar(); // Libera os nos; preserva os contadores.

    // Cada rotacao simples vale 1; uma rotacao dupla vale 2.
    std::size_t obterRotacoes() const;
    void zerarEstatisticas();

private:
    struct No {
        int chave;
        int altura;
        No* esquerda;
        No* direita;
        explicit No(int valor)
            : chave(valor), altura(1), esquerda(nullptr), direita(nullptr) {}
    };

    No* raiz_;
    std::size_t quantidade_;
    std::size_t rotacoes_;

    static int alturaNo(const No* no);
    static int fatorBalanceamento(const No* no);
    static void atualizarAltura(No* no);
    No* rotacionarDireita(No* no);
    No* rotacionarEsquerda(No* no);
    No* balancear(No* no);
    No* inserirNo(No* no, int chave, bool& inseriu);
    No* removerNo(No* no, int chave, bool& removeu);
    static No* menorNo(No* no);
    static void destruir(No* no);
    static void coletarEmOrdem(const No* no, std::vector<int>& valores);
    static void imprimirNo(const No* no, const std::string& prefixo,
                           const std::string& rotulo, std::ostream& saida);
    static bool validarNo(const No* no, long long minimo, long long maximo,
                          int& alturaCalculada, std::size_t& quantidade);
};

#endif
