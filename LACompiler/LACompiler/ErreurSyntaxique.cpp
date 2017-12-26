#include "stdafx.h";
#include "ErreurSyntaxique.h";
#include <iostream>;
#include <string>;
#include <string.h>;

using namespace std;

ErreurSyntaxique::ErreurSyntaxique()
{
	nombreErreur = 0;
}




ErreurSyntaxique::~ErreurSyntaxique()
{
}

void ErreurSyntaxique::ajouterErreur(string erreur)
{
	erreurs[nombreErreur++] = erreur;
}

void ErreurSyntaxique::afficherErreurs()
{
	for (int i = 0; i < nombreErreur; i++)
		cout << erreurs[i] << endl;
}

int ErreurSyntaxique::getNombreErreur()
{
	return ErreurSyntaxique::nombreErreur;
}
