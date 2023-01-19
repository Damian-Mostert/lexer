if(captured.length()){
  token::types t;
  if(id){
    if(is_condition(captured)||is_condition_with_no_line_at_start(captured)){
      t = token::types::Condition;
      if(is_condition_with_no_line_at_start(captured)){
        if(result.size()){
          if(enableAutoLineRemoval&&result[result.size()-1].type==token::types::Line){
            result.pop_back();
          }
        }
      }
    }else if(is_boolean(captured)){
      t = token::types::Boolean;
    }else{
      t = token::types::Id;
    }
  }else if(number){
    if(DoubleNumber){
      t=token::types::Double;
    }else{
      t=token::types::Int;
    }
  }
  result.push_back(token(t,captured,line,column-captured.length()));
  id=false;
  number=false;
  captured = "";
  DoubleNumber=0;
}
