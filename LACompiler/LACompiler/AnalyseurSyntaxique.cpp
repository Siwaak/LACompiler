#include "stdafx.h"
#include "AnalyseurSyntaxique.h"


AnalyseurSyntaxique::AnalyseurSyntaxique(string fichier)
{
	analyseurLexical = new AnalyseurLexical(fichier);
	prochainMot();//motCourant = analyseurLexical->uniteSuivante();
}

AnalyseurSyntaxique::~AnalyseurSyntaxique()
{

}


bool AnalyseurSyntaxique::motCourantEgalAuMotCle(string chaineLexicale)
{
	//comparaison de l'ul et de l'attribut
	return motCourant.UL == MOTCLE && motCourant.attribut == AnalyseurLexical::tableMotCle[chaineLexicale];
}

void AnalyseurSyntaxique::verifierSyntaxe()
{
	if (programme())
		cout << "true " << endl;
	else cout << "false" << endl;
}

void AnalyseurSyntaxique::prochainMot()
{
	cout << motCourant.UL << endl;
	motCourant = analyseurLexical->uniteSuivante();
}

bool AnalyseurSyntaxique::programme()
{
	if (motCourantEgalAuMotCle("debut"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (listeDeDeclaration())
		{
			if (listeDIstruction())
			{
				if (motCourantEgalAuMotCle("fin"))
				{
					prochainMot();//motCourant = analyseurLexical->uniteSuivante();
					return true;
				}
			}
		}
	}

	return false;
}

bool AnalyseurSyntaxique::listeDeDeclaration()
{
	//<liste de declarations> –> <declaration> ; <liste de declarations>
	if (declaration())
	{
		if (motCourant.UL == POINTVIR)
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();
			if (listeDeDeclaration())
				return true;
		}
	}
	else if (suivantListeDeDeclaration())
		//<liste de declarations> –>e
		return true;

	return false;

}

bool AnalyseurSyntaxique::declaration()
{
	//<declaration> –> entier <identificateur>
	if (motCourantEgalAuMotCle("entier"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (identificateur())
			return true;

	}
	//<declaration> –>tableau entier <identificateur>[<nb entier>] <declaration prime>
	else if (motCourantEgalAuMotCle("tableau"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();

		if (motCourantEgalAuMotCle("entier"))
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			if (identificateur()) {
				if (motCourant.UL == CROCHETOUV)
				{
					prochainMot();//motCourant = analyseurLexical->uniteSuivante();

					if (nbEntier() && motCourant.UL == CROCHETFER)
					{
						prochainMot();//motCourant = analyseurLexical->uniteSuivante();

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
	if (motCourant.UL == CROCHETOUV)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (nbEntier() && motCourant.UL == CROCHETFER)
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			return true;
		}
	}
	else if (suivantDeclarationPrime())
		//<declaration prime> –>e
		return true;

	return false;
}

bool AnalyseurSyntaxique::listeDIstruction()
{
	//<liste d’instructions> –> <instruction>; <liste d’instructions>
	if (instruction()) {
		if (motCourant.UL == POINTVIR)
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();
			if (listeDIstruction())
				return true;
		}
	}
	else if (suivantListeDInstruction())
		//<liste d’instructions>  –>e
		return true;
	return false;
}

bool AnalyseurSyntaxique::instruction()
{
	//<instruction> –> debut <liste d’instructions> fin
	if (motCourantEgalAuMotCle("debut"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();

		if (listeDIstruction())
		{
			if (motCourantEgalAuMotCle("fin"))
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				return true;
			}
		}
	}
	//<instruction> –> arret
	else if (motCourantEgalAuMotCle("arret"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	//<instruction> –>si <expression> alors <instruction> <sinon>
	else if (motCourantEgalAuMotCle("si"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expression() && motCourantEgalAuMotCle("alors"))
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			return instruction() && sinon();
		}
	}
	//<instruction> –>repeter <instruction> jusque <expression>
	else if (motCourantEgalAuMotCle("repeter"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (instruction() && motCourantEgalAuMotCle("jusque"))
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			return expression();
		}
	}
	//<instruction> –>tantque <expression> faire <instruction>
	else if (motCourantEgalAuMotCle("tantque"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expression() && motCourantEgalAuMotCle("faire"))
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			return instruction();
		}
	}
	//<instruction> –>pour <identificateur> allantde <nb entier> a <nb entier> faire <instruction>
	else if (motCourantEgalAuMotCle("pour"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();

		if (identificateur() && motCourantEgalAuMotCle("allantde"))
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			if (nbEntier() && motCourantEgalAuMotCle("a"))
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();

				if (nbEntier() && motCourantEgalAuMotCle("faire"))
				{
					prochainMot();//motCourant = analyseurLexical->uniteSuivante();

					return instruction();
				}
			}
		}
	}
	//<instruction> –>switch <identificateur> faire <cases>
	else if (motCourantEgalAuMotCle("switch"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (identificateur() && motCourantEgalAuMotCle("faire"))
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			return cases();
		}
	}
	//<instruction> -> ecrire <liste d’arguments>
	else if (motCourantEgalAuMotCle("ecrire"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();

		return listeDArgument();
	}
	//<instruction> -> lire <identificateur>
	else if (motCourantEgalAuMotCle("lire"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();

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
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expression())
			return true;
		//Il y a une erreur;
		return false;
	}
	else if (motCourant.UL == CROCHETOUV)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple())
		{
			if (motCourant.UL == CROCHETFER)
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
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
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expression())
		{
			return true;
		}
		//Il y a une erreur;
		return false;
	}
	else if (motCourant.UL == CROCHETOUV)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple())
		{
			if (motCourant.UL == CROCHETFER)
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				if (motCourant.UL == AFFEC)
				{
					prochainMot();//motCourant = analyseurLexical->uniteSuivante();
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
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (instruction())
		{
			return true;
		}
		return false;
	}
	else if (suivantSinon())
		/**EPSILON*/
		return true;
	return false;
}

bool AnalyseurSyntaxique::cases()
{
	if (motCourantEgalAuMotCle("cas"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (nbEntier())
		{
			if (motCourant.UL == DXPOINT)
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				if (instruction())
				{
					if (motCourantEgalAuMotCle("arret"))
					{
						prochainMot();//motCourant = analyseurLexical->uniteSuivante();
						if (cases())
						
							return true;
						
						return false;
					}
				}
			}
		}
	}
	else if (suivantCases())
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
	}
	else if (suivantExpressionPrime())
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
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (terme())
		{
			if (expressionSimplePrime())
			{
				return true;
			}
		}
	}
	return false;
}

bool AnalyseurSyntaxique::expressionSimplePrime()
{
	//<expression simple prime> –> +<terme> <expression simple prime>
	if (motCourant.UL == PLUS) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return terme() && expressionSimplePrime();
	}
	//<expression simple prime> –>- <terme> <expression simple prime>
	else if (motCourant.UL == MOINS) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return terme() && expressionSimplePrime();
	}
	//<expression simple prime> –>|| <terme> <expression simple prime>
	else if (motCourant.UL == OU) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return terme() && expressionSimplePrime();
	}

	else if (suivantExpressionSimplePrime())

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
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return facteur() && termePrime();
	}
	//<terme prime> –>/ <facteur> <terme prime>
	else if (motCourant.UL == DIVISION) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return facteur() && termePrime();
	}
	//<terme prime> –>% <facteur> <terme prime>
	else if (motCourant.UL == MODULO) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return facteur() && termePrime();

	}
	//<terme prime> –>&& <terme prime>
	else if (motCourant.UL == ET) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return termePrime();
	}
	//<terme prime> –>e
	else if (suivantTermePrime())
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
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple()) {
			if (motCourant.UL == PARFER) {
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				return true;
			}
			return false;
		}
	}
	//<facteur> –> ! <facteur>
	else if (motCourant.UL == DIFF) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
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
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple()) {
			if (motCourant.UL == CROCHETFER) {
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				return facteurSeconde();
			}

		}
	}
	//<facteur prime> –>e
	else if (suivantFacteurPrime())
	{
		return true;
	}
	return false;
}

bool AnalyseurSyntaxique::facteurSeconde()
{
	if (motCourant.UL == CROCHETOUV)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple() && motCourant.UL == CROCHETFER)
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			return true;
		}
	}
	else if (suivantFacteurSeconde())
		return true;


	return false;
}

bool AnalyseurSyntaxique::listeDArgument()
{
	return expression() && listeDArgumentPrime();
}

bool AnalyseurSyntaxique::listeDArgumentPrime()
{
	if (motCourant.UL == VIR) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return listeDArgument();
	}

	else if (suivantListeDArgumentPrime())
		return true;

	return false;
}

bool AnalyseurSyntaxique::comparaison()
{
	if (motCourant.UL == AFFEC)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();

		return true;
	}
	else if (motCourant.UL == DIFF)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	else if (motCourant.UL == INF)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	else if (motCourant.UL == SUP)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	else if (motCourant.UL == INFEG)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	else if (motCourant.UL == SUPEG)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	return false;
}

bool AnalyseurSyntaxique::comparaisonPrime()
{
	if (motCourant.UL == AFFEC)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	else if (suivantComparaisonPrime())
		return true;
	return false;
}

bool AnalyseurSyntaxique::identificateur()
{
	if (motCourant.UL == IDENT)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}

	return false;
}

bool AnalyseurSyntaxique::nbEntier()
{
	if (motCourant.UL == NBRENTIER)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}

	return false;
}

bool AnalyseurSyntaxique::suivantListeDeDeclaration()
{
	return motCourantEgalAuMotCle("debut")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("si")
		|| motCourantEgalAuMotCle("repeter")
		|| motCourantEgalAuMotCle("tantque")
		|| motCourantEgalAuMotCle("pour")
		|| motCourantEgalAuMotCle("switch")
		|| motCourantEgalAuMotCle("ecrire")
		|| motCourantEgalAuMotCle("lire")
		|| motCourant.UL == MOINS
		|| motCourant.UL == IDENT
		|| motCourant.UL == DIFF
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER
		|| motCourant.UL == POINTVIR;
}


bool AnalyseurSyntaxique::suivantDeclarationPrime()
{
	return motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantListeDInstruction()
{
	return motCourantEgalAuMotCle("fin");

}

bool AnalyseurSyntaxique::suivantSinon()
{
	return motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantCases()
{
	return motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantExpressionPrime()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}


bool AnalyseurSyntaxique::suivantExpressionSimplePrime()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourant.UL == CROCHETFER
		|| motCourant.UL == AFFEC
		|| motCourant.UL == NEGATION
		|| motCourant.UL == INF
		|| motCourant.UL == SUP

		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}


bool AnalyseurSyntaxique::suivantTermePrime()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourant.UL == PLUS
		|| motCourant.UL == MOINS
		|| motCourant.UL == OU
		|| motCourant.UL == CROCHETFER
		|| motCourant.UL == AFFEC
		|| motCourant.UL == NEGATION
		|| motCourant.UL == INF
		|| motCourant.UL == SUP

		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}



bool AnalyseurSyntaxique::suivantFacteurPrime()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourant.UL == MULTIP
		|| motCourant.UL == DIVISION
		|| motCourant.UL == MODULO
		|| motCourant.UL == ET
		|| motCourant.UL == PLUS
		|| motCourant.UL == MOINS
		|| motCourant.UL == OU
		|| motCourant.UL == CROCHETFER
		|| motCourant.UL == AFFEC
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == NEGATION
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}


bool AnalyseurSyntaxique::suivantFacteurSeconde()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourant.UL == MULTIP
		|| motCourant.UL == DIVISION
		|| motCourant.UL == MODULO
		|| motCourant.UL == ET
		|| motCourant.UL == PLUS
		|| motCourant.UL == MOINS
		|| motCourant.UL == OU
		|| motCourant.UL == CROCHETFER
		|| motCourant.UL == AFFEC
		|| motCourant.UL == NEGATION
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantListeDArgumentPrime()
{
	return motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourant.UL == POINTVIR;
}


bool AnalyseurSyntaxique::suivantComparaisonPrime()
{
	return motCourant.UL == MOINS
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PARFER
		|| motCourant.UL == NBRENTIER
		|| motCourant.UL == IDENT;
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
