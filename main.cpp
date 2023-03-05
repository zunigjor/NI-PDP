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
#include <chrono>

namespace fs = std::filesystem;
using namespace std;

unsigned int recursive_calls = 0;

enum color_t {NO_COLOR, RED, BLUE};

class Edge {
public:
    pair<int, int> vertices;
    int weight;
    bool used;

    Edge(int u, int v, int weight){
        this->vertices.first = u;
        this->vertices.second = v;
        this->weight = weight;
        this->used = true;
    }

    bool operator < (const Edge & other){
        return this->weight < other.weight;
    }

    friend ostream & operator << (ostream & os, const Edge & ge){
        os << "{(" << ge.vertices.first << ", " << ge.vertices.second << "), " << ge.weight << "}";
        return os;
    }
};

class SolutionState {
public:
    string input_name;
    int cost;
    int num_of_vertices;
    int edge_index;
    vector<Edge> edges;
    unordered_map<int, color_t> color;

    SolutionState(string input_name, int num_of_vertices, vector<Edge> graph_edges){
        this->input_name = std::move(input_name);
        this->cost = 0;
        this->num_of_vertices = num_of_vertices;
        this->edge_index = 0;
        this->edges = std::move(graph_edges);
        this->sortEdgesByWeightDesc();
    }

    void sortEdgesByWeightDesc(){
        sort(this->edges.rbegin(), this->edges.rend());
    }

    void resetSolution(){
        for (auto & graph_edge : this->edges)
            graph_edge.used = false;
        for (auto & vertex : color)
            vertex.second = NO_COLOR;
    }

    int numOfUsedEdges(){
        int num = 0;
        for (auto & edge : this->edges)
            if (edge.used) num++;
        return num;
    }

    bool isBipartite(){
        unordered_map<int, vector<int>> adj_map;
        for (auto const & edge : this->edges)
            if (edge.used){
                adj_map[edge.vertices.first].push_back(edge.vertices.second);
                adj_map[edge.vertices.second].push_back(edge.vertices.first);
            }
        for (auto & x : adj_map){
            this->color[x.first] = NO_COLOR;
        }
        queue<pair<int, color_t>> q;
        for (auto & [vertex, _] : adj_map){
            if (this->color[vertex] == NO_COLOR){
                this->color[vertex] = RED;
                q.emplace(vertex, this->color[vertex]);
                while(!q.empty()){
                    pair<int, color_t> p = q.front();
                    q.pop();
                    int v = p.first;
                    color_t c = p.second;
                    for (auto & j : adj_map[v]){
                        if (this->color[j] == c)
                            return false;
                        if (this->color[j] == NO_COLOR){
                            this->color[j] = (c == RED) ? BLUE : RED;
                            q.emplace(j, this->color[j]);
                        }
                    }
                }
            }
        }
        return true;
    }

    bool isConnected(){
        unordered_map<int, vector<int>> adj_map;
        for (auto const & edge : this->edges)
            if (edge.used){
                adj_map[edge.vertices.first].push_back(edge.vertices.second);
                adj_map[edge.vertices.second].push_back(edge.vertices.first);
            }
        vector<bool> visited(this->num_of_vertices);
        int v = 0;
        visited[v] = true;
        queue<int> q;
        q.push(v);
        while (!q.empty()){
            v = q.front();
            q.pop();
            for (auto & j : adj_map[v]){
                if (!visited[j]){
                    visited[j] = true;
                    q.push(j);
                }
            }
        }
        return all_of(visited.begin(), visited.end(), [](bool b){return b;});
    }

    string str(chrono::time_point<chrono::high_resolution_clock> start_time){
        ostringstream oss;
        auto end_time = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        oss << "============================================" << endl;
        oss << "Result of: " << this->input_name << endl;
        oss << "--------------------------------------------" << endl;
        vector<int> reds;
        vector<int> blues;
        for (auto & vertex : this->color){
            if(vertex.second == RED)
                reds.push_back(vertex.first);
            else
                blues.push_back(vertex.first);
        }
        oss << "RED: {";
        for (auto i = reds.begin(); i != reds.end(); i++) {
            oss << *i;
            if (i + 1 != reds.end()) oss << ", ";
        }
        oss << "}" << endl;
        oss << "BLUE: {";
        for (auto i = blues.begin(); i != blues.end(); i++) {
            oss << *i;
            if (i + 1 != blues.end()) oss << ", ";
        }
        oss << "}" << endl;
        for (auto & edge : this->edges) {
            if (edge.used)
                oss << edge << endl;
        }
        //this->calculateCost();
        oss << "w_sum = " << this->cost << endl;
        oss << "--------------------------------------------" << endl;
        auto s = chrono::duration_cast<chrono::seconds>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(s);
        auto m = chrono::duration_cast<chrono::minutes>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(m);
        auto h = chrono::duration_cast<chrono::hours>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(h);
        oss << "recursive calls: " << recursive_calls << endl;
        oss << "took: " << h.count() << "h:" << m.count() << "m:" << s.count() << "." << ms.count() << "s" << endl;
        oss << "============================================" << endl;
        return oss.str();
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
    cout << "Loaded file: " << input_file_path << endl;
    if ( ! fs::exists(fs::path(input_file_path))){
        cout << "File \"" << input_file_path << "\" does not exist" << endl;
        return inputs;
    }
    ifstream inputFile(input_file_path);
    if (!inputFile.is_open()){
        cout << "Error opening " << input_file_path << endl;
        return inputs;
    }
    int num_of_vertices;
    vector<Edge> graph_edges;
    inputFile >> num_of_vertices;
    for (int row = 0; row < num_of_vertices; row++){
        for (int column = 0; column < num_of_vertices; column++){
            int weight;
            inputFile >> weight;
            if (column > row and weight >= 80 and weight <= 120){
                Edge newEdge(row, column, weight);
                graph_edges.push_back(newEdge);
            }
        }
    }
    SolutionState inputState (input_file_path, num_of_vertices, graph_edges);
    inputFile.close();
    inputs.push_back(inputState);
    return inputs;
}

vector<SolutionState> readFromFolder(const string & input_folder_path){
    vector<SolutionState> inputs;
    cout << "Loading folder: " << input_folder_path << endl;
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

int sumWeightRemainingEdges(SolutionState state){
    int sum = 0;
    for (auto i = state.edges.begin() + state.edge_index; i != state.edges.end(); ++i) {
        sum += i->weight;
    }
    return sum;
}

int numOfRemainingEdges(const SolutionState& state){
    return int(state.edges.size()) - state.edge_index;
}

bool noBetterSolutionPossible(SolutionState parent_state, const SolutionState& best_state){
    if (parent_state.cost + sumWeightRemainingEdges(parent_state) < best_state.cost)
        return true;
    if (parent_state.numOfUsedEdges() + numOfRemainingEdges(parent_state) < parent_state.num_of_vertices - 1)
        return true;
    return false;
}

void skipEdge(SolutionState & s){
    s.edge_index++;
}

void addEdge(SolutionState & s){
    Edge & e = s.edges[s.edge_index];
    e.used = true;
    s.edge_index++;
    s.cost += e.weight;
}

color_t oppositeColor(color_t c){
    return (c == RED) ? BLUE : RED;
}

void findBestState(SolutionState parent_state, SolutionState & best_state){
    recursive_calls++;
    if (parent_state.edge_index >= int(parent_state.edges.size())){
        if (parent_state.isConnected() and parent_state.cost >= best_state.cost){
            best_state = parent_state;
            return;
        } else {
            return;
        }
    }
    if (noBetterSolutionPossible(parent_state, best_state)) {
        return;
    }
    int u = parent_state.edges[parent_state.edge_index].vertices.first;
    int v = parent_state.edges[parent_state.edge_index].vertices.second;
    if ((parent_state.color[u] == RED and parent_state.color[v] == RED) or
        (parent_state.color[u] == BLUE and parent_state.color[v] == BLUE)
       ){
        SolutionState opt_skip = parent_state;
        skipEdge(opt_skip);
        findBestState(opt_skip, best_state);
    } else if (parent_state.color[u] == NO_COLOR and parent_state.color[v] == NO_COLOR){
        {
            SolutionState opt_add_red_blue = parent_state;
            addEdge(opt_add_red_blue);
            opt_add_red_blue.color[u] = RED;
            opt_add_red_blue.color[v] = BLUE;
            findBestState(opt_add_red_blue, best_state);
        }
        {
            SolutionState opt_add_blue_red = parent_state;
            addEdge(opt_add_blue_red);
            opt_add_blue_red.color[u] = BLUE;
            opt_add_blue_red.color[v] = RED;
            findBestState(opt_add_blue_red, best_state);
        }
        {
            SolutionState opt_skip_red_red = parent_state;
            skipEdge(opt_skip_red_red);
            opt_skip_red_red.color[u] = RED;
            opt_skip_red_red.color[v] = RED;
            findBestState(opt_skip_red_red, best_state);
        }
        {
            SolutionState opt_skip_blue_blue = parent_state;
            skipEdge(opt_skip_blue_blue);
            opt_skip_blue_blue.color[u] = BLUE;
            opt_skip_blue_blue.color[v] = BLUE;
            findBestState(opt_skip_blue_blue, best_state);
        }
    } else if ((parent_state.color[u] == RED and parent_state.color[v] == NO_COLOR) or
               (parent_state.color[u] == BLUE and parent_state.color[v] == NO_COLOR)
              ){
        {
            SolutionState opt_add_opposite = parent_state;
            addEdge(opt_add_opposite);
            opt_add_opposite.color[v] = oppositeColor(opt_add_opposite.color[u]);
            findBestState(opt_add_opposite, best_state);
        }
        {
            SolutionState opt_skip_same = parent_state;
            skipEdge(opt_skip_same);
            opt_skip_same.color[v] = opt_skip_same.color[u];
            findBestState(opt_skip_same, best_state);
        }
    } else if ((parent_state.color[u] == NO_COLOR and parent_state.color[v] == RED) or
               (parent_state.color[u] == NO_COLOR and parent_state.color[v] == BLUE)
              ){
        {
            SolutionState opt_add_opposite = parent_state;
            addEdge(opt_add_opposite);
            opt_add_opposite.color[u] = oppositeColor(opt_add_opposite.color[v]);
            findBestState(opt_add_opposite, best_state);
        }
        {
            SolutionState opt_skip_same = parent_state;
            skipEdge(opt_skip_same);
            opt_skip_same.color[u] = opt_skip_same.color[v];
            findBestState(opt_skip_same, best_state);
        }
    } else if ((parent_state.color[u] == RED and parent_state.color[v] == BLUE) or
               (parent_state.color[u] == BLUE and parent_state.color[v] == RED)
              ){
        SolutionState opt_add = parent_state;
        addEdge(opt_add);
        findBestState(opt_add, best_state);
    }
}

void findMaxConnectedBipartiteSubgraph(SolutionState initial_solution_state){
    recursive_calls = 0;
    auto start_time = chrono::high_resolution_clock::now();
    // Check if input isn't solution.
    if (initial_solution_state.isBipartite() and initial_solution_state.isConnected()){
        cout << initial_solution_state.str(start_time);
        return;
    }
    // Reset Edge usage and coloring.
    initial_solution_state.resetSolution();
    // Find best state
    SolutionState best_state = initial_solution_state;
    findBestState(initial_solution_state, best_state);
    cout << best_state.str(start_time);
}

int main(int argc, char* argv[]) {
    vector<SolutionState> inputs = readInput(argc, argv);
    for (const auto & input : inputs)
        findMaxConnectedBipartiteSubgraph(input);
    return 0;
}
