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
	return caractere == ' ' || caractere == '\t' || caractere == '\n' || estCommentaire(c);
}

char AnalyseurLexical::lireCaractere()
{
	return 0;
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
	return tableIdent.insert(chaine);
}



int AnalyseurLexical::estMotCle(string mot)
{
	return tableMotCle.find(mot)->second;
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
			c == lireCaractere();
		}
		return true;
	}
	return false;
}
