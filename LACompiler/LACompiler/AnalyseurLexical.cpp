#include "stdafx.h"
#include "AnalyseurLexical.h"
#include <fstream>;
#include <iostream>

using namespace std;

const map<string, int>AnalyseurLexical::tableMotCle = {
														{ "debut",1 },
														{ "fin",2 },
														{ "entier",3 },
														{ "tableau",4 },
														{ "arret",5 },
														{ "si",6 },
														{ "sinon",7 },
														{ "alors",8 },
														{ "tantque",9 },
														{ "faire",10 },
														{ "ecrire",11 },
														{ "lire",12 },
														{ "repeter",13 },
														{ "jusque",14 },
														{ "cas",15 },
														{ "pour",16 },
														{ "switch",17 },
														{ "a",18},
														{ "allantde",19}

}; //liste des motsCle

AnalyseurLexical::AnalyseurLexical(string fichier)
{
	code = "";// initialisation du code;

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
	while (getline(lecteurFichier, ligne)) {
		code += ligne + '\n';
	}

	// Fermeture du fichier
	lecteurFichier.close();
}


AnalyseurLexical::~AnalyseurLexical()
{

}

//fonction de hachage
//h(i)=h(i-1)*33+codeASCII du caractère;

long AnalyseurLexical::hashCode(string chaine)
{
	long hash = 0;


	for (int i = 0; i < chaine.length(); i++)
		hash = hash * 33 + chaine[i];

	return hash;
}

bool AnalyseurLexical::estBlanc(char caractere)
{
	return caractere == ' ' || caractere == '\t' || caractere == '\n';
}

void AnalyseurLexical::lireCaractere()
{
	if (code.size() > 0) {
		//on lit le caractère
		c = code[0];
		//On supprime la case correspondante
		code.erase(0, 1);
	}
	else c = EOF;//Si le code est fini, on renvoie un end of file

}

TLexeme AnalyseurLexical::uniteSuivante()
{//cherche à quel unitélexicale correspond le caractère ou la suite de caractère qui suivent
	TLexeme lex;
	lexeme = "";
	while (estBlanc(c) || estCommentaire(c)) {
		lireCaractere();
		if (c == '\n') cout << endl;
	}
	switch (c) {
	case ',':
		lex.UL = VIR;
		lex.attribut = -1;
		lireCaractere();

		break;
	case ';':
		lex.UL = POINTVIR;
		lex.attribut = -1;
		lireCaractere();

		break;
	case '+':
		lex.UL = PLUS;
		lex.attribut = -1;
		lireCaractere();
		break;

	case '-':
		lex.UL = MOINS;
		lex.attribut = -1;
		lireCaractere();
		break;
	case '*':
		lex.UL = MULTIP;
		lex.attribut = -1;
		lireCaractere();

		break;
	case '/':
		lex.UL = DIVISION;
		lex.attribut = -1;
		lireCaractere();

		break;
	case '%':
		lex.UL = MODULO;
		lex.attribut = -1;
		lireCaractere();

		break;
	case '(':
		lex.UL = PAROUV;
		lex.attribut = -1;
		lireCaractere();

		break;
	case ')':
		lex.UL = PARFER;
		lex.attribut = -1;
		lireCaractere();

		break;
	case '[':
		lex.UL = CROCHETOUV;
		lex.attribut = -1;
		lireCaractere();

		break;
	case ']':
		lex.UL = CROCHETFER;
		lex.attribut = -1;
		lireCaractere();

		break;
	case '=':
		lireCaractere();
		if (c == '=')
		{
			lex.UL = EGAL;
			lex.attribut = -1;
			lireCaractere();

		}
		else
		{
			lex.UL = AFFEC;
			lex.attribut = -1;
		}
		break;
	case '<':
		lireCaractere();
		if (c == '=')
		{
			lex.UL = INFEG;
			lex.attribut = -1;
			lireCaractere();
		}
		else
		{
			lex.UL = INF;
			lex.attribut = -1;
		}
		break;
	case '>':
		lireCaractere();
		if (c == '=')
		{
			lex.UL = SUPEG;
			lex.attribut = -1;
			lireCaractere();
		}
		else
		{
			lex.UL = SUP;
			lex.attribut = -1;
		}
		break;
	case '!':
		lireCaractere();
		if (c == '=')
		{
			lex.UL = DIFF;
			lex.attribut = -1;
			lireCaractere();

		}
		else
		{
			lex.UL = NEGATION;
			lex.attribut = -1;
		}
		break;
	case '&':
		lireCaractere();
		if (c == '&')
		{
			lex.UL = ET;
			lex.attribut = -1;
			lireCaractere();

		}
		else
		{
			lex.UL = ERR1;
			lex.attribut = -1;
		}
		break;
	case '|':
		lireCaractere();
		if (c == '|')
		{
			lex.UL = OU;
			lex.attribut = -1;
			lireCaractere();
		}
		else
		{
			lex.UL = ERR2;
			lex.attribut = -1;
		}
		break;
	case ':':
		lex.UL = DXPOINT;
		lex.attribut = -1;
		lireCaractere();
		break;
	default:
		if (estLettre(c)) {
			lexeme = c;

			lireCaractere();
			while (estLettre(c) || estChiffre(c) || c == '_')
			{
				//aj
				lexeme += c;
				lireCaractere();
			}
			if (estMotCle(lexeme)) {
				lex.UL = MOTCLE;
				lex.attribut = tableMotCle.find(enMiniscule(lexeme))->second;
			}
			else {
				lex.UL = IDENT;
				lex.attribut = indexIdentifiant(lexeme);
			}

		}
		else if (estChiffre(c))
		{

			//aj
			lexeme = c; // remplissage du tableau lexeme ( concatenation )
			lireCaractere();
			while (estChiffre(c))
			{
				lexeme += c;// remplissage du tableau lexeme ( concatenation )
				lireCaractere();
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
	}//switch(c)
	return lex;
}//uniteSuivante()


long AnalyseurLexical::indexIdentifiant(string chaine)
{
	string mot = enMiniscule(chaine);
	long index = hashCode(mot);
	tableIdent[index] = mot;

	return index;
}



bool AnalyseurLexical::estMotCle(string chaine)
{
	string mot = enMiniscule(chaine);
	if (tableMotCle.find(mot) != tableMotCle.end())
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
		lireCaractere();
		while (!codeEstFini() && c != '}')
		{
			lireCaractere();
		}
		if (c == '}')
			return true;

	}
	return false;
}

bool AnalyseurLexical::codeEstFini()
{

	return code.size() == 0 || code[0] == EOF;
}

void AnalyseurLexical::afficherTableMotsCle()
{
	cout << endl << endl << "TABLE DES MOTS CLES" << endl;
	cout << "======================================" << endl << endl;
	for (auto elem : tableMotCle)
	{
		std::cout << elem.second << "\t" << elem.first << endl;
	}
}

void AnalyseurLexical::afficherTableIdentificateurs()
{
	cout << endl << endl << "TABLE DES IDENTIFICATEURS" << endl;
	cout << "======================================" << endl << endl;
	for (map<long, string>::iterator elem = tableIdent.begin(); elem != tableIdent.end(); elem++)
	{
		std::cout << elem->first << "\t" << elem->second << endl;
	}
}
