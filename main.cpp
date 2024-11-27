#include <iostream>
#include <regex>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Banque.h"
#include "Database.h"

using namespace std;

// Fonction pour générer un numéro de compte aléatoire
string genererNumeroCompte() {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string numero;
    srand(time(0)); // Initialisation de la graine pour la génération aléatoire
    for(int i = 0; i < 10; ++i) {
        numero += chars[rand() % chars.size()];
    }
    return numero;
}

// Vérifier si le nom est valide (lettres et espaces uniquement)
bool verifierNom(const string& nom) {
    regex pattern("^[a-zA-Z ]+$");
    return regex_match(nom, pattern);
}

// Vérifier si le solde est valide (doit être positif)
bool verifierSolde(double solde) {
    return solde > 0.0;
}

// Vérifier si l'option entrée par l'utilisateur est valide
bool verifierChoix(int choix) {
    return choix >= 1 && choix <= 5;
}

// Vérifier si le numéro de compte est valide (exactement 5 caractères)
bool verifierNumeroCompte(const string& numero) {
    return numero.length() == 5;
}

// Afficher le menu principal pour la banque
void afficherMenuBanque() {
    cout << "**********************************************************\n";
    cout << "*                 MENU DE LA BANQUE                     *\n";
    cout << "* 1. Ajouter un nouveau compte                           *\n";
    cout << "* 2. Rechercher un compte par ID                         *\n";
    cout << "* 3. Afficher tous les comptes                           *\n";
    cout << "* 4. Exporter les comptes dans un fichier texte          *\n";
    cout << "* 5. Quitter                                             *\n";
    cout << "**********************************************************\n";
    cout << "Veuillez choisir une option (1-5) : ";
}

// Afficher le menu principal pour l'utilisateur
void afficherMenuUtilisateur() {
    cout << "**********************************************************\n";
    cout << "*                 MENU DE L'UTILISATEUR                  *\n";
    cout << "* 1. Créer un nouveau compte                             *\n";
    cout << "* 2. Afficher mes informations de compte                 *\n";
    cout << "* 3. Exporter mes informations dans un fichier texte     *\n";
    cout << "* 4. Quitter                                             *\n";
    cout << "**********************************************************\n";
    cout << "Veuillez choisir une option (1-4) : ";
}

// Demander et vérifier le mot de passe de la banque
bool verifierMotDePasseBanque() {
    const string motDePasseCorrect = "Banque123";  // Exemple de mot de passe
    string motDePasse;

    cout << "Veuillez entrer le mot de passe de la banque : ";
    cin >> motDePasse;

    return motDePasse == motDePasseCorrect;
}

// Exporter les comptes dans un fichier texte
void exporterComptes(Database& db) {
    ofstream fichier("comptes_exportes.txt");

    if (!fichier) {
        cout << "Erreur d'ouverture du fichier pour l'exportation." << endl;
        return;
    }

    string sql = "SELECT Numero, Nom, Solde, Verrouille FROM Comptes;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db.getDb(), sql.c_str(), -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string numero = (char*)sqlite3_column_text(stmt, 0);
        string nom = (char*)sqlite3_column_text(stmt, 1);
        double solde = sqlite3_column_double(stmt, 2);
        bool verrouille = sqlite3_column_int(stmt, 3) == 1;

        fichier << "Numero : " << numero << endl;
        fichier << "Nom : " << nom << endl;
        fichier << "Solde : " << solde << " FRCFA" << endl;
        fichier << "Verrouille : " << (verrouille ? "Oui" : "Non") << endl;
        fichier << "--------------------------------------------" << endl;
    }

    sqlite3_finalize(stmt);
    fichier.close();
    cout << "Les informations ont été exportées dans 'comptes_exportes.txt'." << endl;
}

int main() {
    Database db("BanqueDB.db");

    // Demander si l'utilisateur est une banque ou un utilisateur
    string typeUtilisateur;
    cout << "Êtes-vous une banque ou un utilisateur ? (Entrez 'banque' ou 'utilisateur') : ";
    cin >> typeUtilisateur;

    // Vérification si l'utilisateur est une banque ou un utilisateur
    if (typeUtilisateur == "banque") {
        // Vérification du mot de passe de la banque
        if (!verifierMotDePasseBanque()) {
            cout << "Mot de passe incorrect. Accès refusé." << endl;
            return 1;
        }

        // Menu de la banque
        int choixBanque;
        do {
            afficherMenuBanque();
            cin >> choixBanque;

            while (!verifierChoix(choixBanque)) {
                cout << "Option invalide. Veuillez choisir entre 1 et 5 : ";
                cin >> choixBanque;
            }

            switch (choixBanque) {
                case 1: {  // Ajouter un nouveau compte
                    string nom;
                    double soldeInitial;
                    // Vérifier et demander le nom
                    do {
                        cout << "Entrer le nom du client (lettres et espaces uniquement) : ";
                        cin.ignore();
                        getline(cin, nom);
                        if (!verifierNom(nom)) {
                            cout << "Nom invalide. Veuillez réessayer." << endl;
                        }
                    } while (!verifierNom(nom));

                    // Vérifier et demander le solde initial
                    do {
                        cout << "Entrer le solde initial : ";
                        cin >> soldeInitial;
                        if (!verifierSolde(soldeInitial)) {
                            cout << "Solde invalide. Veuillez réessayer." << endl;
                        }
                    } while (!verifierSolde(soldeInitial));

                    // Générer le numéro de compte
                    string numero = genererNumeroCompte();
                    cout << "Numéro de compte généré : " << numero << endl;

                    // Créer et ajouter le compte à la base de données
                    CompteBancaire compte(numero, nom, soldeInitial);
                    db.ajouterCompte(compte);
                    cout << "Compte créé avec succès !" << endl;
                    break;
                }
                case 2: {  // Rechercher un compte par ID
                    string numero;
                    cout << "Entrer le numéro du compte à rechercher : ";
                    cin >> numero;

                    // Vérification du numéro de compte
                    while (!verifierNumeroCompte(numero)) {
                        cout << "Numéro de compte invalide. Il doit contenir 5 caractères. Veuillez réessayer : ";
                        cin >> numero;
                    }

                    db.rechercherCompteParId(numero);  // Rechercher et afficher le compte
                    break;
                }
                case 3: {  // Afficher tous les comptes
                    db.afficherTousLesComptes();
                    break;
                }
                case 4: {  // Exporter les comptes vers un fichier texte
                    exporterComptes(db);
                    break;
                }
                case 5: {  // Quitter
                    cout << "Au revoir !" << endl;
                    break;
                }
                default: {
                    cout << "Option invalide. Veuillez réessayer." << endl;
                    break;
                }
            }

        } while (choixBanque != 5);  // Répéter jusqu'à ce que la banque choisisse de quitter

    } else if (typeUtilisateur == "utilisateur") {
        // Menu de l'utilisateur
        int choixUtilisateur;
        do {
            afficherMenuUtilisateur();
            cin >> choixUtilisateur;

            while (choixUtilisateur < 1 || choixUtilisateur > 4) {
                cout << "Option invalide. Veuillez choisir entre 1 et 4 : ";
                cin >> choixUtilisateur;
            }

            switch (choixUtilisateur) {
                case 1: {  // Créer un nouveau compte
                    string nom;
                    double soldeInitial;
                    // Vérifier et demander le nom
                    do {
                        cout << "Entrer votre nom (lettres et espaces uniquement) : ";
                        cin.ignore();
                        getline(cin, nom);
                        if (!verifierNom(nom)) {
                            cout << "Nom invalide. Veuillez réessayer." << endl;
                        }
                    } while (!verifierNom(nom));

                    // Vérifier et demander le solde initial
                    do {
                        cout << "Entrer le solde initial : ";
                        cin >> soldeInitial;
                        if (!verifierSolde(soldeInitial)) {
                            cout << "Solde invalide. Veuillez réessayer." << endl;
                        }
                    } while (!verifierSolde(soldeInitial));

                    // Générer le numéro de compte
                    string numero = genererNumeroCompte();
                    cout << "Votre numéro de compte est : " << numero << endl;

                    // Créer et ajouter le compte à la base de données
                    CompteBancaire compte(numero, nom, soldeInitial);
                    db.ajouterCompte(compte);
                    cout << "Compte créé avec succès !" << endl;
                    break;
                }
                case 2: {  // Afficher les informations de son compte
                    string numero;
                    cout << "Entrer votre numéro de compte : ";
                    cin >> numero;

                    db.rechercherCompteParId(numero);  // Rechercher et afficher le compte
                    break;
                }
                case 3: {  // Exporter ses informations vers un fichier texte
                    string numero;
                    cout << "Entrer votre numéro de compte : ";
                    cin >> numero;

                    db.exporterCompte(numero);  // Exporter les informations du compte dans un fichier
                    break;
                }
                case 4: {  // Quitter
                    cout << "Au revoir !" << endl;
                    break;
                }
            }

        } while (choixUtilisateur != 4);  // Répéter jusqu'à ce que l'utilisateur choisisse de quitter

    } else {
        cout << "Entrée invalide. Le programme va maintenant se fermer." << endl;
    }

    return 0;
}
