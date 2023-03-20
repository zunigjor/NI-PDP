/*
 * Jorge Zuniga (zuniga.cz)
 * FIT CVUT NI-PDP
 * 2023
 */
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <omp.h>

namespace fs = std::filesystem;
using namespace std;

enum color_t {NO_COLOR, RED, BLUE};

class Edge {
public:
    pair<int, int> vertices;
    int weight;
    bool used;
public:
    Edge(int u, int v, int weight) {
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
    int num_of_vertices;
    vector<Edge> edges;
    vector<color_t> color;
    int edge_index;
    int cost;
public:
    SolutionState() {
        this->num_of_vertices = 0;
        this->edge_index = 0;
        this->cost = 0;
    };

    SolutionState(int num_of_vertices, vector<Edge> edges) {
        this->cost = 0;
        this->num_of_vertices = num_of_vertices;
        this->edge_index = 0;
        this->edges = std::move(edges);
        this->color = vector<color_t>(num_of_vertices, NO_COLOR);
        this->sortEdgesByWeightDesc();
    }

    void sortEdgesByWeightDesc() {
        sort(this->edges.rbegin(), this->edges.rend());
    }

    void resetSolution() {
        for (auto & graph_edge : this->edges)
            graph_edge.used = false;
        for (auto & vertex_color : color)
            vertex_color = NO_COLOR;
    }

    int numOfUsedEdges() {
        int num = 0;
        for (auto & edge : this->edges)
            if (edge.used) num++;
        return num;
    }

    bool isBipartite() {
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
                            this->color[j] = SolutionState::getOppositeColor(c);
                            q.emplace(j, this->color[j]);
                        }
                    }
                }
            }
        }
        return true;
    }

    bool isConnected() {
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

    int sumWeightRemainingEdges() {
        int sum = 0;
        for (auto i = this->edges.begin() + this->edge_index; i != this->edges.end(); ++i) {
            sum += i->weight;
        }
        return sum;
    }

    int numOfRemainingEdges() {
        return int(this->edges.size()) - this->edge_index;
    }

    void skipEdge() {
        this->edge_index++;
    }

    void addEdge() {
        this->edges[this->edge_index].used = true;
        this->cost += this->edges[this->edge_index].weight;
        this->edge_index++;
    }

    static color_t getOppositeColor(color_t c){
        return (c == RED) ? BLUE : RED;
    }
};

class ProblemInstance {
public:
    string input_name;
    SolutionState initial_state;
    SolutionState best_state;
    // Metrics
    unsigned int recursive_calls;
    chrono::high_resolution_clock::time_point start_time;
public:
    ProblemInstance(string input_name, SolutionState initial_state) {
        this->input_name = std::move(input_name);
        this->initial_state = std::move(initial_state);
        this->recursive_calls = 0;
    }

    bool noBetterSolutionPossible(SolutionState state){
        if (state.cost + state.sumWeightRemainingEdges() < best_state.cost)
            return true;
        if (state.numOfUsedEdges() + state.numOfRemainingEdges() < state.num_of_vertices - 1)
            return true;
        return false;
    }

    void findBestState(SolutionState parent_state) {
        this->recursive_calls++;
        if (parent_state.edge_index >= int(parent_state.edges.size())) {
            if (parent_state.isConnected() and parent_state.cost >= this->best_state.cost) {
                this->best_state = parent_state;
                return;
            } else {
                return;
            }
        }
        if (this->noBetterSolutionPossible(parent_state))
            return;

        int u = parent_state.edges[parent_state.edge_index].vertices.first;
        int v = parent_state.edges[parent_state.edge_index].vertices.second;
        if ((parent_state.color[u] == RED and parent_state.color[v] == RED) or
            (parent_state.color[u] == BLUE and parent_state.color[v] == BLUE)
                ){
            SolutionState opt_skip = parent_state;
            opt_skip.skipEdge();
            findBestState(opt_skip);
        } else if (parent_state.color[u] == NO_COLOR and parent_state.color[v] == NO_COLOR){
            {
                SolutionState opt_add_red_blue = parent_state;
                opt_add_red_blue.addEdge();
                opt_add_red_blue.color[u] = RED;
                opt_add_red_blue.color[v] = BLUE;
                findBestState(opt_add_red_blue);
            }
            {
                SolutionState opt_add_blue_red = parent_state;
                opt_add_blue_red.addEdge();
                opt_add_blue_red.color[u] = BLUE;
                opt_add_blue_red.color[v] = RED;
                findBestState(opt_add_blue_red);
            }
            {
                SolutionState opt_skip_red_red = parent_state;
                opt_skip_red_red.skipEdge();
                opt_skip_red_red.color[u] = RED;
                opt_skip_red_red.color[v] = RED;
                findBestState(opt_skip_red_red);
            }
            {
                SolutionState opt_skip_blue_blue = parent_state;
                opt_skip_blue_blue.skipEdge();
                opt_skip_blue_blue.color[u] = BLUE;
                opt_skip_blue_blue.color[v] = BLUE;
                findBestState(opt_skip_blue_blue);
            }
        } else if ((parent_state.color[u] == RED and parent_state.color[v] == NO_COLOR) or
                   (parent_state.color[u] == BLUE and parent_state.color[v] == NO_COLOR)
                ){
            {
                SolutionState opt_add_opposite = parent_state;
                opt_add_opposite.addEdge();
                opt_add_opposite.color[v] = SolutionState::getOppositeColor(opt_add_opposite.color[u]);
                findBestState(opt_add_opposite);
            }
            {
                SolutionState opt_skip_same = parent_state;
                opt_skip_same.skipEdge();
                opt_skip_same.color[v] = opt_skip_same.color[u];
                findBestState(opt_skip_same);
            }
        } else if ((parent_state.color[u] == NO_COLOR and parent_state.color[v] == RED) or
                   (parent_state.color[u] == NO_COLOR and parent_state.color[v] == BLUE)
                ){
            {
                SolutionState opt_add_opposite = parent_state;
                opt_add_opposite.addEdge();
                opt_add_opposite.color[u] = SolutionState::getOppositeColor(opt_add_opposite.color[v]);
                findBestState(opt_add_opposite);
            }
            {
                SolutionState opt_skip_same = parent_state;
                opt_skip_same.skipEdge();
                opt_skip_same.color[u] = opt_skip_same.color[v];
                findBestState(opt_skip_same);
            }
        } else if ((parent_state.color[u] == RED and parent_state.color[v] == BLUE) or
                   (parent_state.color[u] == BLUE and parent_state.color[v] == RED)
                ){
            {
                SolutionState opt_add = parent_state;
                opt_add.addEdge();
                findBestState(opt_add);
            }
        }
    }

    void findMaxConnectedBipartiteSubgraph() {
        this->start_time = chrono::high_resolution_clock::now();
        if (this->initial_state.isBipartite() and this->initial_state.isConnected()) {
            this->best_state = this->initial_state;
            this->outputBestState();
            return;
        }
        this->initial_state.resetSolution();
        this->findBestState(this->initial_state);
        this->outputBestState();
    }

    void outputBestState() {
        ostringstream oss;
        auto end_time = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - this->start_time);
        oss << "============================================" << endl;
        oss << "Result of: " << this->input_name << endl;
        oss << "--------------------------------------------" << endl;
        vector<size_t> reds;
        vector<size_t> blues;
        for (size_t i = 0; i < this->best_state.color.size(); i++) {
            if(this->best_state.color[i] == RED)
                reds.push_back(i);
            else
                blues.push_back(i);
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
        oss << "--------------------------------------------" << endl;
        for (auto & edge : this->best_state.edges)
            if (edge.used)
                oss << edge << endl;
        oss << "--------------------------------------------" << endl;
        oss << "w_sum = " << this->best_state.cost << endl;
        oss << "--------------------------------------------" << endl;
        auto s = chrono::duration_cast<chrono::seconds>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(s);
        auto m = chrono::duration_cast<chrono::minutes>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(m);
        auto h = chrono::duration_cast<chrono::hours>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(h);
        oss << "recursive calls: " << this->recursive_calls << endl;
        oss << "took: " << h.count() << "h:" << m.count() << "m:" << s.count() << "." << ms.count() << "s" << endl;
        oss << "============================================" << endl;
        cout << oss.str();
    }
};

class InputHandler {
public:
    static void printHelp() {
        cout << "NI-PDP řešení úkolu pro sudé paralelky." << endl;
        cout << "Bipartitní souvislý podgraf hranově ohodnoceného grafu s maximální vahou" << endl;
        cout << "Parametry:" << endl;
        cout << "   -h, --help                  Výpíše tuto zprávu." << endl;
        cout << "   --file <filepath>...        Spustí program pro soubory." << endl;
        cout << "   --folder <folderpath>...    Spustí program pro složky." << endl;
        cout << "Parametry jsou vyhodnocvány v pořadí." << endl;
        cout << "Jorge Zuňiga 2023" << endl;
    }

    static vector<ProblemInstance> readFromFile(const string & input_file_path) {
        vector<ProblemInstance> inputs;
        cout << "Loaded file " << input_file_path << endl;
        if (!fs::exists(fs::path(input_file_path))) {
            cout << "File \"" << input_file_path << "\' does not exist." << endl;
            return inputs;
        }
        ifstream input_file(input_file_path);
        if (!input_file.is_open()) {
            cout << "Error opening " << input_file_path << endl;
            return inputs;
        }
        int num_of_vertices = 0;
        vector<Edge> edges;
        input_file >> num_of_vertices;
        for (int row = 0; row < num_of_vertices; row ++) {
            for (int column = 0; column < num_of_vertices; column++) {
                int weight;
                input_file >> weight;
                if (column > row and weight >= 80 and weight <= 120) {
                    Edge edge(row, column, weight);
                    edges.push_back(edge);
                }
            }
        }
        input_file.close();
        inputs.emplace_back(input_file_path, SolutionState(num_of_vertices, edges));
        return inputs;
    }

    static vector<ProblemInstance> readFromFolder(const string & input_folder_path) {
        vector<ProblemInstance> inputs;
        cout << "Loaded folder " << input_folder_path << endl;
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
            vector<ProblemInstance> input = readFromFile(file_path);
            inputs.insert(inputs.end(), input.begin(), input.end());
        }
        return inputs;
    }

    static vector<ProblemInstance> readInput(int argc, char* argv[]) {
        vector<ProblemInstance> inputs;
        vector<string> args(argv + 1, argv+argc);
        // Find flags -h, --help, --file <filepath>..., --folder <folderpath>...
        bool help_arg_found_short = find(args.begin(), args.end(), "-h") != args.end();
        bool help_arg_found_long = find(args.begin(), args.end(), "--help") != args.end();
        auto file_arg_it = find(args.begin(), args.end(), "--file");
        bool file_arg_found = file_arg_it != args.end();
        auto folder_arg_it = find(args.begin(), args.end(), "--folder");
        bool folder_arg_found = folder_arg_it != args.end();
        bool any_found = (help_arg_found_short or help_arg_found_long or file_arg_found or folder_arg_found);
        // If -h, --help or none of the accepted flags found. Print out the help message.
        if (help_arg_found_short or help_arg_found_long or !any_found) {
            InputHandler::printHelp();
            return inputs;
        }
        // If --file <filepath>... is present, extract from files
        if (file_arg_found) {
            auto input_files_it = next(file_arg_it);
            while (input_files_it != args.end() and *input_files_it != "--folder") {
                vector<ProblemInstance> file_input = InputHandler::readFromFile(*input_files_it);
                inputs.insert(inputs.end(), file_input.begin(), file_input.end());
                input_files_it = next(input_files_it);
            }
        }
        // If --folder <folderpath>... is present, extract from folders
        if (folder_arg_found) {
            auto input_folders_it = next(folder_arg_it);
            while (input_folders_it != args.end() and *input_folders_it != "--file") {
                vector<ProblemInstance> folder_inputs = InputHandler::readFromFolder(*input_folders_it);
                inputs.insert(inputs.end(), folder_inputs.begin(), folder_inputs.end());
                input_folders_it = next(input_folders_it);
            }
        }
        return inputs;
    }
};

int main(int argc, char* argv[]) {
    vector<ProblemInstance> inputs = InputHandler::readInput(argc, argv);
    for (auto problem_instance : inputs)
        problem_instance.findMaxConnectedBipartiteSubgraph();
    return 0;
}