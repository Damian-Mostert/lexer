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
      Space,
      Parentheses,
      Squere,
      Curly
    }type;
    vector<token>tokens;
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
        case 17:return"Space";break;
        case 18:return"Parentheses";break;
        case 19:return"Squere";break;
        case 20:return"Curly";break;
      }
      return "Null";
    }
    class token_to_string{
    public:
      string res;
      token_to_string(token tk,string col){
        res = token_to_string(tk,col,0).res;
      }
      token_to_string(token tk,string col,int deep){
        string t=get_token_type_as_string(tk);
        string tbs="";
        for(int i =0;i<deep;i++){tbs+="\t";}
        if(tk.type==token::types::Parentheses||tk.type==token::types::Squere||tk.type==token::types::Curly){
          res=tbs+col+"<"+t+" line="+to_string(tk.position.line)+" column="+to_string(tk.position.column)+">\033[0;0;0m\n";
          for(auto&i:tk.tokens){
            res+=token_to_string(i,col,deep+1).res;
          }
          res+=tbs+col+"</"+t+">\033[0;0;0m\n";
        }else{
          res =  tbs+col+"<"+t+" line="+to_string(tk.position.line)+" column="+to_string(tk.position.column)+">\033[0;0;0m"+string(tk.value)+col+"</"+t+">\033[0;0;0m\n";
        }
      }
    };
    string tokens_to_string(vector<token>tks,string col){
      string res;
      for(token&i:tks){
        res+=token_to_string(i,col).res;
      }
      return res;
    };
    token parse_string(token in){
      token result = in;
      if(in.type==token::types::String){
        string input_string;
        bool escape = false;
        int skip=0;
        for(int i = 1;i<in.value.length()-1;i++){
          if(skip){
            skip--;
          }else if(escape){
            if(in.value[i] == 'n'){
              input_string+="\n";
            }else if(in.value[i] == 'v'){
              input_string+="\v";
            }else if(in.value[i] == 't'){
              input_string+="\t";
            }else if(in.value[i] == 'f'){
              input_string+="\f";
            }else if(in.value[i] == '0'){
              if(i+1>in.value.length()){
                if(i+2>in.value.length()){
                  if(in.value[i+1]=='3'&&in.value[i+2]=='3'){
                    skip+=2;
                    input_string+="\033";
                  }else if(in.value[i+1]=='X'&&in.value[i+2]=='b'){
                    skip+=2;
                    input_string+="\0Xb";
                  }else{
                    input_string+="\0";
                  }
                }else{
                  input_string+="\0";
                }
              }else{
                input_string+="\0";
              }
            }else{
              input_string+=in.value[i];
            }
            escape=false;
          }else if(in.value[i] == '\\'){
            escape=true;
          }else{
            input_string+=in.value[i];
          }
        }
        result.value=input_string;
      }
      return result;
    }
    token parse_tripple_string(token in){
      token result;

      return result;
    }
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
    vector<token::pos>ErrorPositions;
    vector<token>result;
    lex(){}
    lex(string data){
      lex s(data,0,0,0);
      Error=s.Error;
      ErrorMessages=s.ErrorMessages;
      ErrorPositions=s.ErrorPositions;
      result=s.result;
    }
    lex(string data,int letter,int line,int column){
      char prev='\0';
      token::pos bracket_start;
      int bracket_start_letter = 0;
      int parentheses = 0;
      int squere = 0;
      int curly = 0;
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
      string firstchuck;
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
          if(captured.length()||parentheses||curly||squere){
            if(!no_String()||scriptCancel||parentheses||curly||squere){
              string n;
              int c;
              if(parentheses||curly||squere){
                captured = firstchuck+captured;
                Error=true;
                ErrorMessages.push_back("unclosed bracket");
                ErrorPositions.push_back(bracket_start);
                string t;
                if(parentheses){
                  t="(";
                }else if(curly){
                  t="{";
                }else if(squere){
                  t="[";
                }
                result.push_back(token(token::types::Bracket,t,bracket_start.line,bracket_start.column));
                c=0;
                strln=0;
                line=bracket_start.line;
                ogcol = bracket_start.column-1;
              }else if(tripplebentcolon||trippledoublecolon||tripplecolon){
                ErrorMessages.push_back("unclosed tripple string");
                ErrorPositions.push_back({line-strln,ogcol+1});
                result.push_back(token(token::types::UnclosedString,string("")+captured[0]+captured[1]+captured[2],line-strln,ogcol+1));
                c=2;
                Error = true;
                ogcol+=2;
              }else if(scriptCancel){
                if(EnableAutoCLoseStarDashCancel){
                  result.push_back(token(token::types::ScriptCancel,captured+"*/",line-strln,ogcol+1));
                  if(!Error){
                    Error=false;
                  }
                }else{
                  ErrorMessages.push_back("unclosed script cancel");
                  ErrorPositions.push_back({line-strln,ogcol});
                  result.push_back(token(token::types::UnclosedScriptCancel,string("")+captured[0]+captured[1],line-strln,ogcol));
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
                column= ogcol+1;
                line -=strln;
                prev='\0';
                parentheses = 0;
                squere = 0;
                curly = 0;
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
        }else if(parentheses){
          if(chuck == '('){
            parentheses++;
            captured+=chuck;
          }else if(chuck == ')'){
            parentheses--;
            if(parentheses){
              captured+=chuck;
            }else if(!parentheses){
              lex sub(captured,bracket_start_letter+1,bracket_start.line,bracket_start.column+1);
              token Parentheses;
              Parentheses.position = bracket_start;
              Parentheses.type=token::types::Parentheses;
              Parentheses.tokens = sub.result;
              result.push_back(Parentheses);
              if(sub.Error){
                Error=true;
                for(int x=0;x<sub.ErrorMessages.size();x++){
                  ErrorMessages.push_back(sub.ErrorMessages[x]);
                }
                for(int x=0;x<sub.ErrorPositions.size();x++){
                  ErrorPositions.push_back(sub.ErrorPositions[x]);
                }
              }
              captured="";
            }
          }else{
            captured+=chuck;
          }
        }else if(squere){
          if(chuck == '['){
            squere++;
            captured+=chuck;
          }else if(chuck == ']'){
            squere--;
            if(squere){
              captured+=chuck;
            }else if(!squere){
              lex sub(captured,bracket_start_letter+1,bracket_start.line,bracket_start.column+1);
              token Squere;
              Squere.position = bracket_start;
              Squere.type=token::types::Squere;
              Squere.tokens = sub.result;
              result.push_back(Squere);
              if(sub.Error){
                Error=true;
                for(int x=0;x<sub.ErrorMessages.size();x++){
                  ErrorMessages.push_back(sub.ErrorMessages[x]);
                }
                for(int x=0;x<sub.ErrorPositions.size();x++){
                  ErrorPositions.push_back(sub.ErrorPositions[x]);
                }
              }
              captured="";
            }
          }else{
            captured+=chuck;
          }
        }else if(curly){
          if(chuck == '{'){
            curly++;
            captured+=chuck;
          }else if(chuck == '}'){
            curly--;
            if(curly){
              captured+=chuck;
            }else if(!curly){
              lex sub(captured,bracket_start_letter+1,bracket_start.line,bracket_start.column+1);
              token Curly;
              Curly.position = bracket_start;
              Curly.type=token::types::Curly;
              Curly.tokens = sub.result;
              result.push_back(Curly);
              if(sub.Error){
                Error=true;
                for(int x=0;x<sub.ErrorMessages.size();x++){
                  ErrorMessages.push_back(sub.ErrorMessages[x]);
                }
                for(int x=0;x<sub.ErrorPositions.size();x++){
                  ErrorPositions.push_back(sub.ErrorPositions[x]);
                }
              }
              captured="";
            }
          }else{
            captured+=chuck;
          }
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
          }
          if(colon){
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
            }else if(!allowLinesInStrings&&chuck == '\n'){
              Error=true;
              ErrorMessages.push_back("unclosed string");
              ErrorPositions.push_back({line,column});
              colon = false;
              if(strln){
                result.push_back(token(token::types::UnclosedString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::UnclosedString,captured,line,column-captured.length()+1));
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
            }else if(!allowLinesInStrings&&chuck == '\n'){
              Error=true;
              ErrorMessages.push_back("unclosed string");
              ErrorPositions.push_back({line,column});
              doublecolon = false;
              if(strln){
                result.push_back(token(token::types::UnclosedString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::UnclosedString,captured,line,column-captured.length()+1));
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
            }else if(!allowLinesInTrippleStrings&&chuck == '\n'){
              Error=true;
              ErrorMessages.push_back("unclosed string");
              ErrorPositions.push_back({line,column});
              tripplecolon = false;
              if(strln){
                result.push_back(token(token::types::UnclosedString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::UnclosedString,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
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
            }else if(!allowLinesInTrippleStrings&&chuck == '\n'){
              Error=true;
              ErrorMessages.push_back("unclosed string");
              ErrorPositions.push_back({line,column});
              tripplebentcolon = false;
              if(strln){
                result.push_back(token(token::types::UnclosedString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::UnclosedString,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
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
            }else if(!allowLinesInTrippleStrings&&chuck == '\n'){
              Error=true;
              ErrorMessages.push_back("unclosed string");
              ErrorPositions.push_back({line,column});
              trippledoublecolon = false;
              if(strln){
                result.push_back(token(token::types::UnclosedString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::UnclosedString,captured,line,column-captured.length()+1));
              }
              hasogccolbeendone=false;
              captured="";
              strln=0;
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
            }else if(!allowLinesInStrings&&chuck == '\n'){
              Error=true;
              ErrorMessages.push_back("unclosed string");
              ErrorPositions.push_back({line,column});
              bentcolon = false;
              if(strln){
                result.push_back(token(token::types::UnclosedString,captured,line-strln,ogcol+1));
              }else{
                result.push_back(token(token::types::UnclosedString,captured,line,column-captured.length()+1));
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
            }else if(!NoSpaces&&chuck==' '||chuck=='\t'||chuck=='\v'||chuck=='\f'){
              string c = " ";
              if(chuck=='\t'){
                c = "\\t";
              }else if(chuck=='\v'){
                c = "\\v";
              }else if(chuck=='\f'){
                c = "\\f";
              }
              result.push_back(token(token::types::Space,c,line,column));
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
            if(AllowBrackets&&(chuck == '}'||chuck == ']'||chuck == ')'||chuck == '{'||chuck == '['||chuck == '(')){
              if(segmentizeBrackets){
                if(chuck == '('){
                  parentheses++;
                  bracket_start_letter = letter;
                  bracket_start.column = column;
                  bracket_start.line = line;
                  if(data.length()>o+1){
                    firstchuck = data[o+1];
                  }
                }else if(chuck == '['){
                  squere++;
                  bracket_start_letter = letter;
                  bracket_start.column = column;
                  bracket_start.line = line;
                  if(data.length()>o+1){
                    firstchuck = data[o+1];
                  }
                }else if(chuck == '{'){
                  curly++;
                  bracket_start_letter = letter;
                  bracket_start.column = column;
                  bracket_start.line = line;
                  if(data.length()>o+1){
                    firstchuck = data[o+1];
                  }
                }else{
                  result.push_back(token(token::types::Bracket,string("")+chuck,line,column));
                  Error = true;
                  ErrorMessages.push_back("unexpected bracket");
                  ErrorPositions.push_back({line,column});
                }
              }else{
                result.push_back(token(token::types::Bracket,string("")+chuck,line,column));
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
