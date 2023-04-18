#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <map>
#include <random>
#include <algorithm>

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
    while (current_node != goal)
    {
       history.push_back(current_node);
       std::sort(graph.at(current_node).edges.begin(),graph.at(current_node).edges.end(),custom_operator);
       total += graph.at(current_node).edges.at(0).second;
       current_node = graph.at(current_node).edges.at(0).first;
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
    return 0;
}
