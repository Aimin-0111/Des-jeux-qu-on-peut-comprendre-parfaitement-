#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <fstream>

#ifdef _WIN32
    #pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

using namespace std;

enum Etat { GAGNANT, PERDANT, NUL, INCONNU };

struct Graphe {
    int n, k, depart, arrivee;
    vector<vector<int>> adj;
    vector<vector<int>> dest_k;      // Sommets atteignables à distance k
    vector<vector<int>> pred_k;      // Sommets pouvant atteindre u à distance k
    vector<Etat> etats;
    vector<int> sorties_vers_inconnu; // Combien de coups ne mènent pas encore à G

    Graphe(int _n, int _k, int _dep, int _arr) : n(_n), k(_k), depart(_dep), arrivee(_arr) {
        adj.resize(n);
        dest_k.resize(n);
        pred_k.resize(n);
        etats.assign(n, INCONNU);
        sorties_vers_inconnu.assign(n, 0);
    }

    void ajouterArc(int u, int v) { adj[u].push_back(v); }

    // BFS pour trouver tous les sommets à distance EXACTE k
    void calculerDestinationsK(int start, set<int>& final_dest) {
        vector<int> dist(n, -1);
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            if (dist[u] == k) {
                final_dest.insert(u);
                continue;
            }
            
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    void preparer() {
        for (int i = 0; i < n; ++i) {
            set<int> dests;
            calculerDestinationsK(i, dests);
            for (int d : dests) {
                dest_k[i].push_back(d);
                pred_k[d].push_back(i);
            }
            sorties_vers_inconnu[i] = dest_k[i].size();
        }
    }
};

void resoudre(Graphe& g) {
    queue<int> q;

    // Initialisation : L'arrivée est GAGNANT (celui qui y arrive perd)
    // Les sommets sans issue (à distance k) sont NUL (ne peut pas jouer)
    for (int i = 0; i < g.n; ++i) {
        if (i == g.arrivee) {
            g.etats[i] = GAGNANT;
            q.push(i);
        }
        else if (g.dest_k[i].empty()) {
            g.etats[i] = NUL;  // Can't move k steps = draw
        }
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : g.pred_k[v]) {
            if (g.etats[u] != INCONNU) continue;

            if (g.etats[v] == PERDANT) {
                // Si u peut aller vers une position perdante, u est GAGNANT
                g.etats[u] = GAGNANT;
                q.push(u);
            } 
            else if (g.etats[v] == GAGNANT) {
                // Si u va vers un GAGNANT, on décrémente son nombre de sorties "utiles"
                g.sorties_vers_inconnu[u]--;
                // Si TOUTES les sorties de u mènent à des positions GAGNANTES, u est PERDANT
                if (g.sorties_vers_inconnu[u] == 0) {
                    g.etats[u] = PERDANT;
                    q.push(u);
                }
            }
        }
    }

    // Tout ce qui reste INCONNU est NUL (boucles infinies)
    for (int i = 0; i < g.n; ++i) if (g.etats[i] == INCONNU) g.etats[i] = NUL;
}

int main() {
    ifstream infile("input.txt");
    if (!infile.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir input.txt" << endl;
        return 1;
    }
    
    int n, m, k, dep, arr;
    if (!(infile >> n >> m >> k >> dep >> arr)) {
        cerr << "Erreur : Format d'entree invalide" << endl;
        return 1;
    }
    
    if (n < 1 || m < 0 || k < 1 || dep < 0 || dep >= n || arr < 0 || arr >= n) {
        cerr << "Erreur : Parametres invalides" << endl;
        return 1;
    }

    Graphe g(n, k, dep, arr);
    for (int i = 0; i < m; ++i) {
        int u, v;
        if (!(infile >> u >> v)) {
            cerr << "Erreur : Arc numero " << i+1 << " mal forme" << endl;
            return 1;
        }
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cerr << "Erreur : Sommet " << u << " ou " << v << " hors limites" << endl;
            return 1;
        }
        g.ajouterArc(u, v);
    }

    g.preparer();
    resoudre(g);

    cout << "graph LR" << endl;
    for (int i = 0; i < n; ++i) {
        string res = (g.etats[i] == GAGNANT) ? "G" : (g.etats[i] == PERDANT ? "P" : "N");
        string color = (g.etats[i] == GAGNANT) ? "#C5E1A5" : (g.etats[i] == PERDANT ? "#EF9A9A" : "#BDBDBD");
        cout << "  n" << i << "[\" " << res << " \"]" << endl;
        cout << "  style n" << i << " fill:" << color << endl;
    }
    for (int u = 0; u < n; u++) for (int v : g.adj[u]) cout << "  n" << u << " --> n" << v << endl;
    
    cout << endl << "%% Resultat au depart (Sommet " << g.depart << "): "
         << (g.etats[g.depart] == GAGNANT ? "VICTOIRE" : (g.etats[g.depart] == PERDANT ? "DEFAITE" : "NUL")) << endl;

    return 0;
}
