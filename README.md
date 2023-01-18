# lexer
example :
```cpp
#include"lexer.hpp"
#include <fstream>
#include <iostream>
int main(int argc,char*args[]){
    fstream file(args[1]);
    string script((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
    file.close();
    auto result = lexer::lex(script);
    if(result.Error){
      cout<<result.ErrorPosition.line<<":"<<result.ErrorPosition.column<<"\t"<<result.ErrorMessage<<endl;
    }
    cout<<lexer::tokens_to_string(result.result,"\033[95m");
}
```
