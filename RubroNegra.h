#ifndef RUBRO_NEGRA_H
#define RUBRO_NEGRA_H

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// Conjunto de identificadores inteiros, sem valores repetidos.
// Folhas ausentes compartilham o sentinela preto NIL (nao e uma chave).
// Altura: arvore vazia = 0; folha real = 1; NIL nao entra na altura publica.
class RubroNegra {
public:
    RubroNegra();
    ~RubroNegra();
    RubroNegra(const RubroNegra&) = delete;
    RubroNegra& operator=(const RubroNegra&) = delete;

    bool inserir(int chave); // false se a chave ja existe
    bool remover(int chave); // false se a chave nao existe
    bool buscar(int chave) const;
    std::vector<int> emOrdem() const;
    void imprimirEmOrdem(std::ostream& saida = std::cout) const;
    // Arvore deitada: direito acima, esquerdo abaixo; V=vermelho, P=preto.
    void imprimir(std::ostream& saida = std::cout) const;

    bool vazia() const;
    std::size_t tamanho() const;
    int altura() const; // O(n): calcula a altura dos nos reais
    bool validar() const; // O(n): BST, cores, altura negra, pais e tamanho
    void limpar(); // Libera os nos reais; preserva os contadores.

    // Uma rotacao simples vale 1; duas rotacoes valem 2.
    std::size_t obterRotacoes() const;
    // Conta somente mudancas efetivas de cor em nos reais existentes.
    // Cor inicial de um novo no e cor do sentinela nao contam.
    std::size_t obterRecoloracoes() const;
    void zerarEstatisticas();

private:
    enum class Cor { Vermelho, Preto };
    struct No {
        int chave;
        Cor cor;
        No* esquerda;
        No* direita;
        No* pai;
        No(int valor, Cor corInicial)
            : chave(valor), cor(corInicial), esquerda(nullptr),
              direita(nullptr), pai(nullptr) {}
    };

    No* nil_;
    No* raiz_;
    std::size_t quantidade_;
    std::size_t rotacoes_;
    std::size_t recoloracoes_;

    No* buscarNo(int chave) const;
    No* menorNo(No* no) const;
    void definirCor(No* no, Cor cor);
    void rotacionarEsquerda(No* no);
    void rotacionarDireita(No* no);
    void corrigirInsercao(No* no);
    void corrigirRemocao(No* no);
    void transplantar(No* antigo, No* novo);
    void destruir(No* no);
    int alturaNo(const No* no) const;
    void coletarEmOrdem(const No* no, std::vector<int>& valores) const;
    void imprimirNo(const No* no, const std::string& prefixo,
                     const std::string& rotulo, std::ostream& saida) const;
    bool validarNo(const No* no, const No* pai, long long minimo,
                    long long maximo, int& alturaNegra,
                    std::size_t& quantidade) const;
};

#endif