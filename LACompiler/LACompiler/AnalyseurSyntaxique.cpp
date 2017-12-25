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

void AnalyseurSyntaxique::verifierSyntaxe()
{
	if (programme())
		cout << "true " << endl;
	else cout << "false" << endl;
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
	//<liste de declarations> –> <declaration> ; <liste de declarations>
	if (declaration()) {
		if (motCourant.UL == POINTVIR) {
			motCourant = analyseurLexical->uniteSuivante();
			if (listeDeDeclaration())
				return true;
			//Il y a une erreur;
			return false;
		}
	}
	else if (suivantListeDeDeclaration(motCourant))
		//<liste de declarations> –>e
		return true;

	return false;
	
}

bool AnalyseurSyntaxique::declaration()
{
	//<declaration> –> entier <identificateur>
	if (motCourantEgalAuMotCle("entier")) {
		motCourant = analyseurLexical->uniteSuivante();
		if (identificateur())
			return true;

	}
	//<declaration> –>tableau entier <identificateur>[<nb entier>] <declaration prime>
	else if (motCourantEgalAuMotCle("tableau")) {
		motCourant = analyseurLexical->uniteSuivante();

		if (motCourantEgalAuMotCle("entier")) {
			motCourant = analyseurLexical->uniteSuivante();

			if (identificateur()) {
				if (motCourant.UL == CROCHETOUV) {
					motCourant = analyseurLexical->uniteSuivante();

					if (nbEntier() && motCourant.UL == CROCHETFER) {
						motCourant = analyseurLexical->uniteSuivante();

						return declarationPrime();
					}
				}
			}
				

		}
	}
	//Il y a une erreur
	return  false;
}

bool AnalyseurSyntaxique::declarationPrime()
{
	//<declaration prime> –> [ <nb entier> ]
	if (motCourant.UL == CROCHETOUV) {
		motCourant = analyseurLexical->uniteSuivante();
		if (nbEntier() && motCourant.UL == CROCHETFER) {
			motCourant = analyseurLexical->uniteSuivante();

			return true;
		}
	}
	else if (suivantDeclarationPrime(motCourant))
		//<declaration prime> –>e
		return true;

	return false;
}

bool AnalyseurSyntaxique::listeDIstruction()
{
	//<liste d’instructions> –> <instruction>; <liste d’instructions>
	if (instruction()) {
		if (motCourant.UL == POINTVIR) {
			motCourant = analyseurLexical->uniteSuivante();
			if (listeDIstruction())
				return true;
			//Il y a une erreur;
			return false;
		}
	}
	else if (suivantListeDIstruction(motCourant))
		//<liste d’instructions>  –>e
		return true;
	return false;
}

bool AnalyseurSyntaxique::instruction()
{
	//<instruction> –> debut <liste d’instructions> fin
	if (motCourantEgalAuMotCle("debut")) {
		motCourant = analyseurLexical->uniteSuivante();

		return listeDIstruction() && motCourantEgalAuMotCle("fin");
	}
	//<instruction> –> arret
	else if(motCourantEgalAuMotCle("arret"))	{
		motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	//<instruction> –>si <expression> alors <instruction> <sinon>
	else if (motCourantEgalAuMotCle("si")) {
		motCourant = analyseurLexical->uniteSuivante();
		if (expression() && motCourantEgalAuMotCle("alors")) {
			motCourant = analyseurLexical->uniteSuivante();
			
			return instruction() && sinon();
		}
	}
	//<instruction> –>repeter <instruction> jusque <expression>
	else if (motCourantEgalAuMotCle("repeter")) {
		motCourant = analyseurLexical->uniteSuivante();
		if (instruction() && motCourantEgalAuMotCle("jusque")) {
			motCourant = analyseurLexical->uniteSuivante();

			return expression();
		}
	}
	//<instruction> –>tantque <expression> faire <instruction>
	else if (motCourantEgalAuMotCle("tantque")) 
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (expression() && motCourantEgalAuMotCle("faire")) 
		{
			motCourant = analyseurLexical->uniteSuivante();

			return instruction();
		}
	}
	//<instruction> –>pour <identificateur> allantde <nb entier> a <nb entier> faire <instruction>
	else if (motCourantEgalAuMotCle("pour")) 
	{
		motCourant = analyseurLexical->uniteSuivante();

		if (identificateur() && motCourantEgalAuMotCle("allantde"))
		{
			motCourant = analyseurLexical->uniteSuivante();

			if (nbEntier() && motCourantEgalAuMotCle("a"))
			{
				motCourant = analyseurLexical->uniteSuivante();

				if (nbEntier() && motCourantEgalAuMotCle("faire"))
				{
					motCourant = analyseurLexical->uniteSuivante();
					
					return instruction();
				}
			}				
		}
	}
	//<instruction> –>switch <identificateur> faire <cases>
	else if (motCourantEgalAuMotCle("switch"))
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (identificateur() && motCourantEgalAuMotCle("faire")) 
		{
			motCourant = analyseurLexical->uniteSuivante();

			return cases();
		}
	}
	//<instruction> -> ecrire <liste d’arguments>
	else if (motCourantEgalAuMotCle("ecrire")) 
	{
		motCourant = analyseurLexical->uniteSuivante();

			return listeDArgument();		
	}
	//<instruction> -> lire <identificateur>
	else if (motCourantEgalAuMotCle("lire"))
	{
		motCourant = analyseurLexical->uniteSuivante();

		return identificateur();
	}
	//<instruction> -> <expression>
	else if (expression()) 
	{

		return true;
	}
	//<instruction> -> <identificateur> <instruction prime>
	else if (identificateur())
	{

		return instructionPrime();
	}

	return false;
}

bool AnalyseurSyntaxique::instructionPrime()
{
	if (motCourant.UL == AFFEC)
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (expression())
			return true;
		//Il y a une erreur;
		return false;
	}
	else if (motCourant.UL == CROCHETOUV)
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple())
		{
			if (motCourant.UL == CROCHETFER)
			{
				motCourant = analyseurLexical->uniteSuivante();
				if (instructionSeconde())
				{
					return true;
				}
				return false;
			}
		}
	}
	return false;
}

bool AnalyseurSyntaxique::instructionSeconde()
{
	if (motCourant.UL == AFFEC)
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (expression())
		{
			return true;
		}
		//Il y a une erreur;
		return false;
	}
	else if (motCourant.UL == CROCHETOUV)
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple())
		{
			if (motCourant.UL == CROCHETFER)
			{
				motCourant = analyseurLexical->uniteSuivante();
				if (motCourant.UL == AFFEC)
				{
					motCourant = analyseurLexical->uniteSuivante();
					if (expression())
					{
						return true;
					}
					return false;
				}
			}
		}
	}
	return false;
}

bool AnalyseurSyntaxique::sinon()
{
	if (motCourantEgalAuMotCle("sinon"))
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (instruction())
		{
			return true;
		}
		return false;
	}
	else if (suivantSinon(motCourant))
		/**EPSILON*/
		return true;
		return false;
}

bool AnalyseurSyntaxique::cases()
{
	if (motCourantEgalAuMotCle("cas"))
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (nbEntier())
		{
			if (motCourant.UL == DXPOINT)
			{
				motCourant = analyseurLexical->uniteSuivante();
				if (instruction())
				{
					if (motCourantEgalAuMotCle("arret"))
					{
						motCourant = analyseurLexical->uniteSuivante();
						if (cases())
						{
							return true;
						}
						return false;
					}
					return false;
				}
			}
		}
	}
	else if (suivantCases(motCourant))
		/**EPSILON*/
		return true;

	return false;
}

bool AnalyseurSyntaxique::expression()
{
	if (expressionSimple())
	{
		if (expressionPrime())
		{
			return true;
		}
		return false;
	}
	return false;
}

bool AnalyseurSyntaxique::expressionPrime()
{
	if (comparaison())
	{
		if (expressionSimple())
		{
			return true;
		}
		return false;
	}
	else if (suivantExpressionPrime(motCourant))
		/**EPSILON*/
		return	true;

	return false;
}

bool AnalyseurSyntaxique::expressionSimple()
{
	if (terme())
	{
		if (expressionSimplePrime())
		{
			return true;
		}
		return false;
	}
	else if (motCourant.UL == MOINS)
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (terme())
		{
			if (expressionSimplePrime())
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

bool AnalyseurSyntaxique::expressionSimplePrime()
{
	//<expression simple prime> –> +<terme> <expression simple prime>
	if (motCourant.UL == PLUS) {
		motCourant = analyseurLexical->uniteSuivante();
		return terme() && expressionSimplePrime();
	}
	//<expression simple prime> –>- <terme> <expression simple prime>
	else if (motCourant.UL == MOINS) {
		motCourant = analyseurLexical->uniteSuivante();
		return terme() && expressionSimplePrime();
	}
	//<expression simple prime> –>|| <terme> <expression simple prime>
	else if (motCourant.UL == OU) {
		motCourant = analyseurLexical->uniteSuivante();
		return terme() && expressionSimplePrime();
	}

	else if (suivantExpressionSimplePrime(motCourant))

		//<expression simple prime> –>e
		return true;
	return false;
}

bool AnalyseurSyntaxique::terme()
{
	//<terme> –> <facteur> <terme prime>
	return facteur() && termePrime();
}

bool AnalyseurSyntaxique::termePrime()
{
	//<terme prime> –> * <facteur> <terme prime>
	if (motCourant.UL == MULTIP) {
		motCourant = analyseurLexical->uniteSuivante();
		return facteur() && termePrime();
	}
	//<terme prime> –>/ <facteur> <terme prime>
	else if (motCourant.UL == DIVISION) {
		motCourant = analyseurLexical->uniteSuivante();
		return facteur() && termePrime();
	}
	//<terme prime> –>% <facteur> <terme prime>
	else if (motCourant.UL == MODULO) {
		motCourant = analyseurLexical->uniteSuivante();
		return facteur() && termePrime();

	}
	//<terme prime> –>&& <terme prime>
	else if (motCourant.UL == ET) {
		motCourant = analyseurLexical->uniteSuivante();
		return termePrime();
	}
	//<terme prime> –>e
	else if (suivantTermePrime(motCourant))
		return true;
	return false;
}

bool AnalyseurSyntaxique::facteur()
{
	//<facteur> –> <nb entier>
	if (nbEntier())
		return true;
	//<facteur> –> ( <expression simple> )
	else if (motCourant.UL == PAROUV) {
		motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple()) {
			if (motCourant.UL == PARFER) {
				motCourant = analyseurLexical->uniteSuivante();
				return true;
			}
			return false;
		}
	}
	//<facteur> –> ! <facteur>
	else if (motCourant.UL == DIFF) {
		motCourant = analyseurLexical->uniteSuivante();
		return facteur();
	}
	//<facteur> –><identificateur> <facteur prime>
	else if (identificateur()) {
		return facteurPrime();
	}
	return false;
}

bool AnalyseurSyntaxique::facteurPrime()
{
	//<facteur prime> –> [ <expression simple> ] <facteur seconde>
	if (motCourant.UL == CROCHETOUV) {
		motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple()) {
			if (motCourant.UL == CROCHETFER) {
				motCourant = analyseurLexical->uniteSuivante();
				return facteurSeconde();
			}

		}
	}
	//<facteur prime> –>e
	else if (suivantFacteurPrime(motCourant))
		return true;
	return false;
}

bool AnalyseurSyntaxique::facteurSeconde()
{
	if (motCourant.UL == CROCHETOUV)
	{
		motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple() && motCourant.UL == CROCHETFER)
		{
			motCourant = analyseurLexical->uniteSuivante();

			return true;
		}
	}
	else


		return false;
}

bool AnalyseurSyntaxique::listeDArgument()
{
	return expression() && listeDArgumentPrime();
}

bool AnalyseurSyntaxique::listeDArgumentPrime()
{
	if (motCourant.UL == VIR) {
		return listeDArgument();
	}

	else

		return false;
}

bool AnalyseurSyntaxique::comparaison()
{
	if (motCourant.UL == EGAL)
	{
		return motCourant.UL == EGAL;
	}

	else if (motCourant.UL == DIFF)
	{
		return true;
	}
	else if (motCourant.UL == INF)
	{
		return comparaisonPrime();
	}
	else if (motCourant.UL == SUP)
	{
		return comparaisonPrime();
	}

	return false;
}

bool AnalyseurSyntaxique::comparaisonPrime()
{
	if (motCourant.UL == EGAL)
		return true;
	else
		return false;
}

bool AnalyseurSyntaxique::identificateur()
{
	return motCourant.UL == IDENT;
}

bool AnalyseurSyntaxique::nbEntier()
{
	return motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::suivantListeDeDeclaration(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantDeclarationPrime(TLexeme lex)
{ 
	return lex.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantListeDIstruction(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantSinon(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantCases(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantExpressionPrime(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantExpressionSimplePrime(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantTermePrime(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantFacteurPrime(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantFacteurSeconde(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantListeDArgumentPrime(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::suivantComparaisonPrime(TLexeme)
{
	return false;
}

bool AnalyseurSyntaxique::premierProgramme()
{
	return false;
}

bool AnalyseurSyntaxique::premierListeDeDeclaration()
{
	return false;
}

bool AnalyseurSyntaxique::premierDeclaration()
{
	return false;
}

bool AnalyseurSyntaxique::premierDeclarationPrime()
{
	return false;
}

bool AnalyseurSyntaxique::premierListeDIstruction()
{
	return false;
}

bool AnalyseurSyntaxique::premierInstruction()
{
	return false;
}

bool AnalyseurSyntaxique::premierInstructionPrime()
{
	return false;
}

bool AnalyseurSyntaxique::premierInstructionSeconde()
{
	return false;
}

bool AnalyseurSyntaxique::premierSinon()
{
	return false;
}

bool AnalyseurSyntaxique::premierCases()
{
	return false;
}

bool AnalyseurSyntaxique::premierExpression()
{
	return false;
}

bool AnalyseurSyntaxique::premierExpressionPrime()
{
	return false;
}

bool AnalyseurSyntaxique::premierExpressionSimple()
{
	return false;
}

bool AnalyseurSyntaxique::premierExpressionSimplePrime()
{
	return false;
}

bool AnalyseurSyntaxique::premierTerme()
{
	return false;
}

bool AnalyseurSyntaxique::premierTermePrime()
{
	return false;
}

bool AnalyseurSyntaxique::premierFacteur()
{
	return false;
}

bool AnalyseurSyntaxique::premierFacteurPrime()
{
	return false;
}

bool AnalyseurSyntaxique::premierFacteurSeconde()
{
	return false;
}

bool AnalyseurSyntaxique::premierListeDArgument()
{
	return false;
}

bool AnalyseurSyntaxique::premierListeDArgumentPrime()
{
	return false;
}

bool AnalyseurSyntaxique::premierComparaison()
{
	return false;
}

bool AnalyseurSyntaxique::premierComparaisonPrime()
{
	return false;
}

bool AnalyseurSyntaxique::premierIdentificateur()
{
	return false;
}

bool AnalyseurSyntaxique::premierNbEntier()
{
	return false;
}
