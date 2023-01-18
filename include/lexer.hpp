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
    }type;
    token(){}
    token(types Type,string data,int a,int b){type=Type;value=data;position.line=a;position.column=b;}
  };
  #include"lexer/lexer-tools.hpp"
  #include"lexer-config.hpp"
  auto lex(string data){
    class tokenize{
      bool is_condition(string captured){
        if(
          [captured](vector<string>c){
            for(string&i:c){
              if(i==captured){
                return true;
              }
            }
            return false;
          }(ConditionalWords)
        ){
          return true;
        }
        return false;
      };
      bool is_condition_with_no_line_at_start(string captured){
        if(
          [captured](vector<string>c){
            for(string&i:c){
              if(i==captured){
                return true;
              }
            }
            return false;
          }(ConditionalWordsWithNoLineAtStart)
        ){
          return true;
        }
        return false;
        return false;
      }
      bool is_boolean(string captured){
        if(
          [captured](vector<string>c){
            for(string&i:c){
              if(i==captured){
                return true;
              }
            }
            return false;
          }(BooleanWords)
        ){
          return true;
        }
        return false;
      };
    public:
      bool Error=false;
      vector<string> ErrorMessages;
      struct pos{int line=0;int column=0;};
      vector<pos>ErrorPositions;
      vector<token>result;
      tokenize(string data,int line,int column,int letter){
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
        int DoubleNumber=0;
        int hasogccolbeendone=0;
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
            #include"lexer/push_last.hpp"
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
            #include"lexer/push_last.hpp"
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
            #include"lexer/push_last.hpp"
            tripplebentcolon = true;
            captured+="```";
            skip+=2;
          }else if(chuck == '\''&&(no_String())){
            #include"lexer/push_last.hpp"
            colon = true;
            captured+=chuck;
          }else if(chuck == '"'&&no_String()){
            #include"lexer/push_last.hpp"
            doublecolon = true;
            captured+=chuck;
          }else if(chuck == '`'&&no_String()){
            #include"lexer/push_last.hpp"
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
                  result.push_back(token(token::types::TrippleString,captured,line-strln,ogcol));
                }else{
                  result.push_back(token(token::types::TrippleString,captured,line,column-captured.length()+3));
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
                  result.push_back(token(token::types::TrippleString,captured,line-strln,ogcol));
                }else{
                  result.push_back(token(token::types::TrippleString,captured,line,column-captured.length()+3));
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
                  result.push_back(token(token::types::TrippleString,captured,line-strln,ogcol));
                }else{
                  result.push_back(token(token::types::TrippleString,captured,line,column-captured.length()+3));
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
              #include"lexer/push_last.hpp"
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
                if(DoubleNumber){
                  result.push_back(token(token::types::Double,captured,line,column));
                }else{
                  result.push_back(token(token::types::Int,captured,line,column));
                }
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
              }else
              #include"lexer/push_last.hpp"
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
                    DoubleNumber=1;
                    if((data.length()==(letter+1))||(!isdigit(data[letter+1]))){
                      Error = true;
                      ErrorMessages.push_back("invalid number");
                      ErrorPositions.push_back({line,column});
                      string newcap;
                      for(int z=0;z<captured.length()-1;z++){
                        newcap+=captured[z];
                      }
                      captured=newcap;
                      #include"lexer/push_last.hpp"
                      result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                    }else{
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
                    }
                  }else{
                    result.push_back(token(token::types::Operator,string("")+chuck,line,column));
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
            string n;
            int c=1;
            Error = true;
            if(tripplebentcolon||trippledoublecolon||tripplecolon){
              ErrorMessages.push_back("unclosed tripple string");
              ErrorPositions.push_back({line-strln,ogcol+1});
              result.push_back(token(token::types::UnclosedString,string("")+captured[0]+captured[1]+captured[2],line-strln,ogcol+1));
              c=3;
            }else{
              ErrorMessages.push_back("unclosed string");
              ErrorPositions.push_back({line-strln,ogcol+1});
              result.push_back(token(token::types::UnclosedString,string("")+captured[0],line-strln,ogcol+1));
            }
            for(c;c<captured.length();c++){n+=captured[c];}
            int a = line-strln;
            int b = int(letter-captured.length())+1;
            tokenize added(n,a,ogcol+c,b);
            if(added.Error){
              for(int i = 0;i<added.ErrorMessages.size();i++){
                ErrorMessages.push_back(added.ErrorMessages[i]);
                ErrorPositions.push_back(added.ErrorPositions[i]);
              }
            }
            for(int i = 0;i<added.result.size();i++){
              result.push_back(added.result[i]);
            }
          }else if(lineCancel){
            result.push_back(token(token::types::LineCancel,captured,line,column));
          }else if(scriptCancel){
            captured+="*/";
            result.push_back(token(token::types::ScriptCancel,captured,line-strln,ogcol));
          }else if(number){
            if(DoubleNumber){
              result.push_back(token(token::types::Double,captured,line,column));
            }else{
              result.push_back(token(token::types::Int,captured,line,column));
            }
          }else{
            result.push_back(token(token::types::Id,captured,line,column));
          }
        }
      }
    };
    return tokenize(data,0,0,0);
  }
};
