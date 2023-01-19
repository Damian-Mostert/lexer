# lexer
this is a lexer/tokenizer that can be configured for diffrent uses
### to configure lexer check out the **[include/lexer](/include/lexer)** directory
### to use lexer in your project you need the lexer.hpp file and lexer directory in your include path
then just simply use :
```cpp
#include"include/lexer.hpp"
```
or if your using a command like 'g++ file.cpp -o file -I./include' :
```cpp
#include<lexer.hpp>
```
### to configure lexer check out the **[lexer](/include/lexer)** directory

example :
```cpp
#include"include/lexer.hpp"
#include <fstream>
#include <iostream>
using namespace std;
int main(int argc,char*args[]){
    //read input file
    fstream file(args[1]);
    //convert file to string
    string script((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
    //close read file
    file.close();
    //lex file
    auto result = lexer::lex(script);
    //print tokens
    cout<<lexer::tools::tokens_to_string(result.result,"\033[95m");
}
```
your output should be something like this : 

<br> file data :

```
string z = "hello world"
```

```cmd
<Id line=0 column=0>string</Id>
<Id line=0 column=7>z</Id>
<Operator line=0 column=9>=</Operator>
<String line=0 column=11>"hello world"</String>
<Line line=0 column=24>\n</Line>
```

example with error handeling :

```cpp
#include"include/lexer.hpp"
#include <fstream>
#include <iostream>
using namespace std;
int main(int argc,char*args[]){
    //read input file
    fstream file(args[1]);
    //convert file to string
    string script((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
    //close read file
    file.close();
    //lex file
    auto result = lexer::lex(script);
    //check for error
    if(result.Error){
    //for each error
      for(int i = 0;i<result.ErrorMessages.size();i++){
        //print error data
        cout<<result.ErrorPositions[i].line<<":"<<result.ErrorPositions[i].column<<"\t"<<result.ErrorMessages[i]<<endl;
        //create and print error pointer
        cout<<lexer::tools::create_script_pointer(result.ErrorMessages[i],script,result.ErrorPositions[i]);    
      }
    }
    //print all tokens
    cout<<lexer::tools::tokens_to_string(result.result,"\033[96m");
}

```
your output should be something like this :

<br> file data :

```
string z = "hello world
```

```cmd
0:11	unclosed string
 1 | string z = "hello world
   |            ^ unclosed string
<Id line=0 column=0>string</Id>
<Id line=0 column=7>z</Id>
<Operator line=0 column=9>=</Operator>
<UnclosedString line=0 column=11>"</UnclosedString>
<Id line=0 column=11>hello</Id>
<Id line=0 column=17>world</Id>
<Line line=0 column=22>\n</Line>
```

### lex output structure :

```cpp
{
    bool Error=false;
    vector<string> ErrorMessages;
    vector<token::pos>ErrorPositions;
    vector<token>result;
}
```


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
string create_script_pointer(string message,string script,token::pos position){
      string e;
      vector<string>lines;
      string line;
      for(char&i:script){
        if(i=='\n'){
          lines.push_back(line);
          line="";
        }else{
          line+=i;
        }
      }
      if(lines.size()>position.line-1){e+=" "+to_string(position.line)+" | "+lines[position.line-1]+"\033[0m\n";}
      e+=" "+to_string(position.line+1)+" | "+lines[position.line]+"\n";
      int x = 0;e+="   | ";
      for(char&i:lines[position.line]){
        if(x<position.column){
          if(i=='\t'){
            e+=i;
          }else{
            e+=' ';
          }
        }
        x++;
      }
      e+="^ "+message+"\n";
      if(lines.size()>position.line+1){e+=" "+to_string(position.line+2)+" | "+lines[position.line+1]+"\033[0m\n";}
      return e;
    }
};
```


