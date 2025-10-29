#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_snap>\n";
        return 1;
    }
    ifstream in(argv[1]);
    if (!in) {
        cerr << "Erro ao abrir arquivo: " << argv[1] << "\n";
        return 1;
    }

    unordered_set<uint64_t> seen;
    vector<uint64_t> ids;
    ids.reserve(1000000);
    uint64_t u, v;

    string line;
    while (getline(in, line)) {
        if (line.size() == 0 || line[0] == '#') continue;
        stringstream ss(line);
        if (!(ss >> u >> v)) continue;
        if (seen.insert(u).second) ids.push_back(u);
        if (seen.insert(v).second) ids.push_back(v);
    }

    for (uint64_t x : ids) cout << x << "\n";
    return 0;
}
