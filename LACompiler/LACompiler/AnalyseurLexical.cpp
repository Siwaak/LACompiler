#include "stdafx.h"
#include "AnalyseurLexical.h"


AnalyseurLexical::AnalyseurLexical()
{

}


AnalyseurLexical::~AnalyseurLexical()
{

}

bool AnalyseurLexical::estBlanc(char caractere)
{

	return caractere == ' ' || caractere == '\t' || caractere == '\n';
}

char AnalyseurLexical::lireCaractere()
{
	return 0;
}

int AnalyseurLexical::lexemeSuivant()
{
	char caracSuivant = ' ';
	switch (caracSuivant) {
	case '{':
		do {
			lireCaractere();
		} while (caracSuivant != '}');

		return COMMENTAIRE;

		break;
	case ',':
		caracSuivant = lireCaractere();

		return VIRGULE;
		break;
	case ';':
		caracSuivant = lireCaractere();

		return POINTVIRGULE;
		break;
	case '=':
		caracSuivant = lireCaractere();

		if (caracSuivant == '=') {
			caracSuivant = lireCaractere();
			return EGAL;
		}

		return AFFECTATION;
		break;
	case '+':
		caracSuivant = lireCaractere();
		return PLUS;
		break;
	case '-':
		caracSuivant = lireCaractere();
		return MOINS;
		break;
	case '*':
		caracSuivant = lireCaractere();
		return MULTIPLICATION;
		break;
	case '/':
		caracSuivant = lireCaractere();
		return DIVISION;
		break;
	case '%':
		caracSuivant = lireCaractere();
		return MODULO;
		break;
	case '!':
		caracSuivant = lireCaractere();

		if (caracSuivant == '=') {
			caracSuivant = lireCaractere();
			return DIFFERENT;
		}
		return NEGATION;
		break;
	case '>':
		caracSuivant = lireCaractere();

		if (caracSuivant == '=') {
			caracSuivant = lireCaractere();
			return SUPEGAL;
		}
		return SUPPERIEUR;
		break;
	case '<':
		caracSuivant = lireCaractere();

		if (caracSuivant == '=') {
			caracSuivant = lireCaractere();
			return INFFEGEAL;
		}
		return INFERRIEUR;
		break;
	case '&':
		caracSuivant = lireCaractere();

		if (caracSuivant == '&') {
			caracSuivant = lireCaractere();
			return ET;
		}
		return ERREUR;
		break;
	case '|':
		caracSuivant = lireCaractere();

		if (caracSuivant == '|') {
			caracSuivant = lireCaractere();
			return OU;
		}
		return ERREUR;
		break;
	case '%':
		caracSuivant = lireCaractere();
		return MODULO;
		break;
	case '%':
		caracSuivant = lireCaractere();
		return MODULO;
		break;


	}

}
