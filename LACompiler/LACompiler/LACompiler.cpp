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
		if (argc > 1)
		{
			string fichier = argv[1];

			AnalyseurSyntaxique *analyseurSyntaxique = new AnalyseurSyntaxique(fichier);

			analyseurSyntaxique->verifierSyntaxe();
		}
		else

			cout << "Veuiller indiquer le scripte a compiler\n";

	}
	catch (const std::exception& e)
	{
		cout << e.what();
	}

	return 0;
}

