/*
 * Jorge Zuniga (zuniga.cz)
 * FIT CVUT NI-PDP
 * 2023
 */
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <utility>
#include <vector>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <mpi.h>
//----------------------------------------------------------------------------------------------------------------------
#define MAX_VERTICES 150
#define MAX_EDGES 11175 // (150 choose 2)
#define MAX_WEIGHT 120
#define MIN_WEIGHT 80
// OpenMP number of threads
int number_of_threads = 1;
// MPI Main process id
#define MPI_MAIN 0
// MPI Tags
#define TAG_NUM_OF_INPUTS 0
#define TAG_GRAPH 1
#define TAG_STATE 2
#define TAG_BEST 3
//----------------------------------------------------------------------------------------------------------------------
using namespace std;
namespace fs = std::filesystem;
using time_point = chrono::high_resolution_clock::time_point;
//----------------------------------------------------------------------------------------------------------------------
enum color_t : uint8_t {NO_COLOR = 0, RED = 1, BLUE = 2};
class Format;
class MyMpi; // MPI Sending and receive
class Edge;
class Graph;
class SolutionState;
class ProblemInstance; // MPI Main
class Worker;          // MPI Worker
class InputHandler;
//----------------------------------------------------------------------------------------------------------------------
class Format {
public:
    static string elapsedTime(time_point start_time, time_point end_time) {
        auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        auto h = chrono::duration_cast<chrono::hours>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(h);
        auto m = chrono::duration_cast<chrono::minutes>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(m);
        auto s = chrono::duration_cast<chrono::seconds>(ms);
        ms -= chrono::duration_cast<chrono::milliseconds>(s);
        ostringstream oss;
        oss << h.count() << "h:" << m.count() << "m:" << s.count() << "." << ms.count() << "s";
        return oss.str();
    }
};
//----------------------------------------------------------------------------------------------------------------------
class MyMpi {
public:
    static void sendString (const int & dest, const int & tag, const string & message) {
        MPI_Send(message.data(), int(message.size()), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }

    static void sendInt(const int & dest, const int & tag, const int & message) {
        MPI_Send(&message, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }

    static MPI_Status recvString(const int & source, const int & tag, string & buffer) {
        MPI_Status status;
        MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
        int message_size;
        MPI_Get_count(&status, MPI_CHAR, &message_size);
        buffer.resize(message_size);
        MPI_Recv(&buffer[0], message_size, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
        return status;
    }

    static MPI_Status recvInt(const int & source, const int & tag, int & message) {
        MPI_Status status;
        MPI_Recv(&message, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        return status;
    }
};
//----------------------------------------------------------------------------------------------------------------------
class Edge {
public:
    uint8_t u; // < 150
    uint8_t v; // < 150
    uint8_t weight; // 80 <= weight <= 120
public:
    Edge() {
        this->u = 0;
        this->v = 0;
        this->weight = 0;
    }

    Edge(int u, int v, int weight) {
        this->u = u;
        this->v = v;
        this->weight = weight;
    }

    bool operator > (const Edge & other) const {
        if (this->weight != other.weight)
            return this->weight > other.weight;
        else if (this->u != other.u)
            return this->u > other.u;
        else if (this->v != other.v)
            return this->v > other.v;
        return false;
    }

    [[nodiscard]] string printEdge() const {
        ostringstream oss;
        oss << "{(" << int(u) << ", " << int(v) << "), " << int(weight) << "}";
        return oss.str();
    }
};
//----------------------------------------------------------------------------------------------------------------------
class Graph {
public:
    Edge edges[MAX_EDGES];
    uint16_t edges_size = 0; // < 11175 = (150 choose 2)
public:
    string toString() {
        ostringstream oss;
        for (auto & edge : edges)
            oss << int(edge.u) << " " << int(edge.v) << " " << int(edge.weight) << " ";
        oss << int(edges_size);
        return oss.str();
    }

    void fromString(const string& input) {
        int u, v, w, size;
        istringstream iss(input);
        for (auto & edge : edges){
            iss >> u;   edge.u = u;
            iss >> v;   edge.v = v;
            iss >> w;   edge.weight = w;
        }
        iss >> size;    edges_size = size;
    }
};
//----------------------------------------------------------------------------------------------------------------------
class SolutionState {
public:
    color_t colors[MAX_VERTICES] = {NO_COLOR};
    uint8_t num_of_vertices = 0;        // < 150
    uint16_t edge_index = 0;            // < 11175 = (150 choose 2)
    uint16_t used_edges = 0;            // < 11175 = (150 choose 2)
    uint32_t cost = 0;                  // < 1341000 = 11175 * 120
    uint32_t sum_cost_all = 0;          // < 1341000 = 11175 * 120
    uint32_t edges_total_weight = 0;    // < 1341000 = 11175 * 120
    Graph * graph = nullptr;
public:
    string toString() {
        ostringstream oss;
        for (auto & color : colors)
            oss << int(color) << " ";
        oss << int(num_of_vertices) << " ";
        oss << int(edge_index) << " ";
        oss << int(used_edges) << " ";
        oss << int(cost) << " ";
        oss << int(sum_cost_all) << " ";
        oss << int(edges_total_weight);
        return oss.str();
    }

    void fromString(const string& input, Graph * graph_ptr) {
        int int_color, int_num_of_vertices, int_edge_index, int_used_edges, int_cost, int_sum_cost_all, int_edges_total_weight;
        istringstream iss(input);
        for (auto & color : colors) {
            iss >> int_color; color = static_cast<color_t>(int_color);
        }
        iss >> int_num_of_vertices;     num_of_vertices = int_num_of_vertices;
        iss >> int_edge_index;          edge_index = int_edge_index;
        iss >> int_used_edges;          used_edges = int_used_edges;
        iss >> int_cost;                cost = int_cost;
        iss >> int_sum_cost_all;        sum_cost_all = int_sum_cost_all;
        iss >> int_edges_total_weight;  edges_total_weight = int_edges_total_weight;
        graph = graph_ptr;
    }

    void resetSolution() {
        memset(colors, NO_COLOR, sizeof(colors));
        edge_index = 0;
        used_edges = 0;
        cost = 0;
    }

    [[nodiscard]] bool isLeaf() const {
        return edge_index == graph->edges_size;
    }

    [[nodiscard]] bool isBetterThan(SolutionState state) const {
        return cost > state.cost;
    }

    bool isBipartite() {
        unordered_map<int, vector<int>> adj_map;
        for (int i = 0; i < graph->edges_size; i++) {
            adj_map[int(graph->edges[i].u)].push_back(int(graph->edges[i].v));
            adj_map[int(graph->edges[i].v)].push_back(int(graph->edges[i].u));
        }
        queue<pair<int, color_t>> q;
        for (auto & [vertex, _] : adj_map){
            if (this->colors[vertex] == NO_COLOR){
                this->colors[vertex] = RED;
                q.emplace(vertex, this->colors[vertex]);
                while(!q.empty()){
                    pair<int, color_t> p = q.front();
                    q.pop();
                    int v = p.first;
                    color_t c = p.second;
                    for (auto & j : adj_map[v]){
                        if (this->colors[j] == c)
                            return false;
                        if (this->colors[j] == NO_COLOR){
                            this->colors[j] = SolutionState::getOppositeColor(c);
                            q.emplace(j, this->colors[j]);
                        }
                    }
                }
            }
        }
        return true;
    }

    void isConnectedDFS(const uint8_t (& adj_map)[MAX_VERTICES][MAX_VERTICES], const uint8_t (& adj_map_sizes)[MAX_VERTICES], uint8_t v, bool (& visited)[MAX_VERTICES]) {
        visited[v] = true;
        for (int i = 0; i < adj_map_sizes[v]; i++)
            if (!visited[adj_map[v][i]])
                isConnectedDFS(adj_map, adj_map_sizes, adj_map[v][i], visited);
    }

    bool isConnected() {
        uint8_t adj_map[MAX_VERTICES][MAX_VERTICES] = {{0}};
        uint8_t adj_map_sizes[MAX_VERTICES] = {0};
        for (int i = 0; i < graph->edges_size; i++) {
            adj_map[graph->edges[i].u][adj_map_sizes[graph->edges[i].u]] = graph->edges[i].v;
            adj_map_sizes[graph->edges[i].u]++;
            adj_map[graph->edges[i].v][adj_map_sizes[graph->edges[i].v]] = graph->edges[i].u;
            adj_map_sizes[graph->edges[i].v]++;
        }
        bool visited[MAX_VERTICES] = {false};
        isConnectedDFS(adj_map, adj_map_sizes, 0, visited);
        for (int i = 0; i < num_of_vertices; ++i)
            if (!visited[i])
                return false;
        return true;
    }

    [[nodiscard]] uint32_t sumWeightRemainingEdges() const {
        return edges_total_weight - sum_cost_all;
    }

    [[nodiscard]] int numOfUsedEdges() const {
        return int(used_edges);
    }

    [[nodiscard]] int numOfRemainingEdges() const {
        return graph->edges_size - edge_index;
    }

    static color_t getOppositeColor(color_t c) {
        return (c == RED) ? BLUE : RED;
    }

    void skipEdge() {
        sum_cost_all += graph->edges[edge_index].weight;
        edge_index++;
    }

    void addEdge() {
        sum_cost_all += graph->edges[edge_index].weight;
        cost += graph->edges[edge_index].weight;
        edge_index++;
        used_edges++;
    }
};
//----------------------------------------------------------------------------------------------------------------------
class ProblemInstance {
private:
    Graph graph;
    SolutionState initial_state;
    SolutionState best_state;
    // MPI parallelism
    vector<SolutionState> solution_states_queue;
    int world_size;
    // Metrics
    string input_name;
    time_point start_time;
private:
    void printResult() {
        auto end_time = chrono::high_resolution_clock::now();
        cout << "============================================" << endl;
        cout << "Result of: " << this->input_name << endl;
        cout << "--------------------------------------------" << endl;
        vector<uint8_t> reds;
        vector<uint8_t> blues;
        for (uint8_t i = 0; i < this->best_state.num_of_vertices; i++)
            if (this->best_state.colors[i] == RED)
                reds.push_back(i);
            else
                blues.push_back(i);
        cout << "RED: {";
        for (auto i = reds.begin(); i != reds.end(); i++) {
            cout << int(*i);
            if (i + 1 != reds.end()) cout << ", ";
        }
        cout << "}" << endl;
        cout << "BLUE: {";
        for (auto i = blues.begin(); i != blues.end(); i++) {
            cout << int(*i);
            if (i + 1 != blues.end()) cout << ", ";
        }
        cout << "}" << endl;
        cout << "--------------------------------------------" << endl;
        vector<Edge> used_edges;
        for (int i = 0; i < graph.edges_size; i++)
            if (best_state.colors[graph.edges[i].u] != best_state.colors[graph.edges[i].v])
                used_edges.push_back(graph.edges[i]);
        cout << "EDGES: ";
        for (auto i = used_edges.begin(); i != used_edges.end(); i++) {
            cout << i->printEdge();
            if (i+1 != used_edges.end()) cout << ", ";
        }
        cout << endl;
        cout << "--------------------------------------------" << endl;
        cout << "Weights sum = " << best_state.cost << endl;
        cout << "--------------------------------------------" << endl;
        cout << "Took: " << Format::elapsedTime(start_time, end_time) << endl;
        cout << "============================================" << endl;
    }

    [[nodiscard]] bool noBetterSolutionPossible(SolutionState state) const {
        if (state.cost + state.sumWeightRemainingEdges() < best_state.cost)
            return true;
        if (state.numOfUsedEdges() + state.numOfRemainingEdges() < int(state.num_of_vertices - 1))
            return true;
        return false;
    }

    size_t solutionQueueLimit() {
        return world_size * 2;
    }

    void generateStatesQueue() {
        this->solution_states_queue.push_back(initial_state);
        while (solution_states_queue.size() < solutionQueueLimit()) {
            findBestStateBFS(solution_states_queue.front());
            solution_states_queue.erase(solution_states_queue.begin());
        }
    }

    void findBestStateBFS(SolutionState state) {
        // Check if better solution found
        if (state.isLeaf()) {
            if (state.isConnected() and state.isBetterThan(best_state)) {
                best_state = state;
                return;
            } else {
                return;
            }
        }
        // Cut the tree of solutions
        if (noBetterSolutionPossible(state))
            return;
        // Color the graph to keep bipartity.
        int u = graph.edges[state.edge_index].u;
        int v = graph.edges[state.edge_index].v;
        if ((state.colors[u] == RED and state.colors[v] == RED) or
            (state.colors[u] == BLUE and state.colors[v] == BLUE)){
            {
                SolutionState opt_skip = state;
                opt_skip.skipEdge();
                solution_states_queue.push_back(opt_skip);
            }
        } else if (state.colors[u] == NO_COLOR and state.colors[v] == NO_COLOR) {
            {
                SolutionState opt_add_red_blue = state;
                opt_add_red_blue.addEdge();
                opt_add_red_blue.colors[u] = RED;
                opt_add_red_blue.colors[v] = BLUE;
                solution_states_queue.push_back(opt_add_red_blue);
            }
            {
                SolutionState opt_add_blue_red = state;
                opt_add_blue_red.addEdge();
                opt_add_blue_red.colors[u] = BLUE;
                opt_add_blue_red.colors[v] = RED;
                solution_states_queue.push_back(opt_add_blue_red);
            }
            {
                SolutionState opt_skip_red_red = state;
                opt_skip_red_red.skipEdge();
                opt_skip_red_red.colors[u] = RED;
                opt_skip_red_red.colors[v] = RED;
                solution_states_queue.push_back(opt_skip_red_red);
            }
            {
                SolutionState opt_skip_blue_blue = state;
                opt_skip_blue_blue.skipEdge();
                opt_skip_blue_blue.colors[u] = BLUE;
                opt_skip_blue_blue.colors[v] = BLUE;
                solution_states_queue.push_back(opt_skip_blue_blue);
            }
        } else if ((state.colors[u] == RED and state.colors[v] == NO_COLOR) or
                   (state.colors[u] == BLUE and state.colors[v] == NO_COLOR)) {
            {
                SolutionState opt_add_opposite = state;
                opt_add_opposite.addEdge();
                opt_add_opposite.colors[v] = SolutionState::getOppositeColor(opt_add_opposite.colors[u]);
                solution_states_queue.push_back(opt_add_opposite);
            }
            {
                SolutionState opt_skip_same = state;
                opt_skip_same.skipEdge();
                opt_skip_same.colors[v] = opt_skip_same.colors[u];
                solution_states_queue.push_back(opt_skip_same);
            }
        } else if ((state.colors[u] == NO_COLOR and state.colors[v] == RED) or
                   (state.colors[u] == NO_COLOR and state.colors[v] == BLUE)) {
            {
                SolutionState opt_add_opposite = state;
                opt_add_opposite.addEdge();
                opt_add_opposite.colors[u] = SolutionState::getOppositeColor(opt_add_opposite.colors[v]);
                solution_states_queue.push_back(opt_add_opposite);
            }
            {
                SolutionState opt_skip_same = state;
                opt_skip_same.skipEdge();
                opt_skip_same.colors[u] = opt_skip_same.colors[v];
                solution_states_queue.push_back(opt_skip_same);
            }
        } else if ((state.colors[u] == RED and state.colors[v] == BLUE) or
                   (state.colors[u] == BLUE and state.colors[v] == RED)) {
            {
                SolutionState opt_add = state;
                opt_add.addEdge();
                solution_states_queue.push_back(opt_add);
            }
        }
    }
public:
    ProblemInstance(string input_name, int num_of_vertices, vector<Edge> edges) {
        for (graph.edges_size = 0; graph.edges_size < min(sizeof(this->graph.edges), edges.size()); graph.edges_size++) {
            this->graph.edges[graph.edges_size] = edges[graph.edges_size];
            this->initial_state.edges_total_weight += edges[graph.edges_size].weight;
        }
        sort(this->graph.edges, this->graph.edges + graph.edges_size, greater<>());
        initial_state.num_of_vertices = num_of_vertices;
        best_state = this->initial_state;
        world_size = 0;
        this->input_name = std::move(input_name);
    }

    string getInputName() {
        return input_name;
    }

    [[nodiscard]] uint32_t getBestStateCost() const {
        return best_state.cost;
    }

    void findMaxConnectedBipartiteSubgraph() {
        start_time = chrono::high_resolution_clock::now();
        initial_state.graph = &this->graph;
        if (initial_state.isBipartite() and initial_state.isConnected()) {
            best_state = initial_state;
        } else {
            initial_state.resetSolution();
            MPI_Comm_size(MPI_COMM_WORLD, &world_size);
            generateStatesQueue();
            // Send graph to all workers
            for (int i = 1; i < world_size; i++) {
                MyMpi::sendString(i, TAG_GRAPH, graph.toString());
            }
            // Send work
            int running_workers = 1;
            while (!solution_states_queue.empty()) {
                if (running_workers < world_size) {
                    MyMpi::sendString(running_workers, TAG_STATE, solution_states_queue.front().toString());
                    solution_states_queue.erase(solution_states_queue.begin());
                    running_workers++;
                } else {
                    SolutionState potential_new_best;
                    string potential_new_best_str;
                    MPI_Status status = MyMpi::recvString(MPI_ANY_SOURCE, TAG_BEST, potential_new_best_str);
                    potential_new_best.fromString(potential_new_best_str, &graph);
                    if (potential_new_best.isBetterThan(best_state)) {
                        best_state = potential_new_best;
                    } else {
                        MyMpi::sendString(status.MPI_SOURCE, TAG_STATE, best_state.toString());
                    }
                    MyMpi::sendString(status.MPI_SOURCE, TAG_STATE, solution_states_queue.front().toString());
                    solution_states_queue.erase(solution_states_queue.begin());
                }
            }
            while (running_workers > 1) {
                SolutionState potential_new_best;
                string potential_new_best_str;
                MPI_Status status;
                status = MyMpi::recvString(MPI_ANY_SOURCE, TAG_BEST, potential_new_best_str);
                potential_new_best.fromString(potential_new_best_str, &graph);
                if (potential_new_best.isBetterThan(best_state))
                    best_state = potential_new_best;
                MyMpi::sendString(running_workers - 1, TAG_STATE, initial_state.toString());
                running_workers--;
            }
        }
        printResult();
    }
};

//----------------------------------------------------------------------------------------------------------------------
class Worker {
private:
    Graph graph;
    SolutionState initial_state;
    SolutionState best_state;
private:
    [[nodiscard]] bool noBetterSolutionPossible(SolutionState state) const {
        if (state.cost + state.sumWeightRemainingEdges() < best_state.cost)
            return true;
        if (state.numOfUsedEdges() + state.numOfRemainingEdges() < int(state.num_of_vertices - 1))
            return true;
        return false;
    }

    void findBestStateDFS(SolutionState state) {
        // Check if better solution found
        if (state.isLeaf()) {
            if (state.isConnected() and state.isBetterThan(best_state)) {
                //#pragma omp critical
                {
                    if(state.isBetterThan(best_state))
                        best_state = state;
                }
                return;
            } else {
                return;
            }
        }
        // Cut the tree of solutions
        if (noBetterSolutionPossible(state))
            return;
        // Color the graph to keep bipartity.
        int u = graph.edges[state.edge_index].u;
        int v = graph.edges[state.edge_index].v;
        if ((state.colors[u] == RED and state.colors[v] == RED) or
            (state.colors[u] == BLUE and state.colors[v] == BLUE)){
            {
                SolutionState opt_skip = state;
                opt_skip.skipEdge();
                #pragma omp task
                {
                    findBestStateDFS(opt_skip);
                }
            }
        } else if (state.colors[u] == NO_COLOR and state.colors[v] == NO_COLOR) {
            {
                SolutionState opt_add_red_blue = state;
                opt_add_red_blue.addEdge();
                opt_add_red_blue.colors[u] = RED;
                opt_add_red_blue.colors[v] = BLUE;
                #pragma omp task
                {
                    findBestStateDFS(opt_add_red_blue);
                }
            }
            {
                SolutionState opt_add_blue_red = state;
                opt_add_blue_red.addEdge();
                opt_add_blue_red.colors[u] = BLUE;
                opt_add_blue_red.colors[v] = RED;
                #pragma omp task
                {
                    findBestStateDFS(opt_add_blue_red);
                }
            }
            {
                SolutionState opt_skip_red_red = state;
                opt_skip_red_red.skipEdge();
                opt_skip_red_red.colors[u] = RED;
                opt_skip_red_red.colors[v] = RED;
                #pragma omp task
                {
                    findBestStateDFS(opt_skip_red_red);
                }
            }
            {
                SolutionState opt_skip_blue_blue = state;
                opt_skip_blue_blue.skipEdge();
                opt_skip_blue_blue.colors[u] = BLUE;
                opt_skip_blue_blue.colors[v] = BLUE;
                #pragma omp task
                {
                    findBestStateDFS(opt_skip_blue_blue);
                }
            }
        } else if ((state.colors[u] == RED and state.colors[v] == NO_COLOR) or
                   (state.colors[u] == BLUE and state.colors[v] == NO_COLOR)) {
            {
                SolutionState opt_add_opposite = state;
                opt_add_opposite.addEdge();
                opt_add_opposite.colors[v] = SolutionState::getOppositeColor(opt_add_opposite.colors[u]);
                #pragma omp task
                {
                    findBestStateDFS(opt_add_opposite);
                }
            }
            {
                SolutionState opt_skip_same = state;
                opt_skip_same.skipEdge();
                opt_skip_same.colors[v] = opt_skip_same.colors[u];
                #pragma omp task
                {
                    findBestStateDFS(opt_skip_same);
                }
            }
        } else if ((state.colors[u] == NO_COLOR and state.colors[v] == RED) or
                   (state.colors[u] == NO_COLOR and state.colors[v] == BLUE)) {
            {
                SolutionState opt_add_opposite = state;
                opt_add_opposite.addEdge();
                opt_add_opposite.colors[u] = SolutionState::getOppositeColor(opt_add_opposite.colors[v]);
                #pragma omp task
                {
                    findBestStateDFS(opt_add_opposite);
                }
            }
            {
                SolutionState opt_skip_same = state;
                opt_skip_same.skipEdge();
                opt_skip_same.colors[u] = opt_skip_same.colors[v];
                #pragma omp task
                {
                    findBestStateDFS(opt_skip_same);
                }
            }
        } else if ((state.colors[u] == RED and state.colors[v] == BLUE) or
                   (state.colors[u] == BLUE and state.colors[v] == RED)) {
            {
                SolutionState opt_add = state;
                opt_add.addEdge();
                #pragma omp task
                {
                    findBestStateDFS(opt_add);
                }
            }
        }
    }
public:
    void workerMain(){
        // Receive graph
        string graph_str;
        MyMpi::recvString(MPI_MAIN, TAG_GRAPH, graph_str);
        graph.fromString(graph_str);
        // Receive work
        while (true) {
            string initial_state_str;
            MyMpi::recvString(MPI_MAIN, TAG_STATE, initial_state_str);
            initial_state.fromString(initial_state_str, &graph);
            if (all_of(initial_state.colors, initial_state.colors + MAX_VERTICES, [](color_t c){return c == NO_COLOR;})) {
                break;
            } else if (initial_state.isLeaf() and initial_state.isBetterThan(best_state)) {
                best_state = initial_state;
            } else {
                #pragma omp parallel num_threads(number_of_threads)
                    #pragma omp single
                        findBestStateDFS(initial_state);
                MyMpi::sendString(MPI_MAIN, TAG_BEST, best_state.toString());
            }
        }
    }
};
//----------------------------------------------------------------------------------------------------------------------
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
                if (column > row and MIN_WEIGHT <= weight and weight <= MAX_WEIGHT) {
                    Edge edge(row, column, weight);
                    edges.push_back(edge);
                }
            }
        }
        input_file.close();
        inputs.emplace_back(input_file_path, num_of_vertices, edges);
        return inputs;
    }

    static vector<ProblemInstance> readInput(int argc, char* argv[]) {
        vector<ProblemInstance> inputs;
        vector<string> args(argv + 1, argv+argc);
        // Find flags -h, --help, --file <filepath>...
        bool help_arg_found_short = find(args.begin(), args.end(), "-h") != args.end();
        bool help_arg_found_long = find(args.begin(), args.end(), "--help") != args.end();
        auto file_arg_it = find(args.begin(), args.end(), "--file");
        bool file_arg_found = file_arg_it != args.end();
        auto thread_num_arg_it = find(args.begin(), args.end(), "-t");
        bool thread_num_arg_found = thread_num_arg_it != args.end();
        bool any_found = (help_arg_found_short or help_arg_found_long or file_arg_found or thread_num_arg_found);
        // If -h, --help or none of the accepted flags found. Print out the help message.
        if (help_arg_found_short or help_arg_found_long or !any_found) {
            InputHandler::printHelp();
            return inputs;
        }
        // If -t <int> is found, set the number of threads variable
        if (thread_num_arg_found) {
            auto num_of_threads_it = next(thread_num_arg_it);
            if (num_of_threads_it != args.end()) {
                istringstream iss(*num_of_threads_it);
                iss >> number_of_threads;
            }
            cout << "Number of threads: " << number_of_threads << endl;
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
        return inputs;
    }
};
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Main process
    if (rank == MPI_MAIN) {
        // Read input
        vector<ProblemInstance> inputs = InputHandler::readInput(argc, argv);
        // Measure time
        auto start_time_total = chrono::high_resolution_clock::now();
        // Find solutions
        for (auto & problem_instance : inputs)
            problem_instance.findMaxConnectedBipartiteSubgraph();
        // End of time measure
        auto end_time_total = chrono::high_resolution_clock::now();
        // Print total time if more than one result
        if (inputs.size() > 1) {
            cout << "============================================" << endl;
            cout << "Total time: " << Format::elapsedTime(start_time_total, end_time_total) << endl;
            cout << "============================================" << endl;
        }
        // Assert
        unordered_map<string, uint32_t> results = {
                // Easy
                {"graf_10_3.txt", 1300},
                {"graf_10_5.txt", 1885},
                {"graf_10_6.txt", 2000},
                {"graf_10_7.txt", 2348},
                {"graf_12_3.txt", 1422},
                {"graf_12_5.txt", 2219},
                {"graf_12_6.txt", 2533},
                {"graf_12_9.txt", 3437},
                {"graf_13_9.txt", 3700},
                {"graf_13_12.txt", 4182},
                {"graf_15_4.txt", 2547},
                {"graf_15_5.txt", 2892},
                {"graf_15_6.txt", 3353},
                {"graf_15_8.txt", 3984},
                {"graf_15_12.txt", 5380},
                {"graf_15_14.txt", 5578},
                {"graf_17_10.txt", 5415},
                // Medium
                {"graf_20_16.txt", 9353},
                {"graf_20_17.txt", 9768},
                {"graf_20_19.txt", 10288},
                {"graf_21_15.txt", 9570},
                {"graf_22_17.txt", 11015},
                {"graf_23_20.txt", 12902},
                {"graf_24_23.txt", 14844},
                // Hard
                {"graf_25_16.txt", 12105},
                {"graf_25_22.txt", 15594},
                {"graf_26_25.txt", 17477},
                {"graf_27_19.txt", 15470},
                {"graf_28_24.txt", 18729},
                {"graf_29_26.txt", 20810},
                {"graf_30_25.txt", 21336}
        };
        // Print if wrong result
        for (auto problem_instance : inputs) {
            string name = fs::path(problem_instance.getInputName()).filename();
            if (results.count(name) > 0) {
                uint32_t expected = results[name];
                uint32_t got = problem_instance.getBestStateCost();
                if (got != expected)
                    cout << problem_instance.getInputName() << " expected: " << expected << " got: " << got << endl;
            }
        }
    }
    // Worker process
    if (rank != MPI_MAIN) {
        Worker worker;
        worker.workerMain();
    }
    MPI_Finalize();
    return 0;
}