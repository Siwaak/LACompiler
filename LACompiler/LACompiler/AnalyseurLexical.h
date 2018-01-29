#pragma once

#include <iostream>;
#include <vector>;
#include <string>;
#include <map>;
#include "Structures.h";

using namespace std;



class AnalyseurLexical
{
	string code; //contient le script � traduire
	string lexeme;//stockera la valeur s'il s'agit d'un identificateur, d'un mot cl� ou d'une constante
	char c;//Caractere lu
	int ligne; //le numero de la ligne dans le script
	

	
public:
	AnalyseurLexical(string);
	~AnalyseurLexical();
	map<long, identificateur> tableIdent; //liste des identificateurs;
	static  map<string, int> tableMotCle;
	int getLigne()
	{
		return ligne;
	}
	string dernierIdent;
	void ajouterIdent(string,string);
	long hashCode(string); //calcul un entier (index dans un talbeau) � partire d'une chaine

	bool estBlanc(char);//V�rifie si un caract�re est blanc

	void lireCaractere();//suprime le premier caract�re de la variable code et le met dans c

	TLexeme uniteSuivante();// r�cup�re le lexeme suivant

	long indexIdentifiant(string);	//D�termine l'index d'un identifiant dans le tableau. l'insert s'il n'existe pas

	bool estMotCle(string mot);// verifie si un mot fait partie des mots cl�

	bool estLettre(char);

	bool estChiffre(char);

	bool estCommentaire(char);

	bool codeEstFini();

	void afficherTableMotsCle();

	void afficherTableIdentificateurs();
};

