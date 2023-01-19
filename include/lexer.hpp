#include <cstring>
#include<string>
#include<vector>
namespace lexer{
  using namespace std;
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
    string parse_string(){

    }
    string parse_tripple_string(){

    }
    string create_script_pointer(){

    }
  }

  class lex{
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
    bool is_allowed_id_char_boolean(char i){
      if(
        [i](vector<char>c){
          for(char&v:c){
            if(i==v){
              return true;
            }
          }
          return false;
        }(AllowedSymbolsInId)
      ){
        return true;
      }
      return false;
    };
  public:
    #include"lexer/config.hpp"
    bool Error=false;
    vector<string> ErrorMessages;
    struct pos{int line=0;int column=0;};
    vector<pos>ErrorPositions;
    vector<token>result;
    lex(){}
    lex(string data){
      char prev='\0';
      int line=0;
      int column=0;
      int letter=0;
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
      for(int o =0;o<=data.size();o++){
        char chuck = data[o];
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
        if(o==data.size()){
          if(captured.length()){
            if(!no_String()||scriptCancel){
              string n;
              int c;
              if(tripplebentcolon||trippledoublecolon||tripplecolon){
                ErrorMessages.push_back("unclosed tripple string");
                ErrorPositions.push_back({line-strln,ogcol+1});
                result.push_back(token(token::types::UnclosedString,string("")+captured[0]+captured[1]+captured[2],line-strln,ogcol+1));
                c=2;
                Error = true;
              }else if(scriptCancel){
                if(EnableAutoCLoseStarDashCancel){
                  result.push_back(token(token::types::ScriptCancel,captured+"*/",line-strln,ogcol+1));
                  if(!Error){
                    Error=false;
                  }
                }else{
                  ErrorMessages.push_back("unclosed script cancel");
                  ErrorPositions.push_back({line-strln,ogcol+1});
                  result.push_back(token(token::types::UnclosedScriptCancel,string("")+captured[0]+captured[1],line-strln,ogcol+1));
                  Error = true;
                }
                c=1;
              }else{
                ErrorMessages.push_back("unclosed string");
                ErrorPositions.push_back({line-strln,ogcol+1});
                result.push_back(token(token::types::UnclosedString,string("")+captured[0],line-strln,ogcol+1));
                c=0;
                Error = true;
              }
              if(Error){
                for(c;c<captured.length();c++){n+=captured[c];}
                letter -=captured.length();
                column= ogcol;
                line -=strln;
                prev='\0';
                colonescape=0;
                colon=0;
                doublecolon=0;
                bentcolon=0;
                tripplecolon=0;
                trippledoublecolon=0;
                tripplebentcolon=0;
                number=0;
                id=0;
                lineCancel=0;
                scriptCancel=0;
                captured="";
                skip=0;
                strln=0;
                ogcol=0;
                badnumber=0;
                DoubleNumber=0;
                hasogccolbeendone=0;
                data=n;
                o=0;
              }
            }else if(lineCancel){
              result.push_back(token(token::types::LineCancel,captured,line,column));
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
        }else if(skip){
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
        }else if(enableTrippleStrings&&chuck == '\''&&[](int i,string script){
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
        }(o,data)&&no_String()){
          #include"lexer/lex::push_captured.hpp"
          tripplecolon = true;
          captured+="'''";
          skip+=2;
        }else if(enableTrippleStrings&&chuck == '"'&&[](int i,string script){
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
        }(o,data)&&no_String()){
          #include"lexer/lex::push_captured.hpp"
          trippledoublecolon = true;
          captured+="\"\"\"";
          skip+=2;
        }else if(enableTrippleStrings&&chuck == '`'&&[](int i,string script){
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
        }(o,data)&&no_String()){
          #include"lexer/lex::push_captured.hpp"
          tripplebentcolon = true;
          captured+="```";
          skip+=2;
        }else if(enableStrings&&chuck == '\''&&(no_String())){
          #include"lexer/lex::push_captured.hpp"
          colon = true;
          captured+=chuck;
        }else if(enableStrings&&chuck == '"'&&no_String()){
          #include"lexer/lex::push_captured.hpp"
          doublecolon = true;
          captured+=chuck;
        }else if(enableStrings&&chuck == '`'&&no_String()){
          #include"lexer/lex::push_captured.hpp"
          bentcolon = true;
          captured+=chuck;
        }else if(!no_String()){
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
            }(o,data)){
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
            }(o,data)){
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
            }(o,data)){
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
            #include"lexer/lex::push_captured.hpp"
            if(enableAutoLineRemoval&&chuck == '\n'){
              if(result.size()){
                if(result[result.size()-1].type != token::types::Condition&&result[result.size()-1].type != token::types::ScriptCancel&&result[result.size()-1].type != token::types::LineCancel&&result[result.size()-1].type != token::types::Line&&result[result.size()-1].type != token::types::Operator){
                  result.push_back(token(token::types::Line,"\\n",line,column));
                }
              }
            }else if(!NoLines&&chuck=='\n'){
              result.push_back(token(token::types::Line,"\\n",line,column));
            }
          }else if(isalpha(chuck)||is_allowed_id_char_boolean(chuck)){
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
            if(EnableDoubleNumbers&&chuck=='.'&&number){
              captured+=chuck;
            }else
            #include"lexer/lex::push_captured.hpp"
            if(chuck == '}'||chuck == ']'||chuck == ')'||chuck == '{'||chuck == '['||chuck == '('){
              result.push_back(token(token::types::Bracket,string("")+chuck,line,column));
              if(!AllowBrackets){
                Error = true;
                ErrorMessages.push_back("unexpected token");
                ErrorPositions.push_back({line,column});
              }
            }else if(chuck == ';'||chuck == ','){
              result.push_back(token(token::types::Comma,string("")+chuck,line,column));
            }else{
              if(EnableDoubleDashCancel&&(chuck == '/'&&prev=='/')){
                lineCancel=true;
                captured+=prev;
                captured+=chuck;
                result.pop_back();
              }else if(EnableHashCancel&&(chuck == '#')){
                lineCancel=true;
                captured+=chuck;
              }else if(EnableStarDashCancel&&(chuck == '*'&&prev=='/')){
                scriptCancel=true;
                captured+=prev;
                captured+=chuck;
                result.pop_back();
              }else if(EnableStarDashCancel&&chuck =='/'){
                if(data.length()>o+1){
                  if(data[o+1]=='*'||data[o+1]=='/'){
                    result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                  }else{
                    result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                  }
                }else{
                  result.push_back(token(token::types::Operator,string("")+chuck,line,column));
                }
              }else{
                if(EnableDoubleNumbers&&chuck=='.'&&number){
                  DoubleNumber=1;
                  if((data.length()==(letter+1))||(!isdigit(data[o+1]))){
                    if(EnableInvalidDoubleNumbers){
                      Error = true;
                      ErrorMessages.push_back("invalid number");
                      ErrorPositions.push_back({line,column});
                    }
                    string newcap;
                    for(int z=0;z<captured.length()-1;z++){
                      newcap+=captured[z];
                    }
                    captured=newcap;
                    #include"lexer/lex::push_captured.hpp"
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
                }else if(
                  chuck == '='||
                  chuck == '+'||
                  chuck == '-'||
                  chuck == '*'||
                  chuck == '/'||
                  chuck == '!'||
                  chuck == '?'||
                  chuck == '<'||
                  chuck == '>'||
                  chuck == '&'||
                  chuck == '|'||
                  chuck == '~'||
                  chuck == '^'||
                  chuck == '%'||
                  chuck == ':'||
                  chuck == '.'||
                  chuck == '@'
                ){
                  //handel operator fixing
                  bool isValid = false;
                  string Operator;
                  if(data.size()>o+1){
                    if(data.size()>o+2){
                      for(string&s:ValidTriOperators){
                        if(s == string()+chuck+string()+data[o+1]+string()+data[o+2]){
                          isValid=true;
                          Operator=chuck;
                          Operator+=data[o+1];
                          Operator+=data[o+2];
                          skip+=2;
                        }
                      }
                      if(!isValid){
                        for(string&s:ValidDualOperators){
                          if(s == string()+chuck+string()+data[o+1]){
                            isValid=true;
                            Operator=chuck;
                            Operator+=data[o+1];
                            skip++;
                          }
                        }
                      }
                      if(!isValid){
                        for(string&s:ValidOperators){
                          if(s == string()+chuck){
                            isValid=true;
                            Operator=chuck;
                          }
                        }
                      }
                    }else{
                      if(!isValid){
                        for(string&s:ValidDualOperators){
                          if(s == string()+chuck+data[o+1]){
                            isValid=true;
                            Operator=chuck;
                            Operator+=data[o+1];
                            skip++;
                          }
                        }
                      }
                      if(!isValid){
                        for(string&s:ValidOperators){
                          if(s == string()+chuck){
                            isValid=true;
                            Operator=chuck;
                          }
                        }
                      }
                    }
                  }else{
                    for(string&s:ValidOperators){
                      if(s == string()+chuck){
                        isValid=true;
                        Operator=chuck;
                      }
                    }
                  }
                  if(!isValid){
                    Error = true;
                    ErrorMessages.push_back("invalid operator");
                    ErrorPositions.push_back({line,column});
                    result.push_back(token(token::types::InvalidOperator,string()+chuck,line,column));
                  }else{
                    result.push_back(token(token::types::Operator,Operator,line,column));
                  }
                }else{
                  Error = true;
                  ErrorMessages.push_back("stray character");
                  ErrorPositions.push_back({line,column});
                  result.push_back(token(token::types::InvalidOperator,string()+chuck,line,column));
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

    }
  };

};
