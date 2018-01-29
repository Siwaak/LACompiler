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

bool AnalyseurSyntaxique::estDejaDeclare(long index)
{
	if (analyseurLexical->tableIdent.find(index) != analyseurLexical->tableIdent.end())
		return true;
	return false;
}

bool AnalyseurSyntaxique::nEstPasTableauDim2(long index)
{
	return analyseurLexical->tableIdent[index].type != "tableau2";
}

bool AnalyseurSyntaxique::nEstPasTableau(long index)
{
	return analyseurLexical->tableIdent[index].type != "tableau";
}


bool AnalyseurSyntaxique::motCourantEgalAuMotCle(string chaineLexicale)
{
	//comparaison de l'ul et de l'attribut
	return motCourant.UL == MOTCLE && motCourant.attribut == AnalyseurLexical::tableMotCle[chaineLexicale];
}

void AnalyseurSyntaxique::verifierSyntaxe()
{
	if (programme())
		cout << "Le programme ne comporte aucune erreure syntaxique " << endl;
	else cout << "Le programme comporte des erreurs syntaxiques" << endl;
}

void AnalyseurSyntaxique::prochainMot()
{	
	do {
		//cout << motCourant.UL << endl;
		motCourant = analyseurLexical->uniteSuivante();

		if (motCourant.UL == ERR1 || motCourant.UL == ERR2 || motCourant.UL == ERR3 || motCourant.UL == ERR4)
		{
			cout << to_string(analyseurLexical->getLigne())<< " "<< motCourant.UL << endl;
			
		}
	} while (motCourant.UL == ERR1 || motCourant.UL == ERR2 || motCourant.UL == ERR3 || motCourant.UL == ERR4);

}
//void AnalyseurSyntaxique::prochainMot()
//{
//	cout << motCourant.UL << endl;
//	motCourant = analyseurLexical->uniteSuivante();
//}

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
				else
				{
					cout << analyseurLexical->getLigne() << ": Mot cle 'fin' attendu" << endl;
				}
			}
			else
			{
				cout << analyseurLexical->getLigne() << ": Liste d'instructions attendu" << endl;
				return false;
			}
		}
	}
	else
	{
		cout << analyseurLexical->getLigne() << ": Mot cle 'debut' attendu" << endl;
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
		else
		{
			cout << analyseurLexical->getLigne() << ": Point virgule attendu" << endl;
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
		/*if (identificateur())
			return true;*/
		if (motCourant.UL == IDENT) 
		{ 
			if (estDejaDeclare(motCourant.attribut))
			{
				//erreur semantique
				cout << "La variable " << analyseurLexical->dernierIdent << " est deja déclarée" << endl;
			}
			
				analyseurLexical->ajouterIdent(analyseurLexical->dernierIdent, "entier");
			
			//analyseurLexical->tableIdent[motCourant.attribut].type = "entier";
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();
			return true;
		}
		else
		{
			cout << analyseurLexical->getLigne() << ": Identificateur attendu" << endl;
		}
	}
	//<declaration> –>tableau entier <identificateur>[<nb entier>] <declaration prime>
	else if (motCourantEgalAuMotCle("tableau"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();

		if (motCourantEgalAuMotCle("entier"))
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();
			if (motCourant.UL == IDENT)
			{
				long index = motCourant.attribut;

				if (estDejaDeclare(index))
				{
					cout << "Le tableau " << analyseurLexical->dernierIdent << " est deja declaree" << endl;
				}

				
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				if (motCourant.UL == CROCHETOUV)
				{
					prochainMot();//motCourant = analyseurLexical->uniteSuivante();

					if (motCourant.UL == NBRENTIER)
					{
						long nbre = motCourant.attribut;
						prochainMot();
						if (motCourant.UL == CROCHETFER)
						{
							prochainMot();//motCourant = analyseurLexical->uniteSuivante();

							return declarationPrime(index, nbre);
						}
						else
							cout << analyseurLexical->getLigne() << ": Crochet fermante attendu" << endl;

					}

				}
				else
					cout << analyseurLexical->getLigne() << ": Crochet ouvrante attendu" << endl;
			}
			else
				cout << analyseurLexical->getLigne() << ": Identificateur attendu" << endl;
		}
		else
		{
			cout << analyseurLexical->getLigne() << ": Mot cle 'entier' attendu" << endl;
		}
	}
		
	return  false;
	

	
}

bool AnalyseurSyntaxique::declarationPrime(long index,long nbre )
{
	//<declaration prime> –> [ <nb entier> ]
	if (motCourant.UL == CROCHETOUV)
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (motCourant.UL == NBRENTIER)
		{
			long nbre2 = motCourant.attribut;
			prochainMot();
			if (motCourant.UL == CROCHETFER)
			{
				analyseurLexical->ajouterIdent(analyseurLexical->dernierIdent, "tableau2");
				analyseurLexical->tableIdent[index].taille1 = nbre;
				analyseurLexical->tableIdent[index].taille2 = nbre2;

				prochainMot();//motCourant = analyseurLexical->uniteSuivante();

				return true;
			}
		}

	}
	else if (suivantDeclarationPrime())
	{
		//<declaration prime> –>e
		analyseurLexical->ajouterIdent(analyseurLexical->dernierIdent, "tableau");
		analyseurLexical->tableIdent[index].taille1 = nbre;
		return true;
	}	

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
		if (motCourant.UL == IDENT)
		{
			long index = motCourant.attribut;
			if (!estDejaDeclare(index))
			{
				cout << analyseurLexical->dernierIdent << " n'est pas encore declare" << endl;
			}
			else if (!nEstPasTableau(index))
			{
				cout << analyseurLexical->dernierIdent << " est un tableau " << endl;
			}

			else if (!nEstPasTableauDim2(index))
			{
				cout << analyseurLexical->dernierIdent << " est une matrice " << endl;
			}
			//analyseurLexical->ajouterIdent(analyseurLexical->dernierIdent, "entier");
			prochainMot();

			if (motCourantEgalAuMotCle("allantde"))
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
	}
	//<instruction> –>switch <identificateur> faire <cases>
	else if (motCourantEgalAuMotCle("switch"))
	{
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (motCourant.UL == IDENT)
		{
			long index = motCourant.attribut;
			if (!estDejaDeclare(index))
			{
				cout << analyseurLexical->dernierIdent << " n'est pas encore declare" << endl;
			}
			else if (!nEstPasTableau(index))
			{
				cout << analyseurLexical->dernierIdent << " est un tableau " << endl;
			}

			else if (!nEstPasTableauDim2(index))
			{
				cout << analyseurLexical->dernierIdent << " est une matrice " << endl;
			}
			//analyseurLexical->ajouterIdent(analyseurLexical->dernierIdent, "entier");
			prochainMot();
			if (motCourantEgalAuMotCle("faire"))
			{
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();

				return cases();
			}
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
		if (motCourant.UL == IDENT)
		{
			long index = motCourant.attribut;
			if (!estDejaDeclare(index))
			{
				cout << analyseurLexical->dernierIdent << " n'est pas encore declare" << endl;
			}
			else if (!nEstPasTableau(index))
			{
				cout << analyseurLexical->dernierIdent << " est un tableau " << endl;
			}

			else if (!nEstPasTableauDim2(index))
			{
				cout << analyseurLexical->dernierIdent << " est une matrice " << endl;
			}
			//analyseurLexical->ajouterIdent(analyseurLexical->dernierIdent, "entier");
			prochainMot();
			return true;
		}
		return false;
	}
	//<instruction> -> <identificateur> <instruction prime>
	else if (motCourant.UL == IDENT)
	{
		long index = motCourant.attribut;
		
		if (!estDejaDeclare(index))
		{
			cout << analyseurLexical->dernierIdent << " n'est pas encore declare" << endl;
		}
		//analyseurLexical->ajouterIdent(analyseurLexical->dernierIdent, "entier");
		
			
			prochainMot();
			
		return instructionPrime(index);
	}
	//<instruction> -> <expression>
	else if (expression())
	{
		
		return true;
	}


	return false;
}

bool AnalyseurSyntaxique::instructionPrime(long index)
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
				if (instructionSeconde(index))
				{
					return true;
				}
				return false;
			}
		}
	}
	return false;
}

bool AnalyseurSyntaxique::instructionSeconde(long index)
{
	if (motCourant.UL == AFFEC)
	{
		if (nEstPasTableau(index))
		{
			cout << analyseurLexical->tableIdent[index].nom << "n'est pas un tableau" << endl;
		}
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
		if (nEstPasTableauDim2(index))
		{
			cout << analyseurLexical->tableIdent[index].nom << "n'est pas  une matrice" << endl;
		}
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
	//else if (identificateur()) {
	else if (motCourant.UL == IDENT) {
		if (!estDejaDeclare(analyseurLexical->hashCode(analyseurLexical->dernierIdent)))
		{
			cout << analyseurLexical->dernierIdent << " n'est pas encore declare" << endl;
		}
		//analyseurLexical->ajouterIdent(analyseurLexical->dernierIdent, "entier");

		long index = analyseurLexical->hashCode(analyseurLexical->dernierIdent);
		prochainMot();
		return facteurPrime(index);
	}
	return false;
}

bool AnalyseurSyntaxique::facteurPrime(long index)
{
	//<facteur prime> –> [ <expression simple> ] <facteur seconde>
	if (motCourant.UL == CROCHETOUV) {
		
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple()) {
			if (motCourant.UL == CROCHETFER) {
				prochainMot();//motCourant = analyseurLexical->uniteSuivante();
				return facteurSeconde(index);
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

bool AnalyseurSyntaxique::facteurSeconde(long index)
{
	if (motCourant.UL == CROCHETOUV)
	{

		if (nEstPasTableauDim2(index))
		{
			//err Sémantique
			cout << analyseurLexical->dernierIdent << " n'est pas  une matrice" << endl;
		}
		prochainMot();//motCourant = analyseurLexical->uniteSuivante();
		if (expressionSimple() && motCourant.UL == CROCHETFER)
		{
			prochainMot();//motCourant = analyseurLexical->uniteSuivante();

			return true;
		}
	}
	else if (suivantFacteurSeconde())
	{
		if (nEstPasTableau(index))
		{
			//err Sémantique
			cout << analyseurLexical->dernierIdent << " n'est pas un tableau" << endl;
		}
		return true;
	}

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
