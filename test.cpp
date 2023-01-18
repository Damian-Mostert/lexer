#include"lexer.hpp"
#include <fstream>
#include <iostream>

int main(int argc,char*args[]){
    fstream file(args[1]);
    string script((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
    file.close();
    auto result = lexer::lex(script);
    for(auto&i:result.result){
      cout<<lexer::token_to_String(i,"\033[91m")<<endl;
    }
}
