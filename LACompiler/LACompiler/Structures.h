#pragma once

#include <iostream>;
#include <map>;
#include <string>;
#include <algorithm>;


using namespace std;

typedef enum {
	MOTCLE, IDENT, VIR, POINTVIR, AFFEC, EGAL, DIFF, INF, INFEG, SUP, SUPEG, PLUS, MOINS,
	MULTIP, DIVISION, MODULO, NEGATION, ET, OU, PAROUV, PARFER,
	CROCHETOUV, CROCHETFER, NBRENTIER, COMMENTAIRE, DXPOINT, ERR1, ERR2, ERR3, ERR4, ERR5
}UniteLexical;// Types des unités lexicales
			  // ERR1 : & attendu, ERR2: | attendu, ERR3: caractere inattendu, ERR4 commentaire non fermé


//Surcharge de << pour afficher la chaine de l'unité lexicale au lieu de son index.
//Cette fonction est inline pour éviter des définitions multiples

inline ostream& operator<<(ostream& out, const UniteLexical ul) {
	string chaineUl;

	switch (ul) {
	case MOTCLE : 
		chaineUl = "MOTCLE";
		break;

	case IDENT:
		chaineUl = "IDENT";
		break;

	case VIR:
		chaineUl = "VIR";
		break;

	case POINTVIR:
		chaineUl = "POINTVIR";
		break;

	case AFFEC:
		chaineUl = "AFFEC";
		break;

	case EGAL:
		chaineUl = "EGAL";
		break;

	case DIFF:
		chaineUl = "DIFF";
		break;

	case INF:
		chaineUl = "INF";
		break;
	case INFEG:
		chaineUl = "INFEG";
		break;

	case SUP:
		chaineUl = "SUP";
		break;

	case SUPEG:
		chaineUl = "SUPEG";
		break;

	case PLUS:
		chaineUl = "PLUS";
		break;
		
	case MOINS:
		chaineUl = "MOINS";
		break;

	case MULTIP:
		chaineUl = "MULTIP";
		break;

	case DIVISION:
		chaineUl = "DIVISION";
		break;
		
	case MODULO:
		chaineUl = "MODULO";
		break;

	case NEGATION:
		chaineUl = "NEGATION";
		break;
		
	case ET:
		chaineUl = "ET";
		break;

	case OU:
		chaineUl = "OU";
		break;

	case PAROUV:
		chaineUl = "PAROUV";
		break;

	case PARFER:
		chaineUl = "PARFER";
		break;

	case CROCHETOUV:
		chaineUl = "CROCHETOUV";
		break;

	case NBRENTIER:
		chaineUl = "NBRENTIER";
		break;
		
	case COMMENTAIRE:
		chaineUl = "COMMENTAIRE";
		break;
	case DXPOINT:
		chaineUl = "DXPOINT";
		break;
		
	case ERR1:
		chaineUl = "ERR1: & attendu";

		break;
	case ERR2:
		chaineUl = "ERR2: | attendu";
		break;
	
	case ERR3:
		chaineUl = "ERR3: Le carractere n'appartient pas a l'alphabet";
		break;

	case ERR4:
		chaineUl = "ERR4: commentaire non ferme";
		break;

	case ERR5:
		chaineUl = "ERR5: Fin de fichier";
		break;
	default :
		break;
	}

	return out << chaineUl;
}


typedef struct TLexeme {
	UniteLexical UL;
	long attribut;
}TLexeme;

//Met un mot en minuscule
inline string enMiniscule(string chaine) {
	
	 transform(chaine.begin(), chaine.end(), chaine.begin(), (int(*)(int))tolower);

	 return chaine;
}