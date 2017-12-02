#pragma once
typedef stuct{
	int attribut;
}TLexeme;
class AnalyseurLexical
{
	public:
		AnalyseurLexical();
		~AnalyseurLexical();
		Tlexeme lexemeSuivant();
		int indexIdentifiant();
		int indexMotCle();
		bool estMotCle();
};

