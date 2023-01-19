// strings in this vector will return Boolean type
vector<string>BooleanWords{"true","false","null","undefined"};

// strings in this vector will return Condition type
vector<string>ConditionalWords{"for","if","try","while""class","is","operator","function"};

// strings in this vector will return Condition type, and remove a line before it
vector<string>ConditionalWordsWithNoLineAtStart{"else","catch",};

// chars in this vector will be allowed in Id types, for eg "this_is$oneID"
vector<char>AllowedSymbolsInId{'$','_'};

// strings in this vector will be allowed single operators
vector<string>ValidOperators{"+","-","*","/","=","?","<",">","!",":","%","&","."};

// strings in this vector will be allowed dual operators
vector<string>ValidDualOperators{"++","--","==","<=",">=","-=","+=","!=","&&","||"};

// strings in this vector will be allowed tri operators
vector<string>ValidTriOperators{"===","!==","..."};

// if true it will tokenize "/*no relative data*/" as one object like a string
int EnableStarDashCancel = 1;

// if true it will automaticly close "/**/" objects
int EnableAutoCLoseStarDashCancel = 0;

// if true it will enable a dash cancell like this one
int EnableDoubleDashCancel = 1;

//if true it will allow a hash cancel "# like pythons"
int EnableHashCancel = 1;

//if true it will allow join numbers that have a . in them
int EnableDoubleNumbers = 1;

//if true it will throw error with bad number statements
int EnableInvalidDoubleNumbers = 1;

// if true it will allow brackets
int AllowBrackets = 1;

// if true it will enable strings
int enableStrings = 1;

//if true it will enable tripple strings
int enableTrippleStrings = 1;

//if true it will allow lines in strings
int allowLinesInStrings = 1;

//if true it will allow lines in tripple strings
int allowLinesInTrippleStrings = 1;

//if true it will remove lines where not needed, like before and after operators
int enableAutoLineRemoval = 1;

//if true it will remove all lines
int NoLines = 0;

//if true it will remove all spaces
int NoSpaces = 1;

//if true it will auto segmentize brackets, and make errors if there are bad brackets
int segmentizeBrackets = 1;
