#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

typedef struct TLexeme {
	UniteLexical UL;
	int attribut;
}TLexeme;

class AnalyseurLexical
{
	typedef enum {
		MOTCLE, IDENT, VIR, POINTVIR, AFFEC, EGAL, DIFF, INF, INFEG, SUP, SUPEG, PLUS, MOINS,
		MULTIP, DIVISION, MODULO, NEGATION, ET, OU, ACCOLOUV, ACCOFER, PAROUV, PARFER,
		CROCHETOUV, CROCHETFER, NBRENTIER, ERR1, ERR2, ERR3
	}UniteLexical;
	// ERR1 : & attendu, ERR2: | attendu, ERR3: caractere inattendu


	string lexeme;//stockera la valeur s'il s'agit d'un identificateur, d'un mot clé ou d'une constante
	char c;//Caractere lu
	map<int, string> tableIdent; //liste des identificateurs;
	const map<int, string> tableMotCle = {
		{1,"debut" },
		{2,"fin" },
		{3, "entier" },
		{4, "tableau"},
		{5, "arret" },
		{6, "si"},
		{7, "sinon"},
		{8, "alors"},
		{9, "tantque"},
		{10, "faire"},
		{11, "ecrire"},
		{12, "lire"}
	}; //liste des motsCle
public:
	AnalyseurLexical();
	~AnalyseurLexical();

	bool estBlanc(char);
	char lireCaractere();
	TLexeme uniteSuivante();
	int indexIdentifiant(string);
	int indexMotCle(string);
	bool estMotCle(string);
	bool estLettre(char);
	bool estChiffre(char);
};

