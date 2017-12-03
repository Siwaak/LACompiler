#include "stdafx.h"
#include "AnalyseurLexical.h"
#include <fstream>;
#include <iostream>


AnalyseurLexical::AnalyseurLexical(string fichier)
{
	code = "debut entier x; entier i; tableau T[13] debut fin fin";
	c = ' ';//Initialisation du caractère par un esapce

	//ifstream lecteurFichier;
	//lecteurFichier.open(fichier);

	//// Vérification de l'ouverture du fichier
	//if (!lecteurFichier.is_open()) {
	//	cout << "Impossible d'ouvrire le fichier!" << endl;
	//	exit(EXIT_FAILURE);
	//}
	//string ligne;

	//// Copie du contenue du fichier dans la variable code
	//while (lecteurFichier >> ligne) {
	//	code += ligne;
	//}

	//// Fermeture du fichier
	//lecteurFichier.close();
	//cout << code;
}


AnalyseurLexical::~AnalyseurLexical()
{

}

bool AnalyseurLexical::estBlanc(char caractere)
{
	return caractere == ' ' || caractere == '\t' || caractere == '\n' || estCommentaire(c);
}

char AnalyseurLexical::lireCaractere()
{
	//if (code.size() > 0) {
	//	char c = code[0];
	//	code.erase(0, 1);
	//}
	//else c = EOF;//Si le code est fini, on renvoie un end of file
	cin >> c;
	return c;
}

TLexeme AnalyseurLexical::uniteSuivante()
{
	TLexeme lex;
	while (estBlanc(c))
		c = lireCaractere();
	switch (c) {
	case ',':
		lex.UL = VIR;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case ';':
		lex.UL = POINTVIR;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case '+':
		lex.UL = PLUS;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case '-':
		lex.UL = MOINS;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case '*':
		lex.UL = MULTIP;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case '/':
		lex.UL = MULTIP;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case '%':
		lex.UL = MODULO;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case '(':
		lex.UL = PAROUV;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case ')':
		lex.UL = PARFER;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case '[':
		lex.UL = CROCHETOUV;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case ']':
		lex.UL = CROCHETFER;
		lex.attribut = -1;
		c = lireCaractere();

		break;
	case '=':
		c = lireCaractere();
		if (c == '=')
		{
			lex.UL = EGAL;
			lex.attribut = -1;
			c = lireCaractere();

		}
		else
		{
			lex.UL = AFFEC;
			lex.attribut = -1;
		}
		break;
	case '<':
		c = lireCaractere();
		if (c == '=')
		{
			lex.UL = INFEG;
			lex.attribut = -1;
			c = lireCaractere();

		}
		else
		{
			lex.UL = INF;
			lex.attribut = -1;
		}
		break;
	case '>':
		c = lireCaractere();
		if (c == '=')
		{
			lex.UL = SUPEG;
			lex.attribut = -1;
			c = lireCaractere();

		}
		else
		{
			lex.UL = SUP;
			lex.attribut = -1;
		}
		break;
	case '!':
		c = lireCaractere();
		if (c == '=')
		{
			lex.UL = DIFF;
			lex.attribut = -1;
			c = lireCaractere();

		}
		else
		{
			lex.UL = NEGATION;
			lex.attribut = -1;
		}
		break;
	case '&':
		c = lireCaractere();
		if (c == '&')
		{
			lex.UL = ET;
			lex.attribut = -1;
			c = lireCaractere();

		}
		else
		{
			lex.UL = ERR1;
			lex.attribut = -1;
		}
		break;
	case '|':
		c = lireCaractere();
		if (c == '|')
		{
			lex.UL = OU;
			lex.attribut = -1;
			c = lireCaractere();

		}
		else
		{
			lex.UL = ERR2;
			lex.attribut = -1;
		}
		break;
	default:
		if (estLettre(c)) {
			lexeme = c;
			
			c = lireCaractere();
			while (estLettre(c) || estChiffre(c) || c == '_')
			{
				lexeme+=c;
				c = lireCaractere();
			}
			lexeme+='\0';
			if (estMotCle(lexeme))
				lex.UL = MOTCLE;
			lex.UL = IDENT;
			lex.attribut = indexIdentifiant(lexeme);
		
		}
		else if (estChiffre(c))
		{
			lexeme.push_back(c); // remplissage du tableau lexeme
			c = lireCaractere();
			while (estChiffre(c))
			{
				lexeme.push_back(c);
				c = lireCaractere();				
			}
			lexeme += '\0';
			lex.UL = NBRENTIER;
			lex.attribut = std::stoi(lexeme); //Converti la chaine lexeme en entier
		}
		else
		{
			lex.UL = ERR3;
			lex.attribut = -1;
		}

		break;

	}
	return lex;
}


int AnalyseurLexical::indexIdentifiant(string chaine)
{
	//return tableIdent.insert(chaine);
	return false;
}



int AnalyseurLexical::estMotCle(string mot)
{
	//return tableMotCle.find(mot)->second;
	return false;
}

bool AnalyseurLexical::estLettre(char c)
{
	return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

bool AnalyseurLexical::estChiffre(char c)
{
	return '0' <= c && c <= '9';
}

bool AnalyseurLexical::estCommentaire(char)
{
	if (c == '{')
	{
		c = lireCaractere();
			while (c != '}')
		{
			c = lireCaractere();
		}
		return true;
	}
	return false;
}
