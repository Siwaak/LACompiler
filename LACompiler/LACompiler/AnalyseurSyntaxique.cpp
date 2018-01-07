#include "stdafx.h"
#include "AnalyseurSyntaxique.h"
#include"ErreurSyntaxique.h"
#include <string>
#include <string.h>


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
		cout << "True " << endl;
	else cout << "False" << endl;
}

void AnalyseurSyntaxique::prochainMot()
{
	
	do {
		cout << motCourant.UL << endl;
		motCourant = analyseurLexical->uniteSuivante();

		if (motCourant.UL == ERR1 || motCourant.UL == ERR2 || motCourant.UL == ERR3 || motCourant.UL == ERR4)
		{
			cout << to_string(analyseurLexical->getLigne())<< " "<< motCourant.UL << endl;
			
		}
	} while (motCourant.UL == ERR1 || motCourant.UL == ERR2 || motCourant.UL == ERR3 || motCourant.UL == ERR4);

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
					prochainMot();
					while (motCourant.UL!=ERR5)
					{
						lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mots trouves apres fin de programme");
						prochainMot();
					}
					return true;
				}
				else
				{
					lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <fin> attendu");
					return true;
				}
			}
		}
	}
	else
	{
		lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <debut> attendu");
		return true;
	}
}

bool AnalyseurSyntaxique::listeDeDeclaration()
{
	//<liste de declarations> –> <declaration> ; <liste de declarations>
	if (declaration())
	{
		if (motCourant.UL == POINTVIR)
		{
			cout << analyseurLexical->getLigne() <<"pt vir" << endl;

			prochainMot();//motCourant = analyseurLexical->uniteSuivante();
			if (listeDeDeclaration())
				return true;
		}
		else
		{
			lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <;> attendu");
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

					if (nbEntier()) 
					{
						if (motCourant.UL == CROCHETFER)
						{
							prochainMot();//motCourant = analyseurLexical->uniteSuivante();
							return declarationPrime();
						}
						else
						{
							lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <]> attendu");
							return true;
						}
					}
				}
				else
				{
					lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <[> attendu");
						return true;
				}
			}
		}
		else
		{
			lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <entier> attendu");
				return true;
		}
	}
	else
	{
		lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <tableau> ou mot <entier> attendu");
			return true;
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
		if (nbEntier())
		{
			if (motCourant.UL == CROCHETFER)
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				return declarationPrime();
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <]> attendu");
				return true;
			}
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
	if (instruction())
	{
		if (motCourant.UL == POINTVIR)
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();
			if (listeDIstruction())
				return true;
		}
		else
		{
			lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <;> attendu");
			return true;
		}
	}
	else if (suivantListeDInstruction())
		//<liste d’instructions>  –>e
		return true;
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
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <fin> attendu");
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
		if (expression())
		{
			if (motCourantEgalAuMotCle("alors"))
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();

				if (instruction())
				{
					if (sinon())
					{
						return true;
					}
				}
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <alors> attendu");
				return true;
			}

		}
		
	}
	//<instruction> –>repeter <instruction> jusque <expression>
	else if (motCourantEgalAuMotCle("repeter"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (instruction())
		{
			if (motCourantEgalAuMotCle("jusque"))
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				if (expression())
				{
					return true;
				}
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <jusque> attendu");
				return true;
			}
		}
	}
	//<instruction> –>tantque <expression> faire <instruction>
	else if (motCourantEgalAuMotCle("tantque"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expression())
		{
			if (motCourantEgalAuMotCle("faire"))
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();

				if (instruction())
				{
					return true;
				}
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <faire> attendu");
				return true;
			}
		}	
		
	}
	//<instruction> –>pour <identificateur> allantde <nb entier> a <nb entier> faire <instruction>
	else if (motCourantEgalAuMotCle("pour"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();

		if (identificateur())
		{
			if (motCourantEgalAuMotCle("allantde"))
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				if (nbEntier())
				{
					if (motCourantEgalAuMotCle("a"))
					{
						prochainMot();//motCourant = analyseurLexical->uniteSuivante();

						if (nbEntier())
						{
							if (motCourantEgalAuMotCle("faire"))
							{
								prochainMot();//motCourant = analyseurLexical->uniteSuivante();
								if (instruction())
								{
									return true;
								}
							}
							else
							{
								lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <faire> attendu");
								return true;
							}
						}	
					}
					else
					{
						lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": lettre <a> attendu");
						return true;
					}
				}
					
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <allantde> attendu");
				return true;
			}
		}
		
	}
	//<instruction> –>switch <identificateur> faire <cases>
	else if (motCourantEgalAuMotCle("switch"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (identificateur())
		{
			if (motCourantEgalAuMotCle("faire"))
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();

				if (cases())
				{
					return true;
				}
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <faire> attendu");
				return true;
			}
		}
			
	}
	//<instruction> -> ecrire <liste d’arguments>
	else if (motCourantEgalAuMotCle("ecrire"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (listeDArgument())
		{
			return true;
		}
	}
	//<instruction> -> lire <identificateur>
	else if (motCourantEgalAuMotCle("lire"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (identificateur())
		{
			return true;
		}
	}
	//<instruction> -> <expression>
	else if (expression())
	{
		return true;
	}
	//<instruction> -> <identificateur> <instruction prime>
	else if (identificateur())
	{
		if (instructionPrime())
		{
			return true;
		}
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
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <]> attendu");
				return true;
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
				else
				{
					lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <==> attendu");
					return true;
				}
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <]> attendu");
				return true;
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
						{
							return true;
						}
						return false;
					}
					else
					{
						lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": mot <arret> attendu");
						return true;
					}
				}
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <:> attendu");
				return true;
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
	if (motCourant.UL == PLUS) 
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
	//<expression simple prime> –>- <terme> <expression simple prime>
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
	//<expression simple prime> –>|| <terme> <expression simple prime>
	else if (motCourant.UL == OU) 
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

	else if (suivantExpressionSimplePrime())

		//<expression simple prime> –>e
		return true;
	return false;
}

bool AnalyseurSyntaxique::terme()
{
	//<terme> –> <facteur> <terme prime>
	if (facteur())
	{
		if (termePrime())
		{
			return true;
		}
	}
}

bool AnalyseurSyntaxique::termePrime()
{
	//<terme prime> –> * <facteur> <terme prime>
	if (motCourant.UL == MULTIP) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (facteur())
		{
			if (termePrime())
			{
				return true;
			}
		}
	}
	//<terme prime> –>/ <facteur> <terme prime>
	else if (motCourant.UL == DIVISION) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (facteur())
		{
			if (termePrime())
			{
				return true;
			}
		}
	}
	//<terme prime> –>% <facteur> <terme prime>
	else if (motCourant.UL == MODULO) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (facteur())
		{
			if (termePrime())
			{
				return true;
			}
		}

	}
	//<terme prime> –>&& <terme prime>
	else if (motCourant.UL == ET) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (termePrime())
		{
			return true;
		}
	}
	//<terme prime> –>e
	else if (suivantTermePrime())
	{
		return true;
	}
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
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <)> attendu");
				return true;
			}
			return false;
		}
	}
	//<facteur> –> ! <facteur>
	else if (motCourant.UL == NEGATION) {
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (facteur())
		{
			return true;
		}
	}
	//<facteur> –><identificateur> <facteur prime>
	else if (identificateur()) {
		if (facteurPrime())
		{
			return true;
		}
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
				if (facteurSeconde())
				{
					return true;
				}
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <]> attendu");
				return true;
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
		if (expressionSimple())
		{
			if (motCourant.UL == CROCHETFER)
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				return true;
			}
			else
			{
				lesErreurs.ajouterErreur(to_string(analyseurLexical->getLigne()) + ": symbole <]> attendu");
				return true;
			}
		}
			
	}
	else if (suivantFacteurSeconde())
		return true;
	return false;
}

bool AnalyseurSyntaxique::listeDArgument()
{
	if (expression())
	{
		if (listeDArgumentPrime())
		{
			return true;
		}
	}
		
}

bool AnalyseurSyntaxique::listeDArgumentPrime()
{
	if (motCourant.UL == VIR) 
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (listeDArgument())
		{
			return true;
		}
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
	else if (motCourant.UL == INF)
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


bool AnalyseurSyntaxique::identificateur()
{
	if (motCourant.UL == IDENT)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	else
	{
		return false;
	}
}

bool AnalyseurSyntaxique::nbEntier()
{
	if (motCourant.UL == NBRENTIER)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		return true;
	}
	else
	{
		return false;
	}
}
bool AnalyseurSyntaxique::suivantProgramme()
{
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
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::suivantDeclaration()
{
	return motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantDeclarationPrime()
{
	return motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantListeDInstruction()
{
	return motCourantEgalAuMotCle("fin");
}

bool AnalyseurSyntaxique::suivantInstruction()
{
	return motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantInstructionPrime()
{
	return motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantInstructionSeconde()
{
	return motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourant.UL == POINTVIR;
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

bool AnalyseurSyntaxique::suivantExpression()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourant.UL == VIR
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

bool AnalyseurSyntaxique::suivantExpressionSimple()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourant.UL == CROCHETFER
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| motCourant.UL == PARFER
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
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantTerme()
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
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
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
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantFacteur()
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
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
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
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
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
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantListeDArgument()
{
	return motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantListeDArgumentPrime()
{
	return motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantComparaison()
{
	return motCourant.UL == MOINS
		|| motCourant.UL == IDENT
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::suivantIdentificateur()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("allantde")
		|| motCourant.UL == CROCHETOUV
		|| motCourant.UL == AFFEC
		|| motCourant.UL == MULTIP
		|| motCourant.UL == DIVISION
		|| motCourant.UL == MODULO
		|| motCourant.UL == ET
		|| motCourant.UL == PLUS
		|| motCourant.UL == MOINS
		|| motCourant.UL == OU
		|| motCourant.UL == CROCHETFER
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::suivantNbEntier()
{
	return motCourantEgalAuMotCle("alors")
		|| motCourantEgalAuMotCle("faire")
		|| motCourantEgalAuMotCle("sinon")
		|| motCourantEgalAuMotCle("jusque")
		|| motCourantEgalAuMotCle("arret")
		|| motCourantEgalAuMotCle("allantde")
		|| motCourant.UL == CROCHETOUV
		|| motCourant.UL == AFFEC
		|| motCourant.UL == MULTIP
		|| motCourant.UL == DIVISION
		|| motCourant.UL == MODULO
		|| motCourant.UL == ET
		|| motCourant.UL == PLUS
		|| motCourant.UL == MOINS
		|| motCourant.UL == OU
		|| motCourant.UL == CROCHETFER
		|| motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| motCourant.UL == PARFER
		|| motCourant.UL == VIR
		|| motCourant.UL == POINTVIR;
}

bool AnalyseurSyntaxique::premierProgramme()
{
	return motCourantEgalAuMotCle("debut");
}

bool AnalyseurSyntaxique::premierListeDeDeclaration()
{
	return motCourantEgalAuMotCle("entier")
		|| motCourantEgalAuMotCle("tableau")
		|| suivantListeDeDeclaration();
}

bool AnalyseurSyntaxique::premierDeclaration()
{
	return motCourantEgalAuMotCle("entier")
		|| motCourantEgalAuMotCle("tableau");
}

bool AnalyseurSyntaxique::premierDeclarationPrime()
{
	return motCourant.UL == CROCHETOUV
		|| suivantDeclarationPrime();
}

bool AnalyseurSyntaxique::premierListeDIstruction()
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
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER
		|| suivantListeDInstruction();
}

bool AnalyseurSyntaxique::premierInstruction()
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
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::premierInstructionPrime()
{
	return motCourant.UL == CROCHETOUV
		|| motCourant.UL == AFFEC;
}

bool AnalyseurSyntaxique::premierInstructionSeconde()
{
	return motCourant.UL == CROCHETOUV
		|| motCourant.UL == AFFEC;
}

bool AnalyseurSyntaxique::premierSinon()
{
	return motCourantEgalAuMotCle("sinon")
		|| suivantSinon();
}

bool AnalyseurSyntaxique::premierCases()
{
	return motCourantEgalAuMotCle("cas")
		|| suivantCases();
}

bool AnalyseurSyntaxique::premierExpression()
{
	return motCourant.UL == MOINS
		|| motCourant.UL == IDENT
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::premierExpressionPrime()
{
	return motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP
		|| suivantExpressionPrime();
}

bool AnalyseurSyntaxique::premierExpressionSimple()
{
	return motCourant.UL == MOINS
		|| motCourant.UL == IDENT
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::premierExpressionSimplePrime()
{
	return motCourant.UL == PLUS
		|| motCourant.UL == MOINS
		|| motCourant.UL == OU
		|| suivantExpressionSimplePrime();
}

bool AnalyseurSyntaxique::premierTerme()
{
	return motCourant.UL == IDENT
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::premierTermePrime()
{
	return motCourant.UL == MULTIP
		|| motCourant.UL == DIVISION
		|| motCourant.UL == MODULO
		|| motCourant.UL == ET
		|| suivantTermePrime();
}

bool AnalyseurSyntaxique::premierFacteur()
{
	return motCourant.UL == IDENT
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::premierFacteurPrime()
{
	return motCourant.UL == CROCHETOUV
		|| suivantFacteurPrime();
}

bool AnalyseurSyntaxique::premierFacteurSeconde()
{
	return  motCourant.UL == CROCHETOUV
		|| suivantFacteurSeconde();
}

bool AnalyseurSyntaxique::premierListeDArgument()
{
	return motCourant.UL == MOINS
		|| motCourant.UL == IDENT
		|| motCourant.UL == NEGATION
		|| motCourant.UL == PAROUV
		|| motCourant.UL == NBRENTIER;
}

bool AnalyseurSyntaxique::premierListeDArgumentPrime()
{
	return motCourant.UL == VIR
		|| suivantListeDArgumentPrime();
}

bool AnalyseurSyntaxique::premierComparaison()
{
	return motCourant.UL == EGAL
		|| motCourant.UL == DIFF
		|| motCourant.UL == INFEG
		|| motCourant.UL == SUPEG
		|| motCourant.UL == INF
		|| motCourant.UL == SUP;
}

bool AnalyseurSyntaxique::premierIdentificateur()
{
	return motCourant.UL == IDENT;
}

bool AnalyseurSyntaxique::premierNbEntier()
{
	return motCourant.UL == NBRENTIER;
}
