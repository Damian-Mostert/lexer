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

example with error handeling :

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

### to configure lexer check out the **[include/lexer](/include/lexer)** directory

### using lexer tokens :

token structure :

```cpp
  struct token{
  public:
    string value;
    struct pos{
      int line=0;
      int column=0;
    }position;
    enum types{
      TrippleString,
      String,
      Int,
      Boolean,
      Condition,
      Id,
      Operator,
      Bracket,
      LineCancel,
      ScriptCancel,
      Line,
      Comma,
      UnclosedString,
      Double,
      UnclosedScriptCancel,
      InvalidOperator,
      InvalidDouble,
    }type;
    token(){}
    token(types NewType,string NewValue,int NewLine,int NewColumn){
      value=NewValue;
      type=NewType;
      position.line=NewLine;
      position.column=NewColumn;
    }
  };

```

using the token tools : 



