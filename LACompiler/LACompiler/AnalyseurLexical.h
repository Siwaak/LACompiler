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
	AnalyseurLexical(string);
	~AnalyseurLexical();

	bool estBlanc(char);
	char lireCaractere();
	TLexeme uniteSuivante();
	int indexIdentifiant(string);
	int estMotCle(string);
	bool estLettre(char);
	bool estChiffre(char);
	bool estCommentaire(char);
};

