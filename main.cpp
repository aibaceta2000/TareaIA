#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <map>

using namespace std;

void dfs(char start, char goal, std::map<char, std::vector<std::pair<char, int>>> graph, std::map<char, int> heuristic_values) {

}

int main(int argc, char const *argv[])
{
    /*std::vector<std::vector<std::pair<int,int>>> graph;*/
    std::map<char, std::vector<std::pair<char, int>>> graph;  
    std::map<char, int> heuristic_values;

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
        heuristic_values.insert({heuristic.first, heuristic.second});
        auto iterator = graph.find(heuristic.first);
        if (iterator == graph.end())
        {
            vector<std::pair<char, int>> temp;
            graph.insert({heuristic.first, temp});
        }
    }

    for (auto &&edge : edges)
    {
        graph.at(edge.first.first).push_back(std::make_pair(edge.first.second, edge.second));   
    }
    
    std::cout << graph.size() << std::endl;
    for (auto &&element : graph)
    {
        std::cout << element.first << ": ";
        for (auto &&edge : element.second)
        {
            std::cout<<edge.first << " " <<edge.second << " ";
        }
        std::cout << std::endl;
    }
    

    return 0;
}
