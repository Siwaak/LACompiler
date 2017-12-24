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
		cout << "true" << endl;
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
	else 
		 //<liste de declarations> –>e
		//il faut vérifier si le mot courant appartient à suivant de listeDeDeclaration
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
	else
		//<declaration prime> –>e
		//on vérifie si le mot courant appartient au suivant de declarationPrime

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
	else
		//<liste d’instructions>  –>e
		//il faut vérifier si le mot courant appartient à suivant de listeDInstruction
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
	else if (motCourantEgalAuMotCle("tantque")) {
		motCourant = analyseurLexical->uniteSuivante();
		if (expression() && motCourantEgalAuMotCle("faire")) {
			motCourant = analyseurLexical->uniteSuivante();

			return instruction();
		}
	}
	//<instruction> –>pour <identificateur> allantde <nb entier> a <nb entier> faire <instruction>
	else if (motCourantEgalAuMotCle("pour")) {
		motCourant = analyseurLexical->uniteSuivante();

		if (identificateur() && motCourantEgalAuMotCle("allantde")) {
			motCourant = analyseurLexical->uniteSuivante();

			if (nbEntier() && motCourantEgalAuMotCle("a")) {
				motCourant = analyseurLexical->uniteSuivante();

				if (nbEntier() && motCourantEgalAuMotCle("faire")) {
					motCourant = analyseurLexical->uniteSuivante();
					
					return instruction();
				}
			}				
		}
	}
	//<instruction> –>switch <identificateur> faire <cases>
	else if (motCourantEgalAuMotCle("switch")) {
		motCourant = analyseurLexical->uniteSuivante();
		if (identificateur() && motCourantEgalAuMotCle("faire")) {
			motCourant = analyseurLexical->uniteSuivante();

			return cases();
		}
	}
	//<instruction> -> ecrire <liste d’arguments>
	else if (motCourantEgalAuMotCle("ecrire")) {
		motCourant = analyseurLexical->uniteSuivante();

			return listeDArgument();		
	}
	//<instruction> -> lire <identificateur>
	else if (motCourantEgalAuMotCle("lire")) {
		motCourant = analyseurLexical->uniteSuivante();

		return identificateur();
	}
	//<instruction> -> <expression>
	else if (expression()) {

		return true;
	}
	//<instruction> -> <identificateur> <instruction prime>
	else if (identificateur()) {

		return instructionPrime();
	}

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


	//<expression simple prime> –>e
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
