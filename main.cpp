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

//nodo del grafo, este contiene el identificador, el valor de la heurisitica y un vector de aristas

struct graph_node{
    char identifier;
    int heuristic_value;
    std::vector<std::pair<char, int>> edges;
};

//operadores custom para comparar tuplas
bool custom_operator(tuple<char, int, int> x, tuple<char, int, int> y)
{
    return get<1>(x) < get<1>(y); 
}

bool custom_operator_uniform_cost_search(std::tuple<char, int, std::vector<char>> x, std::tuple<char, int, std::vector<char>> y)
{
    return get<1>(x) > get<1>(y); 
}

bool custom_operator_a_star(std::tuple<char, int, std::vector<char>, int> x, std::tuple<char, int, std::vector<char>, int> y)
{
    return get<3>(x) > get<3>(y); 
}

bool custom_operator_tuple(tuple<int, char> x, tuple<int, char> y)
{
    return get<0>(x) > get<0>(y); 
}

//generador de numeros random entre dos valores a y b
int rand_generator(int a, int b) {
    std::random_device rd;
    std::mt19937 e(rd());
    std::uniform_int_distribution<int> dist(a, b);
    int succesor = dist(e);
    return succesor;
}

//codigo para imprimir la solucion de acuerdo a lo pedido
void print_solution(std::vector<char> history, int costo_total, std::map<char, int>  expansion_history){
    for (size_t i = 0; i < history.size(); i++)
    {
        std::cout<<history.at(i);
        if (i != history.size() - 1) 
        {
            std::cout<<" -> ";
        }
    }
    std::cout << std::endl;
    std::cout<<"Costo: "<<costo_total<<std::endl;
    for (auto &&element : expansion_history)
    {
        if (std::find(history.begin(), history.end(), element.first) != history.end())
        {
            std::cout<< element.first <<": " <<element.second << std::endl;
        }
    } 
}

//algoritmo dfs
void dfs(char start, char goal, std::map<char, graph_node>  graph) {
    //comienza con el nodo de entrada
    char current_node = start;
    vector<char> history;
    int total = 0;
    std::map<char, int>  expansion_history;
    while (current_node != goal)
    {
       //se guarda el nodo actual en el historial de expansion para contar la cantidad de veces que se expandio cada nodo
       auto iterator = expansion_history.find(current_node);
       if (iterator== expansion_history.end())
       {
            expansion_history.insert({current_node, 1});
       }else{
            expansion_history.at(current_node)= expansion_history.at(current_node) + 1;
       }
       //y se guarda en el historial 
       history.push_back(current_node);
       //a partir de la cantidad de aristas del nodo actual se genera un numero random y se escoge un sucesor en la lista de aristas a partir de ese
       //numero random generado, ademas se va guardando la distancia total que se lleva
       int size = graph.at(current_node).edges.size();
       int successor = rand_generator(0, size - 1);
       total += graph.at(current_node).edges.at(successor).second;
       //se cambia el nodo actual por el siguiente nodo
       current_node = graph.at(current_node).edges.at(successor).first;
       //si se llego al sucesor se almacena en el historial y en el historial de expansiones con un values de 0 ya que no se expande
       if(current_node == goal)
       {
        history.push_back(current_node);
        expansion_history.insert({current_node, 0});
       }
    }
    //se imprime la solucion con la funcion print solution
    print_solution(history, total, expansion_history);
}

void uniform_cost_search(char start, char goal, std::map<char, graph_node>  graph){
    char current_node = start;
    vector<char> history;
    int total = 0;
    //para el uniform cost search se crea una priority queue de tuplas de tres elementos que consisten del identificador del vertice, 
    //el costo hasta el momento y el historial de cada tupla, se ordenan de forma custom por el costo total hasta el momento
    std::priority_queue<std::tuple<char, int, std::vector<char>>, std::vector<std::tuple<char, int, std::vector<char>>>, std::function<bool(std::tuple<char, int, std::vector<char>>, std::tuple<char, int, std::vector<char>>)>> pq(custom_operator_uniform_cost_search);
    history.push_back(current_node);
    pq.push(std::make_tuple(graph.at(current_node).identifier, 0, history));
    //se crean dos tuplas final y current para almacentar cuando se llegue al objetivos y el nodo actual en el que se esta respectivamente
    std::tuple<char, int, std::vector<char>> final;
    std::tuple<char, int, std::vector<char>> current;
    std::map<char, int>  expansion_history;
    while (get<0>(pq.top()) != goal)
    {
        //al igual que antes se va añadiendo el historial de expansion
        auto iterator = expansion_history.find(get<0>(pq.top()));
        if (iterator== expansion_history.end())
        {
            expansion_history.insert({get<0>(pq.top()), 1});
        }else{
            expansion_history.at(get<0>(pq.top()))= expansion_history.at(get<0>(pq.top())) + 1;
        }
        current = pq.top();
        pq.pop();
        //despues de hacerle pop a la queue se almacenan en forma de tuplas todas las aristas del nodo actual, se incluye su histial hasta el momento
        //y se va actualizando el costo actual de cada vertice que se añade
        for (auto &&nd : graph.at(get<0>(current)).edges)
        {
            std::vector<char> temp = get<2>(current);
            temp.push_back(nd.first);
            pq.push(std::make_tuple(nd.first, get<1>(current) + nd.second, temp));
        }
        //si el nodo es el objetivo se almacena y se añade al historial de expansion con un contador de 0
        if (get<0>(pq.top()) == goal)
        {
            final = pq.top();
            expansion_history.insert({get<0>(pq.top()), 0});
        }            
    }
    //se imprime la solucion
    print_solution(get<2>(final), get<1>(final), expansion_history); 
}

void a_star(char start, char goal, std::map<char, graph_node>  graph){
    char current_node = start;
    vector<char> history;
    //al igual que antes se usa una priority queue para almacenar los vertices con la diferencia de que en esta ocasion se
    //añade un cuarto elemento que almacena la funcion de evaluacion de cada tupla y ademas la priority queue en se ordena en
    //base a este ultimo elemento
    std::priority_queue<std::tuple<char, int, std::vector<char>, int>, std::vector<std::tuple<char, int, std::vector<char>, int>>, std::function<bool(std::tuple<char, int, std::vector<char>, int>, std::tuple<char, int, std::vector<char>, int>)>> pq(custom_operator_a_star);
    history.push_back(current_node);    
    pq.push(std::make_tuple(graph.at(current_node).identifier, 0, history, 0));
    std::tuple<char, int, std::vector<char>, int> final;
    std::tuple<char, int, std::vector<char>, int> current;
    std::map<char, int>  expansion_history;
    //por cada iteracion se verifica si el elemento en el top de la queue es o no el nodo objetivos
    while (get<0>(pq.top()) != goal)
    {
        //al igual que antes se modifica el historial de expansion
        auto iterator = expansion_history.find(get<0>(pq.top()));
        if (iterator== expansion_history.end())
        {
            expansion_history.insert({get<0>(pq.top()), 1});
        }else{
            expansion_history.at(get<0>(pq.top()))= expansion_history.at(get<0>(pq.top())) + 1;
        }
        current = pq.top();
        pq.pop();
        //al igual que antes despues de hacer pop a la queue se van añadiendo nuevas tuplas al vector y por cada una se añade su identificador
        //su costo hasta el momento, su historial y la funcion de evaluacion
        for (auto &&nd : graph.at(get<0>(current)).edges)
        {
            std::vector<char> temp = get<2>(current);
            temp.push_back(nd.first);
            pq.push(std::make_tuple(nd.first, get<1>(current) + nd.second, temp, get<1>(current) + nd.second + graph.at(nd.first).heuristic_value));
        }
        //si el elemento en el top de la priority queue es el nodo final, se almacena y se añade al historial de expansion con un valor de 0
        if (get<0>(pq.top()) == goal)
        {
            final = pq.top();
            expansion_history.insert({get<0>(pq.top()), 0});
        }      
    }
    //se imprime la solucion
    print_solution(get<2>(final), get<1>(final), expansion_history);         
}

void greedy(char start, char goal, std::map<char, graph_node>  graph){
    char current_node = start;
    vector<char> history;
    int total = 0;
    std::map<char, int>  expansion_history;
    while (current_node != goal)
    {
        //al igual que antes se van añadiendo los vertices al hisrial de expansion
       auto iterator = expansion_history.find(current_node);
       if (iterator== expansion_history.end())
       {
            expansion_history.insert({current_node, 1});
       }else{
            expansion_history.at(current_node)= expansion_history.at(current_node) + 1;
       }
       //tambien se van añadiendo los nodos al historial
       history.push_back(current_node);
       //se crea un vector que se usara para ordenar los vertices de acuerdo al valor de sus heuristicas
       std::vector<std::tuple<char, int, int>> heuristic_compare;   
       //se recorre la lista de adyasencia de el nodo actual y se añade su heuristica, su identificador y su costo para ir sumando el costo total
       //al vector heuristic_compare
       for (auto &&node : graph.at(current_node).edges)
       {
            heuristic_compare.push_back(std::make_tuple(node.first, graph.at(node.first).heuristic_value, node.second));
       }
       //se ordena el vector heuristica_compare para asi encontrar el elemento con la menor heuristica
       std::sort(heuristic_compare.begin(),heuristic_compare.end(),custom_operator);
       //el nodo con la menor heuristica se convierte en el nodo actual
       current_node = get<0>(heuristic_compare.at(0));
       //y se va sumando el costo total hasta el momento
       total += get<2>(heuristic_compare.at(0));
       //si el nodo actual es el nodo objetivo se almacena en el historial y en la lista de expansion con un valor de 0 
       if (current_node == goal)
       {
            history.push_back(current_node);
            expansion_history.insert({current_node, 0});
       }  
    }
    //se imprime la solucion
    print_solution(history, total, expansion_history);
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
