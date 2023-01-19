# lexer

this is a lexer/tokenizer that can be configured for diffrent uses

example :
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

```cpp
namespace tools{
    string get_token_type_as_string(token tk){
      switch(tk.type){
        case 0:return"TrippleString";break;
        case 1:return"String";break;
        case 2:return"Int";break;
        case 3:return"Boolean";break;
        case 4:return"Condition";break;
        case 5:return"Id";break;
        case 6:return"Operator";break;
        case 7:return"Bracket";break;
        case 8:return"LineCancel";break;
        case 9:return"ScriptCancel";break;
        case 10:return"Line";break;
        case 11:return"Comma";break;
        case 12:return"UnclosedString";break;
        case 13:return"Double";break;
        case 14:return"UnclosedScriptCancel";break;
        case 15:return"InvalidOperator";break;
        case 16:return"InvalidNumber";break;
      }
      return "Null";
    }
    string token_to_string(token tk,string col){
      string t=get_token_type_as_string(tk);
      return col+"<"+t+" line="+to_string(tk.position.line)+" column="+to_string(tk.position.column)+">\033[0;0;0m"+string(tk.value)+col+"</"+t+">\033[0;0;0m";
    }
    string tokens_to_string(vector<token>tks,string col){
      string res;
      for(token&i:tks){
        res+=token_to_string(i,col)+"\n";
      }
      return res;
    };
};
```


