#include <gtest/gtest.h>
#include "parser.h"
#include "scanner.h"
#include "exp.h"

int main( int argc , char **argv ) {
    string input, context = "";
    Parser * p;
    while (true) {
        do {
            if (context == "") {
                std::cout << "?-";
            } else {
                std::cout << "|   ";
            }
            getline(std::cin, input);

            if (input != "") {
                while (input[0] == ' ') {
                    input = input.substr(1, input.size() - 1);
                }
            }
            context += input;
        } while (input == "" || context.back() == ',' || context.back() == ';');

        if (context == "halt.") {
            return 0;
        }

        p = new Parser(Scanner(context));
        try {
            p->buildExpression();
            Exp * exp = p->getExpressionTree();
            exp->evaluate();
            std::cout << exp->getExpString() << std::endl;
        } catch (string &msg) {
            std::cout << msg << std::endl;
        }
        context = "";
    }
}
