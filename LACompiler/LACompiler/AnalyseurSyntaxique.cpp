#include "stdafx.h"
#include "AnalyseurSyntaxique.h"


AnalyseurSyntaxique::AnalyseurSyntaxique(string fichier)
{
	analyseurLexical = new AnalyseurLexical(fichier);
	motCourant = analyseurLexical->uniteSuivante();
}


AnalyseurSyntaxique::~AnalyseurSyntaxique()
{
}


bool AnalyseurSyntaxique::motCourantEgalAuMotCle(string chaineLexicale)
{
	//comparaison de l'ul et de l'attribut
	return motCourant.UL == MOTCLE && motCourant.attribut == AnalyseurLexical::tableMotCle.find(enMiniscule(chaineLexicale))->second;
}

bool AnalyseurSyntaxique::programme()
{
	if (motCourantEgalAuMotCle("debut")) {
		motCourant = analyseurLexical->uniteSuivante();
		
		return listeDeDeclaration() && listeDIstruction() && motCourantEgalAuMotCle("fin");

	}

	return false;
}

bool AnalyseurSyntaxique::listeDeDeclaration()
{
	return false;
}

bool AnalyseurSyntaxique::declaration()
{
	return false;
}

bool AnalyseurSyntaxique::declarationPrime()
{
	return false;
}

bool AnalyseurSyntaxique::listeDIstruction()
{
	return false;
}

bool AnalyseurSyntaxique::instruction()
{
	return false;
}

bool AnalyseurSyntaxique::instructionPrime()
{
	return false;
}

bool AnalyseurSyntaxique::instructionSeconde()
{
	return false;
}

bool AnalyseurSyntaxique::sinon()
{
	return false;
}

bool AnalyseurSyntaxique::cases()
{
	return false;
}

bool AnalyseurSyntaxique::expression()
{
	return false;
}

bool AnalyseurSyntaxique::expressionPrime()
{
	return false;
}

bool AnalyseurSyntaxique::expressionSimple()
{
	return false;
}

bool AnalyseurSyntaxique::expressionSimplePrime()
{
	return false;
}

bool AnalyseurSyntaxique::terme()
{
	return false;
}

bool AnalyseurSyntaxique::termePrime()
{
	return false;
}

bool AnalyseurSyntaxique::facteur()
{
	return false;
}

bool AnalyseurSyntaxique::facteurPrime()
{
	return false;
}

bool AnalyseurSyntaxique::facteurSeconde()
{
	return false;
}

bool AnalyseurSyntaxique::listeDArgument()
{
	return false;
}

bool AnalyseurSyntaxique::listeDArgumentPrime()
{
	return false;
}

bool AnalyseurSyntaxique::comparaison()
{
	return false;
}

bool AnalyseurSyntaxique::comparaisonPrime()
{
	return false;
}

bool AnalyseurSyntaxique::identificateur()
{
	return false;
}

bool AnalyseurSyntaxique::nbEntier()
{
	return false;
}
