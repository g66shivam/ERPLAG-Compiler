compiler: driver.c parser.c lexer.c codegen.c typeChecker.c symbolTable.c
	gcc -o compiler driver.c parser.c lexer.c codegen.c typeChecker.c symbolTable.c
