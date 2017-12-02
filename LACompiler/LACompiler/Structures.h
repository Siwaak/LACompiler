#pragma once
typedef struct TLexeme {
	UniteLexical UL;
	int attribut;
}TLexeme;

typedef enum {
	MOTCLE, IDENT, VIR, POINTVIR, AFFEC, EGAL, DIFF, INF, INFEG, SUP, SUPEG, PLUS, MOINS,
	MULTIP, DIVISION, MODULO, NEGATION, ET, OU, ACCOLOUV, ACCOFER, PAROUV, PARFER,
	CROCHETOUV, CROCHETFER, NBRENTIER, ERR1, ERR2, ERR3
}UniteLexical;// Types des unités lexicales
			  // ERR1 : & attendu, ERR2: | attendu, ERR3: caractere inattendu