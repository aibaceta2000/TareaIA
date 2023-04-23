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
        std::cout << "current node: "<<get<0>(pq.top()) << " current distance: "<< get<1>(pq.top())<<" historial: ";
        for (auto &&temp : get<2>(pq.top()))
        {
            std::cout<<temp<<", ";
        }
        std::cout<<std::endl;
        
        pq.pop();
        if (get<0>(pq.top()) == goal)
        {
            std::cout << "current node: "<<get<0>(pq.top()) << " current distance: "<< get<1>(pq.top())<<" historial: ";
            for (auto &&temp : get<2>(pq.top()))
            {
                std::cout<<temp<<", ";
            }
            std::cout<<std::endl;            
        }
        if (get<0>(pq.top()) == goal)
        {
            final = pq.top();
        }            
    }
    std::cout << "final node: "<<get<0>(pq.top()) << " final distance: "<< get<1>(pq.top())<<" historial: ";
    for (auto &&temp : get<2>(pq.top()))
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

int main(int argc, char const *argv[])
{
    std::map<char, graph_node>  graph;

    std::string temp;
    char start, goal;
    std::cin >> temp >> start;
    std::cin >> temp >> goal;
    std::vector<std::pair<char, int>> heuristics;
    std::vector<std::pair<std::pair<char, char>, int>> edges;
    std::vector<std::string> lines;
    std::getline(std::cin, temp);
    while (std::getline(std::cin, temp))
    {
        lines.push_back(temp);
    }
    
    for (auto &&line : lines)
    {
        stringstream ss(line);
        std::string node, node1, node2;
        int cost, heuristic;
        if (line.find(',') != std::string::npos)
        {
            std::getline(ss, node1, ',') && std::getline(ss, node2, ',') && (ss >> cost);
            std::pair<char, char> edge(node1.at(0), node2.at(1));
            std::pair<std::pair<char, char>, int> temp(edge, cost);
            edges.push_back(temp);
        }else{
            ss >> node && ss >> heuristic;
            std::pair<char, int> temp(node.at(0), heuristic);
            heuristics.push_back(temp);
        }
    }
    
    for (auto &&heuristic : heuristics)
    {
        auto iterator = graph.find(heuristic.first);
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

    for (auto &&edge : edges)
    {
        graph.at((edge.first.first)).edges.push_back(std::make_pair(edge.first.second, edge.second));
    }
    
    /*
    for (auto &&node : graph)
    {
        std::cout << "node identifier: " << node.first << " heuristic value: " << node.second.heuristic_value << std::endl;
        std::cout << " edges:" <<std::endl;
        for (auto &&edge : node.second.edges)
        {
            std::cout<< "    " <<edge.first <<" " << edge.second <<std::endl;
        }
        
    }*/

    //dfs(start, goal, graph);
    uniform_cost_search(start, goal, graph);
    //greedy(start, goal, graph);
    /*std::vector<std::tuple<int,char>> nodos;
    nodos.push_back(std::make_tuple(5,'a'));
    nodos.push_back(std::make_tuple(6,'a'));
    nodos.push_back(std::make_tuple(7,'a'));
    nodos.push_back(std::make_tuple(8,'a'));

    std::priority_queue<std::tuple<int,char>, std::vector<std::tuple<int,char>>, std::function<bool(std::tuple<int,char>, std::tuple<int,char>)>> pq(custom_operator_tuple);
    pq.push(std::make_tuple(4,'a'));
    pq.push(std::make_tuple(8,'a'));
    pq.push(std::make_tuple(5,'a'));
    pq.push(std::make_tuple(1,'a'));
    pq.push(std::make_tuple(9,'a'));
    std::cout<<pq.size()<<std::endl;
    for (size_t i = 0; i < 5; i++)
    {
        std::tuple<int,char> temp = pq.top();
        pq.pop();
        std::cout<<get<0>(temp) << std::endl;
    } */
    
    
    return 0;
}
