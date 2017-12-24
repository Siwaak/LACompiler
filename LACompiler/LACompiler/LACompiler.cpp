// LACompiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h";
#include <iostream>;
#include "AnalyseurSyntaxique.h"
//#include "AnalyseurLexical.h";
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	//Gestion des exceptions
	try
	{
		if (argc > 1) {
			string fichier = argv[1];
			AnalyseurSyntaxique *analyseurSyntaxique = new AnalyseurSyntaxique(fichier);
			//AnalyseurLexical *analysLexic = new AnalyseurLexical(fichier);
			//while (!analysLexic->codeEstFini())
			//{
			//	TLexeme lex = analysLexic->uniteSuivante();
			//	if(lex.attribut != -1)
			//		cout << lex.UL << "(" << lex.attribut << ")   ";
			//	else cout << lex.UL << "   ";
			//}

			////Affichage des mots clés
			//analysLexic->afficherTableMotsCle();

			////affichage des identificateurs
			//analysLexic->afficherTableIdentificateurs();

			analyseurSyntaxique->verifierSyntaxe();
		}
	}
	catch (const std::exception& e)
	{
		cout << e.what();
	}

	return 0;
}

