#include "stdafx.h"
#include "AnalyseurLexical.h"
#include <fstream>;
#include <iostream>

using namespace std;
AnalyseurLexical::AnalyseurLexical(string fichier)
{
	//code = "debut entier x; entier i; tableau T[13] debut fin fin";
	c = ' ';//Initialisation du caractère par un esapce

	ifstream lecteurFichier;
	lecteurFichier.open(fichier.c_str());

	// Vérification de l'ouverture du fichier
	if (!lecteurFichier.is_open()) {
		cout << "Impossible d'ouvrire le fichier!" << endl;
		exit(EXIT_FAILURE);
	}
	string ligne;

	// Copie du contenue du fichier dans la variable code
	while (getline(lecteurFichier,ligne)) {
		code += ligne;
	}

	// Fermeture du fichier
	lecteurFichier.close();
	cout << code;
}


AnalyseurLexical::~AnalyseurLexical()
{

}

//fonction de hachage
//h(i)=h(i-1)*33+codeASCII du caractère;
 long AnalyseurLexical::hashCode(string chaine)
{
	unsigned long hash = 7;
	int c;

	for (int i = 0; i< chaine.length(); i++)
		hash = hash * 33 + chaine[i];

	return hash;
}

bool AnalyseurLexical::estBlanc(char caractere)
{
	return caractere == ' ' || caractere == '\t' || caractere == '\n';
}

char AnalyseurLexical::lireCaractere()
{
	if (code.size() > 0) {
		 c = code[0];
		code.erase(0, 1);
	}
	else c = EOF;//Si le code est fini, on renvoie un end of file
	
	return c;
}

TLexeme AnalyseurLexical::uniteSuivante()
{
	TLexeme lex;

	while (estBlanc(c)) {
		c = lireCaractere();		
	}
		

	
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
		lex.UL = DIVISION;
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
	case '{':
		c = lireCaractere();
		while (c != '}')
		{
			lireCaractere();
			cout << " ici";
		}
		if (c == '}') {
			lex.UL = COMMENTAIRE;
			lex.attribut = -1;
		}
		else
		{
			lex.UL = ERR4;
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
			if (estMotCle(lexeme)) {
				lex.UL = MOTCLE;
				lex.attribut = hashCode(lexeme);
			}
			else {
				lex.UL = IDENT;
				lex.attribut = indexIdentifiant(lexeme);
			}
		
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


 long AnalyseurLexical::indexIdentifiant(string chaine)
{
	 
	long index = hashCode(chaine);
	tableIdent[index]=chaine;

	return index;
}



 bool AnalyseurLexical::estMotCle(string mot)
{
	long index = hashCode(mot);
	if(tableMotCle.find(index)!= tableMotCle.end())
		return true;
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

bool AnalyseurLexical::estCommentaire(char carac)
{
	if (carac == '{')
	{
		carac = lireCaractere();
		while (c != '}' || c!=EOF)
		{
			c = lireCaractere();
		}
		if(c== '}')
			return true;
	}
	return false;
}

bool AnalyseurLexical::codeEstFini()
{

	return code.size()==0 || code[0] == EOF;
}

void AnalyseurLexical::afficherTableMotsCle()
{
	for (auto elem : tableMotCle)
	{
		std::cout << elem.first << "\t" << elem.second << endl;
	}
}

void AnalyseurLexical::afficherTableIdentificateurs()
{
	for (auto elem : tableIdent)
	{
		std::cout << elem.first << "\t" << elem.second << endl;
	}
}
