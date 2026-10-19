#include "AVL.h"

#include <algorithm>
#include <limits>

AVL::AVL() : raiz_(nullptr), quantidade_(0), rotacoes_(0) {}

AVL::~AVL() {
    limpar();
}

int AVL::alturaNo(const No* no) {
    return no == nullptr ? 0 : no->altura;
}

int AVL::fatorBalanceamento(const No* no) {
    return no == nullptr ? 0 : alturaNo(no->esquerda) - alturaNo(no->direita);
}

void AVL::atualizarAltura(No* no) {
    no->altura = 1 + std::max(alturaNo(no->esquerda), alturaNo(no->direita));
}

AVL::No* AVL::rotacionarDireita(No* no) {
    No* novaRaiz = no->esquerda;
    No* subarvoreCentral = novaRaiz->direita;
    novaRaiz->direita = no;
    no->esquerda = subarvoreCentral;
    // Atualiza primeiro o no que desceu, depois o que subiu.
    atualizarAltura(no);
    atualizarAltura(novaRaiz);
    ++rotacoes_;
    return novaRaiz;
}

AVL::No* AVL::rotacionarEsquerda(No* no) {
    No* novaRaiz = no->direita;
    No* subarvoreCentral = novaRaiz->esquerda;
    novaRaiz->esquerda = no;
    no->direita = subarvoreCentral;
    atualizarAltura(no);
    atualizarAltura(novaRaiz);
    ++rotacoes_;
    return novaRaiz;
}

AVL::No* AVL::balancear(No* no) {
    if (no == nullptr) return nullptr;
    atualizarAltura(no);
    const int fator = fatorBalanceamento(no);
    if (fator > 1) {
        // Esquerda-direita: converte em esquerda-esquerda.
        if (fatorBalanceamento(no->esquerda) < 0)
            no->esquerda = rotacionarEsquerda(no->esquerda);
        return rotacionarDireita(no);
    }
    if (fator < -1) {
        // Direita-esquerda: converte em direita-direita.
        if (fatorBalanceamento(no->direita) > 0)
            no->direita = rotacionarDireita(no->direita);
        return rotacionarEsquerda(no);
    }
    return no;
}

AVL::No* AVL::inserirNo(No* no, int chave, bool& inseriu) {
    if (no == nullptr) {
        No* novo = new No(chave);
        inseriu = true;
        return novo;
    }
    if (chave < no->chave)
        no->esquerda = inserirNo(no->esquerda, chave, inseriu);
    else if (chave > no->chave)
        no->direita = inserirNo(no->direita, chave, inseriu);
    else
        return no; // Duplicata: nao altera a arvore.
    return inseriu ? balancear(no) : no;
}

bool AVL::inserir(int chave) {
    bool inseriu = false;
    raiz_ = inserirNo(raiz_, chave, inseriu);
    if (inseriu) ++quantidade_;
    return inseriu;
}

AVL::No* AVL::menorNo(No* no) {
    while (no->esquerda != nullptr) no = no->esquerda;
    return no;
}

AVL::No* AVL::removerNo(No* no, int chave, bool& removeu) {
    if (no == nullptr) return nullptr;
    if (chave < no->chave) {
        no->esquerda = removerNo(no->esquerda, chave, removeu);
        } else {
        removeu = true;
        if (no->esquerda == nullptr || no->direita == nullptr) {
            No* filho = no->esquerda != nullptr ? no->esquerda : no->direita;
            delete no;
            return filho; // Serve tambem para folha (filho == nullptr).
        }
        // Dois filhos: substitui a chave pelo sucessor em ordem e o remove.
        No* sucessor = menorNo(no->direita);
        no->chave = sucessor->chave;
        no->direita = removerNo(no->direita, sucessor->chave, removeu);
    }
    // A remocao pode exigir ajustes em varios ancestrais ate a raiz.
    return removeu ? balancear(no) : no;
}

bool AVL::remover(int chave) {
    bool removeu = false;
    raiz_ = removerNo(raiz_, chave, removeu);
    if (removeu) --quantidade_;
    return removeu;
}

bool AVL::buscar(int chave) const {
    const No* atual = raiz_;
    while (atual != nullptr) {
        if (chave == atual->chave) return true;
        atual = chave < atual->chave ? atual->esquerda : atual->direita;
    }
    return false;
}

void AVL::coletarEmOrdem(const No* no, std::vector<int>& valores) {
    if (no == nullptr) return;
    coletarEmOrdem(no->esquerda, valores);
    valores.push_back(no->chave);
    coletarEmOrdem(no->direita, valores);
}

std::vector<int> AVL::emOrdem() const {
    std::vector<int> valores;
    valores.reserve(quantidade_);
    coletarEmOrdem(raiz_, valores);
    return valores;
}

void AVL::imprimirEmOrdem(std::ostream& saida) const {
    const auto valores = emOrdem();
    for (std::size_t i = 0; i < valores.size(); ++i) {
        if (i != 0) saida << ' ';
        saida << valores[i];
    }
    saida << '\n';
}

void AVL::imprimirNo(const No* no, const std::string& prefixo,
                     const std::string& rotulo, std::ostream& saida) {
    if (no == nullptr) return;
    imprimirNo(no->direita, prefixo + "    ", "D: ", saida);
    saida << prefixo << rotulo << no->chave
          << " [h=" << no->altura << ", fb=" << fatorBalanceamento(no) << "]\n";
    imprimirNo(no->esquerda, prefixo + "    ", "E: ", saida);
}

void AVL::imprimir(std::ostream& saida) const {
    if (vazia()) saida << "(arvore vazia)\n";
    else imprimirNo(raiz_, "", "Raiz: ", saida);
}

bool AVL::vazia() const { return raiz_ == nullptr; }
std::size_t AVL::tamanho() const { return quantidade_; }
int AVL::altura() const { return alturaNo(raiz_); }
std::size_t AVL::obterRotacoes() const { return rotacoes_; }
void AVL::zerarEstatisticas() { rotacoes_ = 0; }

void AVL::destruir(No* no) {
    if (no == nullptr) return;
    destruir(no->esquerda);
    destruir(no->direita);
    delete no;
}

void AVL::limpar() {
    destruir(raiz_);
    raiz_ = nullptr;
    quantidade_ = 0;
}

bool AVL::validarNo(const No* no, long long minimo, long long maximo,
                    int& alturaCalculada, std::size_t& quantidade) {
    if (no == nullptr) {
        alturaCalculada = 0;
        return true;
    }
    if (no->chave <= minimo || no->chave >= maximo) return false;
    int alturaEsquerda = 0;
    int alturaDireita = 0;
    if (!validarNo(no->esquerda, minimo, no->chave, alturaEsquerda, quantidade)
        || !validarNo(no->direita, no->chave, maximo, alturaDireita, quantidade))
        return false;
    alturaCalculada = 1 + std::max(alturaEsquerda, alturaDireita);
    ++quantidade;
    const int fator = alturaEsquerda - alturaDireita;
    return no->altura == alturaCalculada && fator >= -1 && fator <= 1;
}

bool AVL::validar() const {
    int alturaCalculada = 0;
    std::size_t quantidade = 0;
    return validarNo(raiz_, std::numeric_limits<long long>::lowest(),
                     std::numeric_limits<long long>::max(), alturaCalculada, quantidade)
           && quantidade == quantidade_;
}