#ifndef COMPTEBANCAIRE_H
#define COMPTEBANCAIRE_H
#include <vector>
#include <string>

using namespace std;

// Class compte bancaire
class CompteBancaire
{
    // attribut de classe privée
    private:
        string numeroCompte;
        string nomclient;
        double solde;
        bool verrouille;
        vector<string> transactions;
    // methode de classe
    public:
        CompteBancaire(string num, string nom, double soldeInitial); // Constructeur de la classe CompteBancaire
        void depot(double montant);
        void retrait(double montant);
        void verrouiller();
        void deverrouiller();
        void afficherReleve(const string& cheminFichier) const;

        string getNumeroCompte() const;
        string getNomClient() const;
        double getSolde() const;
        bool isverrouille() const;
        string toString() const;

};

#endif // COMPTEBANCAIRE_H
