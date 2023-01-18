# lexer
example :
```cpp
#include"include/lexer.hpp"
#include <fstream>
#include <iostream>
int main(int argc,char*args[]){
    fstream file(args[1]);
    string script((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
    file.close();
    auto result = lexer::lex(script);
    if(result.Error){
      for(int i = 0;i<result.ErrorMessages.size();i++){
        cout<<result.ErrorPositions[i].line<<":"<<result.ErrorPositions[i].column<<"\t"<<result.ErrorMessages[i]<<endl;
      }
    }
    cout<<lexer::tokens_to_string(result.result,"\033[95m");
}

```
