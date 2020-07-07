#include <iostream>
#include <string>
#include <brim.h>

using namespace brim;
using namespace std;

int main() {
    
    string text("5+5");
    auto tokens = scan(text.c_str());

    for (usize i = 0; i < tokens.get_length(); i++) {
        Token token = tokens[i];
        switch (token.type) {
            case TokenType::Number:
                cout << "Number: " << token.data.number << "\n";
            break;
            default:
                cout << "Autre\n";
            break;
        }
    }
    
    return 0;
}