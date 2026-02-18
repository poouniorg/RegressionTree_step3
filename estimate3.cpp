// estimate3.cpp
#include <iostream>
#include<fstream>
#include "node.h"
using namespace std;

// Il faut une estimation maximale du nombre de nœuds dans l'arbre.
const int MAX_NODES = 32;
Node tree[MAX_NODES];


// Lit l'arbre à partir d'un fichier au format: node_id,left_id,right_id,condition_or_value
bool read_tree(char* filename) {
    for (int i = 0; i < MAX_NODES; ++i) // Initialiser tous les nœuds à null
        tree[i] = Node();

    ifstream fp(filename);
    if (!fp) return 0.0; // ou gérer les erreurs

    char line[256];
    int node_id, left_id, right_id;
    char cond_val[128];
    while (fp.getline(line, sizeof(line))) {
        if (sscanf(line, "%d,%d,%d,%127[^\n]", &node_id, &left_id, &right_id, cond_val) != 4) 
            return false; // signaler une erreur
        // remplir le nœud
        tree[node_id].set_children(left_id, right_id);
        if (left_id == -1 && right_id == -1) {
            // Nœud feuille : le reste est une valeur
            tree[node_id].mark_as_leaf();
            tree[node_id].set_value(atof(cond_val));
        } else {
            // Nœud interne : cond_val est une condition
            if (!tree[node_id].parse_condition(cond_val)) {
                cerr << "Error: Failed to parse condition: " << cond_val << endl;
                fp.close();
                return false;
            }
        }
    }
    fp.close();
    return true;
}

// Fonction d'inférence utilisant array 
float estimate(float features[FEATURE_COUNT]) {
    int idx = 1; // commencer à la racine

    while (idx < MAX_NODES) {
        if (tree[idx].test_leaf()) {
            return tree[idx].get_value();
        }
        bool go_left = tree[idx].eval_condition(features);
        idx = go_left ? tree[idx].get_left() : tree[idx].get_right();
    }

    return 0.0; // une fonction non void doit toujours retourner
}


// Main program
int main() {
    char choice;
    float age, weight_kg, height_cm, waiting_time;

    cout << "=== BMD Estimator (Based on Trained Regression tree) ===\n\n";

    // Read tree from file
    if (!read_tree("bmd_tree_transition.txt")) {
        cerr << "Failed to load tree. Exiting.\n";
        return 1;
    }

    do {
        cout << "Enter patient details:\n";
        cout << "Age (years): ";
        cin >> age;
        cout << "Weight (kg): ";
        cin >> weight_kg;
        cout << "Height (cm): ";
        cin >> height_cm;
        cout << "Waiting time (days): ";
        cin >> waiting_time;

        // Map to feature array in enum order
        float features[FEATURE_COUNT] = {
            weight_kg,     // WEIGHT_KG = 0
            age,           // AGE = 1
            height_cm,     // HEIGHT_CM = 2
            waiting_time   // WAITING_TIME = 3
        };

        float bmd = estimate(features);
        cout << "\n--> Predicted BMD: " << bmd << "\n\n";

        cout << "Estimate another patient? (y/n): ";
        cin >> choice;
        cout << "\n";

    } while (choice == 'y' || choice == 'Y');

    cout << "Thank you for using the BMD estimator!\n";
    return 0;
}   
