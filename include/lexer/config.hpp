vector<string>BooleanWords{"true","false","null","undefined"};

vector<string>ConditionalWords{"for","if","try","while""class","is","operator","function"};

vector<string>ConditionalWordsWithNoLineAtStart{"else","catch",};

vector<char>AllowedSymbolsInId{'$','_'};

vector<string>ValidOperators{"+","-","*","/","=","?","<",">","!",":","%","&","."};

vector<string>ValidDualOperators{"++","--","==","<=",">=","-=","+=","!=","&&","||"};

vector<string>ValidTriOperators{"===","!==","..."};

int EnableStarDashCancel = 1;

int EnableAutoCLoseStarDashCancel = 0;

int EnableDoubleDashCancel = 1;

int EnableDoubleNumbers = 1;

int EnableInvalidDoubleNumbers = 1;

int EnableHashCancel = 1;

int AllowBrackets = 1;

int enableStrings = 1;

int enableTrippleStrings = 1;

int allowLinesInStrings = 1;

int allowLinesInTrippleStrings = 1;

int enableAutoLineRemoval = 1;

int NoLines = 0;

int NoSpaces = 1;

int segmentizeBrackets = 1;
