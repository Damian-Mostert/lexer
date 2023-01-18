string get_token_type_as_string(token tk){
  switch(tk.type){
    case 0:return "TrippleString";break;
    case 1:return "String";break;
    case 2:return "Int";break;
    case 3:return "Boolean";break;
    case 4:return "Condition";break;
    case 5:return "Id";break;
    case 6:return "Operator";break;
    case 7:return "Bracket";break;
    case 8:return "LineCancel";break;
    case 9:return "ScriptCancel";break;
    case 10:return "Line";break;
    case 11:return "Comma";break;
    case 12:return "UnclosedString";break;
    case 13:return "Double";break;
  }
  return "Null";
}
string token_to_string(token tk,string col){
  string t=get_token_type_as_string(tk);
  return col+"<"+t+" line="+to_string(tk.position.line)+" column="+to_string(tk.position.column)+">\033[0;0;0m"+tk.value+col+"</"+t+">\033[0;0;0m";
}
string tokens_to_string(vector<token>tks,string col){
  string res;
  for(token&i:tks){
    res+=token_to_string(i,col)+"\n";
  }
  return res;
};
