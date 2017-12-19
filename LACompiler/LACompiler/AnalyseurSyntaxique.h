#pragma once
#include <string>
#include <iostream>

using namespace std;

class AnalyseurSyntaxique
{

	const string NON_TERMINAUX[][3] = {
		{"programme","debut",""}
									};//Matrice contenant pour chaque non terminal, son premier et son deuxième.
public:
	AnalyseurSyntaxique();
	virtual ~AnalyseurSyntaxique();
};

