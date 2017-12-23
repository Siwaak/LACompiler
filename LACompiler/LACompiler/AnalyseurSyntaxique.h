#pragma once
#include <string>
#include <iostream>
#include "AnalyseurLexical.h";

using namespace std;

class AnalyseurSyntaxique
{

	//const string NON_TERMINAUX[][3] = {
	//	{"programme","debut",""}
	//								};//Matrice contenant pour chaque non terminal, son premier et son deuxième.
	AnalyseurLexical *analyseurLexical;
	TLexeme motCourant;
public:
	AnalyseurSyntaxique(string);
	virtual ~AnalyseurSyntaxique();

	/*Verifie si le motCourant correspond au mot cle passé en arguement */
	bool motCourantEgalAuMotCle(string);
	
	bool programme();
	bool listeDeDeclaration();
	bool declaration();
	bool declarationPrime();
	bool listeDIstruction();
	bool instruction();
	bool instructionPrime();
	bool instructionSeconde();
	bool sinon();
	bool cases();
	bool expression();
	bool expressionPrime();
	bool expressionSimple();
	bool expressionSimplePrime();
	bool terme();
	bool termePrime();
	bool facteur();
	bool facteurPrime();
	bool facteurSeconde();
	bool listeDArgument();
	bool listeDArgumentPrime();
	bool comparaison();
	bool comparaisonPrime();
	bool identificateur();
	bool nbEntier();
};

