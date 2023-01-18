#include<string>
#include<vector>
using namespace std;
namespace lexer{
  struct token{
  public:
    string value;
    struct{
      int line=0;
      int column=0;
    }position;
    enum types{
      TrippleString,
      String,
      Number,
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
      UnclosedTripleString,
      UnclosedScriptCancel
    }type;
    token(){}
    token(types Type,string data,int a,int b){type=Type;value=data;position.line=a;position.column=b;}
  };
  string token_to_String(token tk,string col){
    string t;
    switch(tk.type){
      case 0:t="TrippleString";break;
      case 1:t="String";break;
      case 2:t="Number";break;
      case 3:t="Boolean";break;
      case 4:t="Condition";break;
      case 5:t="Id";break;
      case 6:t="Operator";break;
      case 7:t="Bracket";break;
      case 8:t="LineCancel";break;
      case 9:t="ScriptCancel";break;
      case 10:t="Line";break;
      case 11:t="Comma";break;
      case 12:t="UnclosedString";break;
      case 13:t="UnclosedTripleString";break;
      case 14:t="UnclosedScriptCancel";break;
    }
    return col+"<"+t+" line="+to_string(tk.position.line)+" column="+to_string(tk.position.column)+">\033[0;0;0m"+tk.value+col+"</"+t+">\033[0;0;0m";
  }
  class tokenize{
  public:
    bool Error=false;
    string ErrorMessage;
    struct{int line=0;int column=0;}ErrorPosition;
    vector<token>result;
    tokenize(string data,int line,int column,int letter){
      string cap;
      char prev='\0';
      int colonescape=0;
      int colon=0;
      int doublecolon=0;
      int bentcolon=0;
      int tripplecolon=0;
      int trippledoublecolon=0;
      int tripplebentcolon=0;
      int number=0;
      int id=0;
      int lineCancel=0;
      int scriptCancel=0;
      string captured;
      int skip=0;
      int strln=0;
      int ogcol=0;
      int badnumber=0;
      int hasogccolbeendone=0;
      auto is_condition=[](string captured){
        if(captured =="is"||captured =="this"||captured =="function"||captured =="friend"||captured =="throw"||captured =="catch"||captured =="try"||captured =="function"||captured =="while"||captured =="for"||captured =="else"||captured =="if"||captured =="return"||captured =="class"||captured == "operator"){
          return true;
        }
        return false;
      };
      for(char&chuck:data){
        auto no_String = [colon,doublecolon,bentcolon,tripplecolon,trippledoublecolon,tripplebentcolon,colonescape](){
          if(
            colon||
            doublecolon||
            bentcolon||
            tripplecolon||
            tripplebentcolon||
            trippledoublecolon||
            colonescape
          ){
            return false;
          }
          return true;
        };
        if(skip){
          skip--;
        }else if(lineCancel||scriptCancel){
          if(lineCancel){
            if(chuck == '\n'){
              result.push_back(token(token::types::LineCancel,captured,line,(column-captured.length())));
              lineCancel = false;
              captured="";
            }else{
              captured+=chuck;
            }
          }
          if(scriptCancel){
            if(prev == '*'&&chuck == '/'){
              captured+=chuck;
              scriptCancel=false;
              string sliced="";
              if(hasogccolbeendone){
                result.push_back(token(token::types::ScriptCancel,captured,line-strln,ogcol));
                strln=0;
                ogcol=0;
                hasogccolbeendone=false;
              }else{
                result.push_back(token(token::types::ScriptCancel,captured,line,(column-captured.length())+1));
              }
              captured = "";
            }else if(chuck == '\n'){
              if(!hasogccolbeendone){
                ogcol = column-captured.length();
                hasogccolbeendone=true;
              }
              strln++;
              captured+=chuck;
            }else{
              captured+=chuck;
            }
          }
        }else if(colonescape){
          if(chuck == '\n'){
            strln+=1;
            if(!hasogccolbeendone){
              ogcol = column-captured.length();
              hasogccolbeendone=true;
            }
          }
          colonescape = false;
          captured+=chuck;
        }else if(chuck == '\\'&&(colon||doublecolon||bentcolon||tripplecolon||trippledoublecolon||tripplebentcolon)){
          captured+=chuck;
          colonescape=true;
        }else if(chuck == '\''&&[](int i,string script){
          if(script.length()>(i+1)){
            if(script[i+1] == '\''){
              if(script.length()>(i+2)){
                if(script[i+2] == '\''){
                  return true;
                }
                return false;
              }
            }
            return false;
          }
          return false;
        }(letter,data)&&no_String()){
          if(captured.length()){
            token::types t;
            if(id){
              if(is_condition(captured)){
                t = token::types::Condition;
                if(captured=="else"||captured=="catch"){
                  if(result.size()){
                    if(result[result.size()-1].type==token::types::Line){
                      result.pop_back();
                    }
                  }
                }
              }else if(captured == "true"||captured=="false"){
                t = token::types::Boolean;
              }else{
                t = token::types::Id;
              }
            }else if(number){
              t = token::types::Number;
            }
            result.push_back(token(t,captured,line,column-captured.length()));
            id=false;
            number=false;
            captured = "";
          }
          tripplecolon = true;
          captured+="'''";
          skip+=2;
        }else if(chuck == '"'&&[](int i,string script){
          if(script.length()>(i+1)){
            if(script[i+1] == '"'){
              if(script.length()>(i+2)){
                if(script[i+2] == '"'){
                  return true;
                }
                return false;
              }
            }
            return false;
          }
          return false;
        }(letter,data)&&no_String()){
          if(captured.length()){
            token::types t;
            if(id){
              if(is_condition(captured)){
                t = token::types::Condition;
                if(captured=="else"||captured=="catch"){
                  if(result.size()){
                    if(result[result.size()-1].type==token::types::Line){
                      result.pop_back();
                    }
                  }
                }
              }else if(captured == "true"||captured=="false"){
                t = token::types::Boolean;
              }else{
                t = token::types::Id;
              }
            }else if(number){
              t = token::types::Number;
            }
            result.push_back(token(t,captured,line,column-captured.length()));
            id=false;
            number=false;
            captured = "";
          }
          trippledoublecolon = true;
          captured+="\"\"\"";
          skip+=2;
        }else if(chuck == '`'&&[](int i,string script){
          if(script.length()>(i+1)){
            if(script[i+1] == '`'){
              if(script.length()>(i+2)){
                if(script[i+2] == '`'){
                  return true;
                }
                return false;
              }
            }
            return false;
          }
          return false;
        }(letter,data)&&no_String()){
          if(captured.length()){
            token::types t;
            if(id){
              if(is_condition(captured)){
                t = token::types::Condition;
                if(captured=="else"||captured=="catch"){
                  if(result.size()){
                    if(result[result.size()-1].type==token::types::Line){
                      result.pop_back();
                    }
                  }
                }
              }else if(captured == "true"||captured=="false"){
                t = token::types::Boolean;
              }else{
                t = token::types::Id;
              }
            }else if(number){
              t = token::types::Number;
            }
            result.push_back(token(t,captured,line,column-captured.length()));
            id=false;
            number=false;
            captured = "";
          }
          tripplebentcolon = true;
          captured+="```";
          skip+=2;
        }else if(chuck == '\''&&(no_String())){
          if(captured.length()){
            token::types t;
            if(id){
              if(is_condition(captured)){
                t = token::types::Condition;
                if(captured=="else"||captured=="catch"){
                  if(result.size()){
                    if(result[result.size()-1].type==token::types::Line){
                      result.pop_back();
                    }
                  }
                }
              }else if(captured == "true"||captured=="false"){
                t = token::types::Boolean;
              }else{
                t = token::types::Id;
              }
            }else if(number){
              t = token::types::Number;
            }
            result.push_back(token(t,captured,line,column-captured.length()));
            id=false;
            number=false;
            captured = "";
          }
          colon = true;
          captured+=chuck;
        }else if(chuck == '"'&&no_String()){
          if(captured.length()){
            token::types t;
            if(id){
              if(is_condition(captured)){
                t = token::types::Condition;
                if(captured=="else"||captured=="catch"){
                  if(result.size()){
                    if(result[result.size()-1].type==token::types::Line){
                      result.pop_back();
                    }
                  }
                }
              }else if(captured == "true"||captured=="false"){
                t = token::types::Boolean;
              }else{
                t = token::types::Id;
              }
            }else if(number){
              t = token::types::Number;
            }
            result.push_back(token(t,captured,line,column-captured.length()));
            id=false;
            number=false;
            captured = "";
          }
          doublecolon = true;
          captured+=chuck;
        }else if(chuck == '`'&&no_String()){
          if(captured.length()){
            token::types t;
            if(id){
              if(is_condition(captured)){
                t = token::types::Condition;
                if(captured=="else"||captured=="catch"){
                  if(result.size()){
                    if(result[result.size()-1].type==token::types::Line){
                      result.pop_back();
                    }
                  }
                }
              }else if(captured == "true"||captured=="false"){
                t = token::types::Boolean;
              }else{
                t = token::types::Id;
              }
            }else if(number){
              t = token::types::Number;
            }
            result.push_back(token(t,captured,line,column-captured.length()));
            id=false;
            number=false;
            captured = "";
          }
          bentcolon = true;
          captured+=chuck;
        }else if(bentcolon||doublecolon||colon||tripplecolon||trippledoublecolon||tripplebentcolon){
          captured+=chuck;
          if(chuck == '\n'){
            strln+=1;
            if(!hasogccolbeendone){
              ogcol = column-captured.length();
              hasogccolbeendone=true;
            }
          }else if(colon){
            if(chuck == '\''){
              colon = false;
              if(strln){
                result.push_back(token(token::types::String,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::String,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
            }
          }else if(doublecolon){
            if(chuck == '"'){
              doublecolon = false;
              if(strln){
                result.push_back(token(token::types::String,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::String,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
            }
          }else if(tripplecolon){
            if(chuck == '\''&&[](int i,string script){
              if(script.length()>(i+1)){
                if(script[i+1] == '\''){
                  if(script.length()>(i+2)){
                    if(script[i+2] == '\''){
                      return true;
                    }
                    return false;
                  }
                }
                return false;
              }
              return false;
            }(letter,data)){
              captured+="''";
              tripplecolon = false;
              if(strln){
                result.push_back(token(token::types::TrippleString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::TrippleString,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
              skip+=2;
            }
          }else if(tripplebentcolon){
            if(chuck == '`'&&[](int i,string script){
              if(script.length()>(i+1)){
                if(script[i+1] == '`'){
                  if(script.length()>(i+2)){
                    if(script[i+2] == '`'){
                      return true;
                    }
                    return false;
                  }
                }
                return false;
              }
              return false;
            }(letter,data)){
              captured+="``";
              tripplebentcolon = false;
              if(strln){
                result.push_back(token(token::types::TrippleString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::TrippleString,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
              skip+=2;
            }
          }else if(trippledoublecolon){
            if(chuck == '"'&&[](int i,string script){
              if(script.length()>(i+1)){
                if(script[i+1] == '"'){
                  if(script.length()>(i+2)){
                    if(script[i+2] == '"'){
                      return true;
                    }
                    return false;
                  }
                }
                return false;
              }
              return false;
            }(letter,data)){
              captured+="\"\"";
              trippledoublecolon = false;
              if(strln){
                result.push_back(token(token::types::TrippleString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::TrippleString,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
              skip+=2;
            }
          }else if(bentcolon){
            if(chuck == '`'){
              bentcolon=false;
              if(strln){
                result.push_back(token(token::types::String,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::String,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
            }
          }
        }else{
          if(isspace(chuck)){
            int p = 0;
            if(captured.length()){
              token::types t;
              if(id){
                if(is_condition(captured)){
                  t = token::types::Condition;
                  if(captured=="else"||captured=="catch"){
                    if(result.size()){
                      if(result[result.size()-1].type==token::types::Line){
                        result.pop_back();
                      }
                    }
                  }
                }else if(captured == "true"||captured=="false"){
                  t = token::types::Boolean;
                }else{
                  t = token::types::Id;
                }
              }else if(number){
                t = token::types::Number;
              }
              result.push_back(token(t,captured,line,column-captured.length()));
              id=false;
              number=false;
              captured = "";
            }
            if(chuck == '\n'){
              if(result.size()){
                if(result[result.size()-1].type != token::types::Condition&&result[result.size()-1].type != token::types::ScriptCancel&&result[result.size()-1].type != token::types::LineCancel&&result[result.size()-1].type != token::types::Line&&result[result.size()-1].type != token::types::Operator){
                  result.push_back(token(token::types::Line,"\\n",line,column));
                }
              }
            }
          }else if(isalpha(chuck)||chuck=='_'){
            if(number){
              number=false;
              result.push_back(token(token::types::Number,captured,line,column));
              captured="";
              id = true;
              captured+=chuck;
            }else{
              id = true;
              captured+=chuck;
            }
          }else if(isdigit(chuck)){
            if(!id){
              number = true;
            }
            captured+=chuck;
          }else {
            if(chuck=='.'&&number){
              captured+=chuck;
            }else if(captured.length()){
              token::types t;
              if(id){
                if(is_condition(captured)){
                  t = token::types::Condition;
                  if(captured=="else"||captured=="catch"){
                    if(result.size()){
                      if(result[result.size()-1].type==token::types::Line){
                        result.pop_back();
                      }
                    }
                  }
                }else if(captured == "true"||captured=="false"){
                  t = token::types::Boolean;
                }else{
                  t = token::types::Id;
                }
              }else if(number){
                t = token::types::Number;
              }
              result.push_back(token(t,captured,line,column-captured.length()));
              id=false;
              number=false;
              captured = "";
            }
            if(chuck == '}'||chuck == ']'||chuck == ')'||chuck == '{'||chuck == '['||chuck == '('){
              result.push_back(token(token::types::Bracket,string("")+chuck,line,column));
            }else if(chuck == ';'||chuck == ','){
              result.push_back(token(token::types::Comma,string("")+chuck,line,column));
            }else{
              if(chuck == '/'&&prev=='/'){
                lineCancel=true;
                captured+=prev;
                captured+=chuck;
                result.pop_back();
              }else if(chuck == '*'&&prev=='/'){
                scriptCancel=true;
                captured+=prev;
                captured+=chuck;
                result.pop_back();
              }else if(chuck =='/'){
                if(data.length()>letter+1){
                  if(data[letter+1]=='*'||data[letter+1]=='/'){
                    result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                  }else{
                    result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                  }
                }else{
                  result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                }
              }else{
                if(chuck=='.'&&number){
                  if((data.length()==(letter+1))||(!isdigit(data[letter+1]))){
                    Error = true;
                    ErrorMessage="bad number";
                    ErrorPosition.line=line;
                    ErrorPosition.column=column;
                  }
                  bool dot_has_been_set = false;
                  int v=0;
                  for(char&c:captured){
                    if(!dot_has_been_set&&c=='.'){
                      dot_has_been_set=true;
                    }else if(c=='.'){
                      captured[v]='0';
                    }
                    v++;
                  }
                }else{
                  if(result.size()){
                    if(result[result.size()-1].type==token::types::Operator){
                      result[result.size()-1].value+=chuck;
                    }else{
                      if(result[result.size()-1].type==token::types::Line){
                        result.pop_back();
                      }
                      result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                    }
                  }else{

                    result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                  }
                }
              }
            }
          }
        }
        if(chuck == '\n'){
          line++;
          column = 0;
        }else{
          column++;
        }
        letter++;
        prev = chuck;
      }
      if(captured.length()){
        if(bentcolon||doublecolon||colon||tripplebentcolon||trippledoublecolon||tripplecolon){
          Error = true;
          ErrorMessage="unexpected token '*' script cancel was never closed";
          ErrorPosition.line=line-strln;
          ErrorPosition.column=ogcol+1;
          string n;
          for(int c=1;c<captured.length();c++){n+=captured[c];}
          int a = line-strln;
          int b = int(letter-captured.length())+1;
          tokenize added(n,a,ogcol+1,b);
          for(int i = 0;i<added.result.size();i++){
            result.push_back(added.result[i]);
          }
        }else if(lineCancel){
          result.push_back(token(token::types::LineCancel,captured,line,column));
        }else if(scriptCancel){
          Error = true;
          ErrorMessage="unexpected token '*' script cancel was never closed";
          ErrorPosition.line=line-strln;
          ErrorPosition.column=ogcol;
          string n;
          for(int c=2;c<captured.length();c++){n+=captured[c];}
          int a = line-strln;
          int b = int(letter-captured.length())+1;
          tokenize added(n,a,ogcol+1,b);
          for(int i = 0;i<added.result.size();i++){
            result.push_back(added.result[i]);
          }
        }else if(number){
          result.push_back(token(token::types::Number,captured,line,column));
        }else{
          result.push_back(token(token::types::Id,captured,line,column));
        }
      }
    }
  };
  tokenize lex(string data){
    return tokenize(data,0,0,0);
  }
};
