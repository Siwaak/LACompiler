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
	void verifierSyntaxe();

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
	bool comparaisonPrime();
	bool identificateur();
	bool nbEntier();

	/*Suivants*/
	bool suivantListeDeDeclaration(TLexeme);
	bool suivantDeclarationPrime(TLexeme);
	bool suivantListeDIstruction(TLexeme);
	bool suivantSinon(TLexeme);
	bool suivantCases(TLexeme);
	bool suivantExpressionPrime(TLexeme);
	bool suivantExpressionSimplePrime(TLexeme);
	bool suivantTermePrime(TLexeme);
	bool suivantFacteurPrime(TLexeme);
	bool suivantFacteurSeconde(TLexeme);
	bool suivantListeDArgumentPrime(TLexeme);
	bool suivantComparaisonPrime(TLexeme);


	/*Les premiers*/
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
	bool premierComparaisonPrime();
	bool premierIdentificateur();
	bool premierNbEntier();

	
};

