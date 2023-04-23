#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <map>
#include <random>
#include <algorithm>
#include <tuple>
#include <queue>

using namespace std;

struct graph_node{
    char identifier;
    int heuristic_value;
    std::vector<std::pair<char, int>> edges;
};

bool custom_operator(pair<char, int> x, pair<char, int> y)
{
    return x.second < y.second; //return the one with smaller first element
}

bool custom_operator_uniform_cost_search(std::tuple<char, int, std::vector<char>> x, std::tuple<char, int, std::vector<char>> y)
{
    return get<1>(x) > get<1>(y); //return the one with smaller first element
}

bool custom_operator_a_star(std::tuple<char, int, std::vector<char>, int> x, std::tuple<char, int, std::vector<char>, int> y)
{
    return get<3>(x) > get<3>(y); //return the one with smaller first element
}

bool custom_operator_tuple(tuple<int, char> x, tuple<int, char> y)
{
    return get<0>(x) > get<0>(y); //return the one with smaller first element
}

int rand_generator(int a, int b) {
    std::random_device rd;
    std::mt19937 e(rd());
    std::uniform_int_distribution<int> dist(a, b);
    int succesor = dist(e);
    return succesor;
}


void dfs(char start, char goal, std::map<char, graph_node>  graph) {
    char current_node = start;
    vector<char> history;
    int total = 0;
    while (current_node != goal)
    {
       history.push_back(current_node);
       int size = graph.at(current_node).edges.size();
       int successor = rand_generator(0, size - 1);
       total += graph.at(current_node).edges.at(successor).second;
       current_node = graph.at(current_node).edges.at(successor).first;
    }
    history.push_back(goal);
    for (auto &&node : history)
    {
        std::cout << node << std::endl;
    }
    std::cout << "total: " << total << std::endl;
    
}

void uniform_cost_search(char start, char goal, std::map<char, graph_node>  graph){
    char current_node = start;
    vector<char> history;
    int total = 0;
    std::priority_queue<std::tuple<char, int, std::vector<char>>, std::vector<std::tuple<char, int, std::vector<char>>>, std::function<bool(std::tuple<char, int, std::vector<char>>, std::tuple<char, int, std::vector<char>>)>> pq(custom_operator_uniform_cost_search);
    history.push_back(current_node);
    pq.push(std::make_tuple(graph.at(current_node).identifier, 0, history));
    std::tuple<char, int, std::vector<char>> final;
    while (get<0>(pq.top()) != goal)
    {
        for (auto &&nd : graph.at(get<0>(pq.top())).edges)
        {
            std::vector<char> temp = get<2>(pq.top());
            temp.push_back(nd.first);
            pq.push(std::make_tuple(nd.first, get<1>(pq.top()) + nd.second, temp));
        }
        pq.pop();
        if (get<0>(pq.top()) == goal)
        {
            final = pq.top();
        }            
    }
    std::cout << "final node: "<<get<0>(final) << " final distance: "<< get<1>(final)<<" historial: ";
    for (auto &&temp : get<2>(final))
        {
            std::cout<<temp<<", ";
        }
    std::cout<<std::endl;         
}

void a_star(char start, char goal, std::map<char, graph_node>  graph){
    char current_node = start;
    vector<char> history;
    std::priority_queue<std::tuple<char, int, std::vector<char>, int>, std::vector<std::tuple<char, int, std::vector<char>, int>>, std::function<bool(std::tuple<char, int, std::vector<char>, int>, std::tuple<char, int, std::vector<char>, int>)>> pq(custom_operator_a_star);
    history.push_back(current_node);    
    pq.push(std::make_tuple(graph.at(current_node).identifier, 0, history, 0));
    std::tuple<char, int, std::vector<char>, int> final;
    std::tuple<char, int, std::vector<char>, int> current;
    while (get<0>(pq.top()) != goal)
    {
        current = pq.top();
        pq.pop();
        for (auto &&nd : graph.at(get<0>(current)).edges)
        {
            std::vector<char> temp = get<2>(current);
            temp.push_back(nd.first);
            pq.push(std::make_tuple(nd.first, get<1>(current) + nd.second, temp, get<1>(current) + nd.second + graph.at(nd.first).heuristic_value));
        }
        
        if (get<0>(pq.top()) == goal)
        {
            final = pq.top();
        }      
    }
    std::cout << "final node: "<<get<0>(final) << " final heuristic: "<< get<3>(final)<<" historial: ";
    for (auto &&temp : get<2>(final))
        {
            std::cout<<temp<<", ";
        }  
    std::cout<<std::endl;         
}

void greedy(char start, char goal, std::map<char, graph_node>  graph){
    char current_node = start;
    vector<char> history;
    int total = 0;
    while (current_node != goal)
    {
       history.push_back(current_node);
       std::vector<std::pair<char, int>> heuristic_compare;
       for (auto &&node : graph.at(current_node).edges)
       {
            heuristic_compare.push_back(std::make_pair(node.first, graph.at(node.first).heuristic_value));
       }
       std::sort(heuristic_compare.begin(),heuristic_compare.end(),custom_operator);
       total += graph.at(current_node).heuristic_value;
       current_node = heuristic_compare.at(0).first;
    }
    history.push_back(goal);
    for (auto &&node : history)
    {
        std::cout << node << std::endl;
    }
    std::cout << "total: " << total << std::endl;
}

void print_solution(std::vector<char> history, int total_distance, int total_heuristic){

}

int main(int argc, char const *argv[])
{   
    //creamos un mapa que como key recibe la letra que identifica al vertice y como value almacena la struct grap_node
    std::map<char, graph_node>  graph;

    std::string temp;
    char start, goal;
    //recibimos el nodo de entrada y de salida
    std::cin >> temp >> start;
    std::cin >> temp >> goal;
    //creamos un vector para almacenar las heuristicas
    std::vector<std::pair<char, int>> heuristics;
    //creamos un vector para almacenar las aristas donde el par de chars contiene los vertices y el int contiene el costo 
    //de ir de un vertice a otro
    std::vector<std::pair<std::pair<char, char>, int>> edges;
    //creamos un vector para almacenar las lines
    std::vector<std::string> lines;
    //recibimos todas las lineas
    std::getline(std::cin, temp);
    while (std::getline(std::cin, temp))
    {
        lines.push_back(temp);
    }
    
    //operamos sobre cada linea
    for (auto &&line : lines)
    {
        stringstream ss(line);
        std::string node, node1, node2;
        int cost, heuristic;
        //si la linea contiene una "," entonces esta debe ser una linea que describe una arista
        if (line.find(',') != std::string::npos)
        {
            std::getline(ss, node1, ',') && std::getline(ss, node2, ',') && (ss >> cost);
            //almacenamos las aristas en el vector de aristas
            std::pair<char, char> edge(node1.at(0), node2.at(1));
            std::pair<std::pair<char, char>, int> temp(edge, cost);
            edges.push_back(temp);
        }else{
            //si la linea no contiene comas entonces debe ser una linea que contiene informacion sobre las heuristicas
            ss >> node && ss >> heuristic;
            //almacenamos las heuristicas
            std::pair<char, int> temp(node.at(0), heuristic);
            heuristics.push_back(temp);
        }
    }
    
    //operamos sobre el vector que contiene las heuristicas
    for (auto &&heuristic : heuristics)
    {
        //verificamos si el vertice de la heuristica se encuentra o no en el grafo
        auto iterator = graph.find(heuristic.first);
        //si no lo esta lo añadimos, creando un nodo y añadiendo su vector de aristas, el char identificador y el valor de la heuristica
        if (iterator == graph.end())
        {
            graph_node temp;
            vector<std::pair<char, int>> temp_vector;
            temp.identifier = heuristic.first;
            temp.heuristic_value = heuristic.second;
            temp.edges = temp_vector;
            graph.insert({heuristic.first, temp});
        }
    }

    //luego añadimos las aristas a la lista de aristas de cada vertice para la cual operamos sobre el vector de aristas
    for (auto &&edge : edges)
    {
        graph.at((edge.first.first)).edges.push_back(std::make_pair(edge.first.second, edge.second));
    }

    if (std::string(argv[1]) == "--d")
    {
        dfs(start, goal, graph);
    }

    if (std::string(argv[1]) == "--u")
    {
        uniform_cost_search(start, goal, graph);
    }

    if (std::string(argv[1]) == "--a")
    {
        a_star(start, goal, graph);
    }
    
    if (std::string(argv[1]) == "--g")
    {
        greedy(start, goal, graph);
    }
    return 0;
}
