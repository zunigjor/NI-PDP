#include <iostream>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;
using namespace std;

class GraphEdge {
public:
    set<int> vertices;
    int weight;
    bool used;

    GraphEdge(int u, int v, int weight){
        this->vertices.insert(u);
        this->vertices.insert(v);
        this->weight = weight;
        this->used = false;
    }

    friend ostream & operator << (ostream & os, const GraphEdge & ge){
        os << "{(";
        for (auto i = ge.vertices.begin(); i != ge.vertices.end(); i++){
            os << *i;
            if (next(i) != ge.vertices.end()) os << ", ";
        }
        os << ") ";
        os << ge.weight << " ";
        os << ge.used;
        os << "}";
        return os;
    }
};

class SolutionState {
public:
    int cost = 0;
    int num_of_vertices = 0;
    vector<GraphEdge> graph_edges;
    map<int, bool> vertex_colors;

    void calculateCost(){
        int weight_sum = 0;
        for (const auto & edge : graph_edges)
            if (edge.used)
                weight_sum += edge.weight;
        this->cost = weight_sum;
    }

    friend ostream & operator << (ostream & os, const SolutionState & s){
        os << s.num_of_vertices << ", " << s.graph_edges.size() << ":" << endl;
        for (const auto & graph_edge : s.graph_edges){
            os << graph_edge << endl;
        }
        return os;
    }
};

void printHelp(){
    cout << "NI-PDP sekvenční řešení úkolu pro sudé paralelky." << endl;
    cout << "Parametry:" << endl;
    cout << "   -h, --help   Výpíše tuto zprávu." << endl;
    cout << "   --file <filepath>   Spustí program pro soubor <filepath>." << endl;
    cout << "   --folder <folderpath>   Spustí program pro soubory ve složce <folderpath>." << endl;
    cout << "Parametry jsou vyhodnocvány v pořadí. Tedy pokud je přítomen -h, už se neprovádí --file a --folder." << endl;
    cout << "Jorge Zuňiga 2023" << endl;
}

vector<SolutionState> readFromFile(const string & input_file_path){
    cout << "Input file path: " << input_file_path << endl;
    vector<SolutionState> inputs;

    ifstream inputFile(input_file_path);
    if (!inputFile.is_open()){
        cout << "Error opening " << input_file_path << endl;
        return inputs;
    }

    SolutionState inputState;
    inputFile >> inputState.num_of_vertices;
    for (int row = 0; row < inputState.num_of_vertices; row++){
        for (int column = 0; column < inputState.num_of_vertices; column++){
            int weight;
            inputFile >> weight;
            if (column > row and weight >= 80 and weight <= 120){
                GraphEdge newEdge(row, column, weight);
                inputState.graph_edges.push_back(newEdge);
            }
        }
    }

    inputFile.close();
    inputs.push_back(inputState);
    return inputs;
}

vector<SolutionState> readFromFolder(const string & input_folder_path){
    cout << "Input folder path: " << input_folder_path << endl;
    vector<SolutionState> inputs;
    vector<string> paths_to_files;
    for (const auto & entry : fs::directory_iterator(input_folder_path)){
        paths_to_files.push_back(entry.path());
    }
    sort(paths_to_files.begin(), paths_to_files.end());
    for (auto file_path : paths_to_files){
        vector<SolutionState> input = readFromFile(file_path);
        inputs.insert(inputs.end(), input.begin(), input.end());
    }
    return inputs;
}

vector<SolutionState> readInput(int argc, char* argv[]){
    vector<SolutionState> inputs;
    vector<string> args(argv + 1, argv+argc);
    // Print help if -h, --help or no args provided
    bool help_flag = find(args.begin(), args.end(), "-h") != args.end();
    help_flag = help_flag or find(args.begin(), args.end(), "--help") != args.end();
    if ( help_flag or args.empty()){
        printHelp();
        return inputs;
    }
    // --file <filepath>
    auto file_flag_iterator = find(args.begin(), args.end(), "--file");
    if (file_flag_iterator != args.end()){
        string input_file_path;
        if (next(file_flag_iterator) != args.end()) {
            input_file_path = *next(file_flag_iterator);
        } else {
            cout << "No input file provided. Exiting." << endl;
            return inputs;
        }
        inputs = readFromFile(input_file_path);
        return inputs;
    }
    // --folder <folderpath>
    auto folder_flag_iterator = find(args.begin(), args.end(), "--folder");
    if (folder_flag_iterator != args.end()){
        string input_folder_path;
        if (next(folder_flag_iterator) != args.end()) {
            input_folder_path = *next(folder_flag_iterator);
        } else {
            cout << "No input folder provided. Exiting." << endl;
            return inputs;
        }
        inputs = readFromFolder(input_folder_path);
        return inputs;
    }
    return inputs;
}

void solve(SolutionState initialSolutionState){

}

int main(int argc, char* argv[]) {
    vector<SolutionState> inputs = readInput(argc, argv);
    for (const auto & i : inputs) {
        solve(i);
    }
    return 0;
}
