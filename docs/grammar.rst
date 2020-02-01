Terminals
=========

identifier ::= "[_A-Za-z]([A-Za-z0-9]*)"
integer    ::= "[0-9]+"

Grammar
======

Program        ::= Function*
Function       ::= FunctionHeader, FunctionBody
FunctionHeader ::= '(' 'function' ArgList ')'
ArgList        ::= '(' Arg* ')'
Arg            ::= '(' identifier integer ')'
FunctionBody   ::= '(' Statement* ')'
Statement      ::= '(' '+' identifier identifier Atom ')'
Atom           ::= integer
Atom           ::= identifier
