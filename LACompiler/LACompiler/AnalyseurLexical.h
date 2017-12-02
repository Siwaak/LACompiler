#pragma once
typedef struct{
	int attribut;
}TLexeme;
class AnalyseurLexical
{
	enum TuniteLexical {
		ERREUR, OPPLUS,COMMENTAIRE, VIRGULE, POINTVIRGULE, EGAL, AFFECTATION, PLUS, MOINS, MULTIPLICATION,DIVISION,
		MODULO
	};

	public:
		AnalyseurLexical();
		~AnalyseurLexical();

		bool estBlanc(char);
		char lireCaractere();
		int lexemeSuivant();
		int indexIdentifiant();
		int indexMotCle();
		bool estMotCle();
};

