#include <iostream>
#include <vector>

// Loki visitor header
#include "Visitor.h"

struct E1;
struct E2;
struct EMax;

struct Visitor : public Loki::BaseVisitor,
                 public Loki::Visitor<LOKI_TYPELIST_3(E1, E2, EMax)> {

    void Visit(E1 &v) {
        std::cout << "I visit E1" << std::endl;
    };

    void Visit(E2 &v) {
        std::cout << "I visit E2" << std::endl;
    };

    void Visit(EMax &v) {
        std::cout << "I visit EMax" << std::endl;
    };

};

struct E1 : public Loki::BaseVisitable<void, Loki::DefaultCatchAll, false> {
   LOKI_DEFINE_VISITABLE()
};

struct E2 : public Loki::BaseVisitable<void, Loki::DefaultCatchAll, false> {
   LOKI_DEFINE_VISITABLE()
};

struct EMax : public Loki::BaseVisitable<void, Loki::DefaultCatchAll, false> {

    LOKI_DEFINE_VISITABLE()

    EMax() {
    }

};

int main(int argc, char *argv[])
{
    EMax em;
    Visitor v;

    em.Accept(v);
    return 0;
}
