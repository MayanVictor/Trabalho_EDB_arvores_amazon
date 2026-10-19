#include "RubroNegra.h"

#include <algorithm>
#include <limits>

RubroNegra::RubroNegra()
    : nil_(new No(0, Cor::Preto)), raiz_(nil_), quantidade_(0),
      rotacoes_(0), recoloracoes_(0) {
    // Auto-referencias permitem consultar a cor dos filhos de NIL.
    nil_->esquerda = nil_;
    nil_->direita = nil_;
    nil_->pai = nil_;
}

RubroNegra::~RubroNegra() {
    limpar();
    delete nil_;
}

void RubroNegra::definirCor(No* no, Cor cor) {
    if (no != nil_ && no->cor != cor) {
        no->cor = cor;
        ++recoloracoes_;
    }
}

void RubroNegra::rotacionarEsquerda(No* no) {
    No* novaRaiz = no->direita;
    no->direita = novaRaiz->esquerda;
    if (novaRaiz->esquerda != nil_) novaRaiz->esquerda->pai = no;
    novaRaiz->pai = no->pai;
    if (no->pai == nil_) raiz_ = novaRaiz;
    else if (no == no->pai->esquerda) no->pai->esquerda = novaRaiz;
    else no->pai->direita = novaRaiz;
    novaRaiz->esquerda = no;
    no->pai = novaRaiz;
    ++rotacoes_;
}

void RubroNegra::rotacionarDireita(No* no) {
    No* novaRaiz = no->esquerda;
    no->esquerda = novaRaiz->direita;
    if (novaRaiz->direita != nil_) novaRaiz->direita->pai = no;
    novaRaiz->pai = no->pai;
    if (no->pai == nil_) raiz_ = novaRaiz;
    else if (no == no->pai->direita) no->pai->direita = novaRaiz;
    else no->pai->esquerda = novaRaiz;
    novaRaiz->direita = no;
    no->pai = novaRaiz;
    ++rotacoes_;
}

bool RubroNegra::inserir(int chave) {
    No* pai = nil_;
    No* atual = raiz_;
    while (atual != nil_) {
        pai = atual;
        if (chave == atual->chave) return false;
        atual = chave < atual->chave ? atual->esquerda : atual->direita;
    }
    No* novo = new No(chave, Cor::Vermelho);
    novo->esquerda = nil_;
    novo->direita = nil_;
    novo->pai = pai;
    if (pai == nil_) raiz_ = novo;
    else if (chave < pai->chave) pai->esquerda = novo;
    else pai->direita = novo;
    ++quantidade_;
    corrigirInsercao(novo);
    return true;
}

void RubroNegra::corrigirInsercao(No* no) {
    while (no->pai->cor == Cor::Vermelho) {
        No* avo = no->pai->pai;
        if (no->pai == avo->esquerda) {
            No* tio = avo->direita;
            if (tio->cor == Cor::Vermelho) {
                // Pai e tio vermelhos: recolore e continua pelo avo.
                definirCor(no->pai, Cor::Preto);
                definirCor(tio, Cor::Preto);
                definirCor(avo, Cor::Vermelho);
                no = avo;
            } else {
                if (no == no->pai->direita) {
                    // Triangulo: converte para uma linha.
                    no = no->pai;
                    rotacionarEsquerda(no);
                }
                definirCor(no->pai, Cor::Preto);
                definirCor(no->pai->pai, Cor::Vermelho);
                rotacionarDireita(no->pai->pai);
            }
        } else {
            // Casos espelhados: pai e filho direito do avo.
            No* tio = avo->esquerda;
            if (tio->cor == Cor::Vermelho) {
                definirCor(no->pai, Cor::Preto);
                definirCor(tio, Cor::Preto);
                definirCor(avo, Cor::Vermelho);
                no = avo;
            } else {
                if (no == no->pai->esquerda) {
                    no = no->pai;
                    rotacionarDireita(no);
                }
                definirCor(no->pai, Cor::Preto);
                definirCor(no->pai->pai, Cor::Vermelho);
                rotacionarEsquerda(no->pai->pai);
            }
        }
    }
    definirCor(raiz_, Cor::Preto);
}

RubroNegra::No* RubroNegra::buscarNo(int chave) const {
    No* atual = raiz_;
    while (atual != nil_ && chave != atual->chave)
        atual = chave < atual->chave ? atual->esquerda : atual->direita;
    return atual;
}

bool RubroNegra::buscar(int chave) const { return buscarNo(chave) != nil_; }

RubroNegra::No* RubroNegra::menorNo(No* no) const {
    while (no->esquerda != nil_) no = no->esquerda;
    return no;
}

void RubroNegra::transplantar(No* antigo, No* novo) {
    if (antigo->pai == nil_) raiz_ = novo;
    else if (antigo == antigo->pai->esquerda) antigo->pai->esquerda = novo;
    else antigo->pai->direita = novo;
    // Mesmo NIL precisa saber o pai temporariamente para corrigir a remocao.
    novo->pai = antigo->pai;
}

bool RubroNegra::remover(int chave) {
    No* alvo = buscarNo(chave);
    if (alvo == nil_) return false;
    No* substituto = alvo;
    Cor corRemovida = substituto->cor;
    No* ajuste = nil_;
    if (alvo->esquerda == nil_) {
        ajuste = alvo->direita;
        transplantar(alvo, alvo->direita);
    } else if (alvo->direita == nil_) {
        ajuste = alvo->esquerda;
        transplantar(alvo, alvo->esquerda);
    } else {
        // Move o sucessor para a posicao do alvo, preservando a ordem BST.
        substituto = menorNo(alvo->direita);
        corRemovida = substituto->cor;
        ajuste = substituto->direita;
        if (substituto->pai == alvo) {
            ajuste->pai = substituto;
        } else {
            transplantar(substituto, substituto->direita);
            substituto->direita = alvo->direita;
            substituto->direita->pai = substituto;
        }
        transplantar(alvo, substituto);
        substituto->esquerda = alvo->esquerda;
        substituto->esquerda->pai = substituto;
        definirCor(substituto, alvo->cor);
    }
    delete alvo;
    --quantidade_;
    // A perda de um preto exige restaurar a altura negra dos caminhos.
    if (corRemovida == Cor::Preto) corrigirRemocao(ajuste);
    nil_->pai = nil_;
    return true;
}

void RubroNegra::corrigirRemocao(No* no) {
    while (no != raiz_ && no->cor == Cor::Preto) {
        if (no == no->pai->esquerda) {
            No* irmao = no->pai->direita;
            // Caso 1: irmao vermelho; transforma em um caso com irmao preto.
            if (irmao->cor == Cor::Vermelho) {
                definirCor(irmao, Cor::Preto);
                definirCor(no->pai, Cor::Vermelho);
                rotacionarEsquerda(no->pai);
                irmao = no->pai->direita;
            }
            if (irmao->esquerda->cor == Cor::Preto
                && irmao->direita->cor == Cor::Preto) {
                // Caso 2: sobrinhos pretos; propaga a falta de um preto ao pai.
                definirCor(irmao, Cor::Vermelho);
                no = no->pai;
            } else {
                if (irmao->direita->cor == Cor::Preto) {
                    // Caso 3: sobrinho distante preto; prepara o caso 4.
                    definirCor(irmao->esquerda, Cor::Preto);
                    definirCor(irmao, Cor::Vermelho);
                    rotacionarDireita(irmao);
                    irmao = no->pai->direita;
                }
                // Caso 4: sobrinho distante vermelho; restaura e termina.
                definirCor(irmao, no->pai->cor);
                definirCor(no->pai, Cor::Preto);
                definirCor(irmao->direita, Cor::Preto);
                rotacionarEsquerda(no->pai);
                no = raiz_;
            }
        } else {
            // Os quatro casos espelhados.
            No* irmao = no->pai->esquerda;
            if (irmao->cor == Cor::Vermelho) {
                definirCor(irmao, Cor::Preto);
                definirCor(no->pai, Cor::Vermelho);
                rotacionarDireita(no->pai);
                irmao = no->pai->esquerda;
            }
            if (irmao->direita->cor == Cor::Preto
                && irmao->esquerda->cor == Cor::Preto) {
                definirCor(irmao, Cor::Vermelho);
                no = no->pai;
            } else {
                if (irmao->esquerda->cor == Cor::Preto) {
                    definirCor(irmao->direita, Cor::Preto);
                    definirCor(irmao, Cor::Vermelho);
                    rotacionarEsquerda(irmao);
                    irmao = no->pai->esquerda;
                }
                definirCor(irmao, no->pai->cor);
                definirCor(no->pai, Cor::Preto);
                definirCor(irmao->esquerda, Cor::Preto);
                rotacionarDireita(no->pai);
                no = raiz_;
            }
        }
    }
    definirCor(no, Cor::Preto);
}

void RubroNegra::coletarEmOrdem(const No* no, std::vector<int>& valores) const {
    if (no == nil_) return;
    coletarEmOrdem(no->esquerda, valores);
    valores.push_back(no->chave);
    coletarEmOrdem(no->direita, valores);
}

std::vector<int> RubroNegra::emOrdem() const {
    std::vector<int> valores;
    valores.reserve(quantidade_);
    coletarEmOrdem(raiz_, valores);
    return valores;
}

void RubroNegra::imprimirEmOrdem(std::ostream& saida) const {
    const auto valores = emOrdem();
    for (std::size_t i = 0; i < valores.size(); ++i) {
        if (i != 0) saida << ' ';
        saida << valores[i];
    }
    saida << '\n';
}

void RubroNegra::imprimirNo(const No* no, const std::string& prefixo,
                            const std::string& rotulo, std::ostream& saida) const {
    if (no == nil_) return;
    imprimirNo(no->direita, prefixo + "    ", "D: ", saida);
    saida << prefixo << rotulo << no->chave
          << (no->cor == Cor::Vermelho ? " [V]\n" : " [P]\n");
    imprimirNo(no->esquerda, prefixo + "    ", "E: ", saida);
}

void RubroNegra::imprimir(std::ostream& saida) const {
    if (vazia()) saida << "(arvore vazia)\n";
    else {
        saida << "V = vermelho; P = preto; folhas NIL pretas omitidas\n";
        imprimirNo(raiz_, "", "Raiz: ", saida);
    }
}

bool RubroNegra::vazia() const { return raiz_ == nil_; }
std::size_t RubroNegra::tamanho() const { return quantidade_; }
std::size_t RubroNegra::obterRotacoes() const { return rotacoes_; }
std::size_t RubroNegra::obterRecoloracoes() const { return recoloracoes_; }
void RubroNegra::zerarEstatisticas() { rotacoes_ = 0; recoloracoes_ = 0; }

int RubroNegra::alturaNo(const No* no) const {
    if (no == nil_) return 0;
    return 1 + std::max(alturaNo(no->esquerda), alturaNo(no->direita));
}

int RubroNegra::altura() const { return alturaNo(raiz_); }

void RubroNegra::destruir(No* no) {
    if (no == nil_) return;
    destruir(no->esquerda);
    destruir(no->direita);
    delete no;
}

void RubroNegra::limpar() {
    destruir(raiz_);
    raiz_ = nil_;
    nil_->pai = nil_;
    quantidade_ = 0;
}

bool RubroNegra::validarNo(const No* no, const No* pai, long long minimo,
                           long long maximo, int& alturaNegra,
                           std::size_t& quantidade) const {
    if (no == nil_) {
        alturaNegra = 1; // Inclui NIL, igualmente em todos os caminhos.
        return true;
    }
    if (no == nullptr || no->esquerda == nullptr || no->direita == nullptr)
        return false;
    if (no->pai != pai || no->chave <= minimo || no->chave >= maximo)
        return false;
    if (no->cor != Cor::Preto && no->cor != Cor::Vermelho) return false;
    if (no->cor == Cor::Vermelho
        && (no->esquerda->cor == Cor::Vermelho || no->direita->cor == Cor::Vermelho))
        return false;
    int esquerda = 0;
    int direita = 0;
    if (!validarNo(no->esquerda, no, minimo, no->chave, esquerda, quantidade)
        || !validarNo(no->direita, no, no->chave, maximo, direita, quantidade))
        return false;
    if (esquerda != direita) return false;
    alturaNegra = esquerda + (no->cor == Cor::Preto ? 1 : 0);
    ++quantidade;
    return true;
}

bool RubroNegra::validar() const {
    if (nil_->cor != Cor::Preto || nil_->esquerda != nil_
        || nil_->direita != nil_ || nil_->pai != nil_) return false;
    if (raiz_ != nil_ && (raiz_->pai != nil_ || raiz_->cor != Cor::Preto))
        return false;
    int alturaNegra = 0;
    std::size_t quantidade = 0;
    return validarNo(raiz_, nil_, std::numeric_limits<long long>::lowest(),
                     std::numeric_limits<long long>::max(), alturaNegra, quantidade)
           && quantidade == quantidade_;
}
