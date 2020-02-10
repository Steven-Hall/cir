Terminals
=========

identifier ::= "[_A-Za-z]([A-Za-z0-9]*)"
integer    ::= "[0-9]+"

Grammar
======

TODO: need to add argument handling

Program        ::= Function*
Function       ::= FunctionHeader, FunctionBody
FunctionHeader ::= '(' 'function' identifier ArgList ')'
ArgList        ::= '(' ')'
FunctionBody   ::= '(' Statement* ')'
Statement      ::= Expression | IfStatement | LabelStatement | JumpStatement
Expression     ::= '(' operator identifier identifier Atom ')'
IfStatement    ::= '(' identifier '(' Statement* ')' '(' Statement* ')' ')'
LabelStatement ::= '(' 'label' identifier '(' Statement* ')' ')'
JumpStatement  ::= '(' 'jump' identifier ')'
Atom           ::= integer | identifier
