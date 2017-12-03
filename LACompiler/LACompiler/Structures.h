#pragma once

#include <iostream>;
#include <map>;
#include <string>;

using namespace std;

typedef enum {
	MOTCLE, IDENT, VIR, POINTVIR, AFFEC, EGAL, DIFF, INF, INFEG, SUP, SUPEG, PLUS, MOINS,
	MULTIP, DIVISION, MODULO, NEGATION, ET, OU, ACCOLOUV, ACCOFER, PAROUV, PARFER,
	CROCHETOUV, CROCHETFER, NBRENTIER, ERR1, ERR2, ERR3
}UniteLexical;// Types des unités lexicales
			  // ERR1 : & attendu, ERR2: | attendu, ERR3: caractere inattendu


//Surcharge de << pour afficher la chaine de l'unité lexicale au lieu de son index.
//Cette fonction est inline pour éviter des définitions multiples
inline ostream& operator<<(ostream& out, const UniteLexical value) {
	static map<UniteLexical, string> strings;
	if (strings.size() == 0) {
#define INSERT_ELEMENT(p) strings[p] = #p
		INSERT_ELEMENT(MOTCLE);
		INSERT_ELEMENT(IDENT);
		INSERT_ELEMENT(VIR);
		INSERT_ELEMENT(POINTVIR);
		INSERT_ELEMENT(AFFEC);
		INSERT_ELEMENT(EGAL);
		INSERT_ELEMENT(DIFF);
		INSERT_ELEMENT(INF);
		INSERT_ELEMENT(INFEG);
		INSERT_ELEMENT(SUP);
		INSERT_ELEMENT(SUPEG);
		INSERT_ELEMENT(PLUS);
		INSERT_ELEMENT(MOINS);
		INSERT_ELEMENT(MULTIP);
		INSERT_ELEMENT(DIVISION);
		INSERT_ELEMENT(MODULO);
		INSERT_ELEMENT(NEGATION);
		INSERT_ELEMENT(ET);
		INSERT_ELEMENT(OU);
		INSERT_ELEMENT(ACCOLOUV);
		INSERT_ELEMENT(ACCOFER);
		INSERT_ELEMENT(PAROUV);
		INSERT_ELEMENT(PARFER);
		INSERT_ELEMENT(CROCHETOUV);
		INSERT_ELEMENT(CROCHETFER);
		INSERT_ELEMENT(NBRENTIER);
		INSERT_ELEMENT(ERR1);
		INSERT_ELEMENT(ERR2);
		INSERT_ELEMENT(ERR3);
#undef INSERT_ELEMENT
	}

	return out << strings[value];
}


typedef struct TLexeme {
	UniteLexical UL;
	int attribut;
}TLexeme;

