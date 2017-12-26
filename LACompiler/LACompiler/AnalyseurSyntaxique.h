#pragma once
#include <string>
#include <iostream>
#include "AnalyseurLexical.h"
#include "ErreurSyntaxique.h";

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
	void verifierSyntaxe();
	void prochainMot();
	//Les erreurs
	ErreurSyntaxique lesErreurs;
	/**Non terminaux*/
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
	bool identificateur();
	bool nbEntier();

	/*Suivants*/
	bool suivantListeDeDeclaration();
	bool suivantDeclarationPrime();
	bool suivantListeDInstruction();
	bool suivantSinon();
	bool suivantCases();
	bool suivantExpressionPrime();
	bool suivantExpressionSimplePrime();
	bool suivantTermePrime();
	bool suivantFacteurPrime();
	bool suivantFacteurSeconde();
	bool suivantListeDArgumentPrime();
	
	/*Premiers*/
	bool premierProgramme();
	bool premierListeDeDeclaration();
	bool premierDeclaration();
	bool premierDeclarationPrime();
	bool premierListeDIstruction();
	bool premierInstruction();
	bool premierInstructionPrime();
	bool premierInstructionSeconde();
	bool premierSinon();
	bool premierCases();
	bool premierExpression();
	bool premierExpressionPrime();
	bool premierExpressionSimple();
	bool premierExpressionSimplePrime();
	bool premierTerme();
	bool premierTermePrime();
	bool premierFacteur();
	bool premierFacteurPrime();
	bool premierFacteurSeconde();
	bool premierListeDArgument();
	bool premierListeDArgumentPrime();
	bool premierComparaison();
	bool premierIdentificateur();
	bool premierNbEntier();

};

