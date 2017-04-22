# ERPLAG-Compiler
Compiler for the toy language ERPLAG as a part of Compiler Construction course at BITS Pilani

Modules
1. Lexer.c - Token Generation
2. Parser.c - Top Down parsing usign the Grammar.txt
3. TypeChecker.c - Contains AST construction, DataType checking and Semantic Analysis
4. SymbolTable.c - Hashed IDs and ModuleName
5. Codegen.c - NASM assembly code generation for X86 processors
6. Driver.c - Integration of all the modules

Compilation and Executon
1. make
2. ./compiler testcases/c1.txt code.asm       i.e. 'executable-name' 'testcase-name' 'output-file'

Team Members
1. Shivam Gupta (g66shivam)
2. Pulkit Gupta (pulkitsja)
