#pragma once

#include <iostream>;
#include <vector>;
#include <string>;
#include <map>;
#include "Structures.h";

using namespace std;



class AnalyseurLexical
{
	string code;
	string lexeme;//stockera la valeur s'il s'agit d'un identificateur, d'un mot clé ou d'une constante
	char c;//Caractere lu
	map<int, string> tableIdent; //liste des identificateurs;
	//map<int, string> tableMotCle;
	const map<unsigned long, string> tableMotCle = {
		{hashCode("debut"),"debut" },
		{hashCode("fin"),"fin" },
		{ hashCode("entier"), "entier" },
		{ hashCode("tableau"), "tableau"},
		{ hashCode("arret"), "arret" },
		{ hashCode("si"), "si"},
		{ hashCode("sinon"), "sinon"},
		{ hashCode("alors"), "alors"},
		{ hashCode("tantque"), "tantque"},
		{ hashCode("faire"), "faire"},
		{ hashCode("ecrire"), "ecrire"},
		{ hashCode("lire"), "lire"}
	}; //liste des motsCle

public:
	AnalyseurLexical(string);
	~AnalyseurLexical();

	 long hashCode(string chaine);

	bool estBlanc(char);
	char lireCaractere();
	TLexeme uniteSuivante();
	 long indexIdentifiant(string);
	 
	 bool estMotCle(string mot);
	 bool estLettre(char);
	bool estChiffre(char);
	bool estCommentaire(char);
	bool codeEstFini();
	void afficherTableMotsCle();
	void afficherTableIdentificateurs();
};

