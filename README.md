# lexer

this is a lexer/tokenizer that can be configured for diffrent uses

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
    cout<<lexer::tokens_to_string(result.result,"\033[95m");
}

```

exapmle with error handeling :

```cpp
#include"include/lexer.hpp"
#include <fstream>
#include <iostream>
using namespace std;
int main(int argc,char*args[]){
    fstream file(args[1]);
    string script((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
    file.close();
    auto result = lexer::lex(script);
    //check for error
    if(result.Error){
    //for each error
      for(int i = 0;i<result.ErrorMessages.size();i++){
        //print error data
        cout<<result.ErrorPositions[i].line<<":"<<result.ErrorPositions[i].column<<"\t"<<result.ErrorMessages[i]<<endl;
      }
    }
    cout<<lexer::tools::tokens_to_string(result.result,"\033[96m");
}

```

to configure lexer check out **[lexer config](/include/lexer)**
