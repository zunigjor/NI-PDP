#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <map>
#include <queue>
#include <unordered_map>

namespace fs = std::filesystem;
using namespace std;

class GraphEdge {
public:
    pair<int, int> vertices;
    int weight;
    bool used;

    GraphEdge(int u, int v, int weight){
        this->vertices.first = u;
        this->vertices.second = v;
        this->weight = weight;
        this->used = true;
    }

    bool operator < (const GraphEdge & other){
        return this->weight < other.weight;
    }

    friend ostream & operator << (ostream & os, const GraphEdge & ge){
        os << "{(" << ge.vertices.first << ", " << ge.vertices.second << ") " << ge.weight << " " << ge.used << "}";
        return os;
    }
};

class SolutionState {
public:
    int cost = 0;
    int num_of_vertices = 0;
    vector<GraphEdge> graph_edges;
    enum color_t {none, red, blue};
    unordered_map<int, color_t> color;

    SolutionState(int num_of_vertices, vector<GraphEdge> graph_edges){
        this->num_of_vertices = num_of_vertices;
        this->graph_edges = std::move(graph_edges);
        this->sortEdgesByWeightDesc();
    }

    void sortEdgesByWeightDesc(){
        sort(this->graph_edges.rbegin(), this->graph_edges.rend());
    }

    void useNone(){
        for (auto & graph_edge : this->graph_edges)
            graph_edge.used = false;
    }

    int calculateCost(){
        int weight_sum = 0;
        for (const auto & edge : graph_edges)
            if (edge.used)
                weight_sum += edge.weight;
        return weight_sum;
    }

    bool isBipartite(){
        unordered_map<int, vector<int>> adj_map;
        for (auto const & edge : this->graph_edges)
            if (edge.used){
                adj_map[edge.vertices.first].push_back(edge.vertices.second);
                adj_map[edge.vertices.second].push_back(edge.vertices.first);
            }
        for (auto & x : adj_map)
            this->color[x.first] = none;
        queue<pair<int, color_t>> q;
        for (auto & [vertex, _] : adj_map){
            if (this->color[vertex] == none){
                this->color[vertex] = red;
                q.emplace(vertex, this->color[vertex]);
                while(!q.empty()){
                    pair<int, color_t> p = q.front();
                    q.pop();
                    int v = p.first;
                    color_t c = p.second;
                    for (auto & j : adj_map[v]){
                        if (this->color[j] == c)
                            return false;
                        if (this->color[j] == none){
                            this->color[j] = (c == red) ? blue : red;
                            q.emplace(j, this->color[j]);
                        }
                    }
                }
            }
        }
        return true;
    }

    ostringstream printResult(){
        vector<int> reds;
        vector<int> blues;
        for (auto & vertex : this->color){
            if(vertex.second == red)
                reds.push_back(vertex.first);
            else
                blues.push_back(vertex.first);
        }
        ostringstream oss;
        oss << "red: {";
        for (auto i = reds.begin(); i != reds.end(); i++) {
            oss << *i;
            if (i + 1 != reds.end()) oss << ", ";
        }
        oss << "}" << endl;
        oss << "blue: {";
        for (auto i = blues.begin(); i != blues.end(); i++) {
            oss << *i;
            if (i + 1 != blues.end()) oss << ", ";
        }
        oss << "}" << endl;
        oss << "w_sum = " << this->calculateCost() << endl;
        return oss;
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
    cout << "   -h, --help              Výpíše tuto zprávu." << endl;
    cout << "   --file <filepath>       Spustí program pro soubor <filepath>." << endl;
    cout << "   --folder <folderpath>   Spustí program pro soubory ve složce <folderpath>." << endl;
    cout << "Parametry jsou vyhodnocvány v pořadí." << endl;
    cout << "Jorge Zuňiga 2023" << endl;
}

vector<SolutionState> readFromFile(const string & input_file_path){
    vector<SolutionState> inputs;
    cout << "Input file path: " << input_file_path << endl;
    if ( ! fs::exists(fs::path(input_file_path))){
        cout << "File \"" << input_file_path << "\" does not exist" << endl;
        return inputs;
    }
    ifstream inputFile(input_file_path);
    if (!inputFile.is_open()){
        cout << "Error opening " << input_file_path << endl;
        return inputs;
    }
    // Input variables
    int num_of_vertices;
    vector<GraphEdge> graph_edges;
    // Read input
    inputFile >> num_of_vertices;
    for (int row = 0; row < num_of_vertices; row++){
        for (int column = 0; column < num_of_vertices; column++){
            int weight;
            inputFile >> weight;
            if (column > row and weight >= 80 and weight <= 120){
                GraphEdge newEdge(row, column, weight);
                graph_edges.push_back(newEdge);
            }
        }
    }
    // Create class
    SolutionState inputState (num_of_vertices, graph_edges);
    // Close file, add to solutions.
    inputFile.close();
    inputs.push_back(inputState);
    return inputs;
}

vector<SolutionState> readFromFolder(const string & input_folder_path){
    vector<SolutionState> inputs;
    cout << "Input folder path: " << input_folder_path << endl;
    if ( ! fs::is_directory(fs::path(input_folder_path))){
        cout << "Folder \"" << input_folder_path << "\" does not exist" << endl;
        return inputs;
    }
    vector<string> paths_to_files;
    for (const auto & entry : fs::directory_iterator(input_folder_path)){
        paths_to_files.push_back(entry.path());
    }
    sort(paths_to_files.begin(), paths_to_files.end());
    for (const auto & file_path : paths_to_files){
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
    if (initialSolutionState.isBipartite()){
        cout << initialSolutionState << endl;
        cout << initialSolutionState.printResult().str();
    }
    initialSolutionState.useNone();

}

int main(int argc, char* argv[]) {
    vector<SolutionState> inputs = readInput(argc, argv);
    for (const auto & i : inputs) {
        solve(i);
    }
    return 0;
}
