#pragma once
#include <iostream>;

using namespace std;


class ErreurSyntaxique
{
	string erreurs[1000];
	int nombreErreur;
public:
	ErreurSyntaxique();
	virtual ~ErreurSyntaxique();
	void ajouterErreur(string);
	void afficherErreurs();
	int getNombreErreur();
};

