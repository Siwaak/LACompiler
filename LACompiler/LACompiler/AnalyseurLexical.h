#pragma once

#include <iostream>;
#include <vector>;
#include <string>;
#include <map>;
#include "Structures.h";

using namespace std;



class AnalyseurLexical
{
	string code; //contient le script à traduire
	string lexeme;//stockera la valeur s'il s'agit d'un identificateur, d'un mot clé ou d'une constante
	char c;//Caractere lu
	map<int, string> tableIdent; //liste des identificateurs;

	const map<string, int> tableMotCle = {
		{	"debut",1 },
		{	"fin",2 },
		{ "entier",3 },
		{ "tableau",4},
		{  "arret",5 },
		{ "si",6},
		{ "sinon",7},
		{ "alors",8},
		{ "tantque",9},
		{"faire",10},
		{ "ecrire",11},
		{ "lire",12}
	}; //liste des motsCle

public:
	AnalyseurLexical(string);
	~AnalyseurLexical();

	long hashCode(string); //calcul un entier (index dans un talbeau) à partire d'une chaine

	bool estBlanc(char);//Vérifie si un caractère est blanc

	void lireCaractere();//suprime le premier caractère de la variable code et le met dans c

	TLexeme uniteSuivante();// récupère le lexeme suivant

	long indexIdentifiant(string);	//Détermine l'index d'un identifiant dans le tableau. l'insert s'il n'existe pas

	bool estMotCle(string mot);// verifie si un mot fait partie des mots clé

	bool estLettre(char); 

	bool estChiffre(char);

	bool estCommentaire(char);

	bool codeEstFini();

	void afficherTableMotsCle();

	void afficherTableIdentificateurs();
};

