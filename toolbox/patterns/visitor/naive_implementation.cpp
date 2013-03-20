
// NAIVE implementation
// of the visitor pattern

#include <iostream>
#include <vector>

struct E1;
struct E2;
struct EMax;

struct Visitor {

    void visit(E1 &v) {
        std::cout << "I visit E1" << std::endl;
    };

    void visit(E2 &v) {
        std::cout << "I visit E2" << std::endl;
    };

    void visit(EMax &v) {
        std::cout << "I visit EMax" << std::endl;
    };

};

struct ElementBase {
    virtual void accept(Visitor &v) = 0;
};

struct E1 : public ElementBase {

    void accept(Visitor &v) {
        v.visit(*this);
    }

};

struct E2 : public ElementBase {
    void accept(Visitor &v) {
        v.visit(*this);
    }
};

struct EMax : public ElementBase {
    std::vector<ElementBase*> elems;

    EMax() {
        elems.push_back(new E1());
        elems.push_back(new E2());
    }

    void accept(Visitor &v) {
        std::vector<ElementBase *>::iterator it = elems.begin(), ite = elems.end();
        while (it != ite) {
            (*it)->accept(v);
            ++it;
        }
        v.visit(*this);
    }
};

int main(int argc, char *argv[])
{
    EMax em;
    Visitor v;

    em.accept(v);
    return 0;
}
