#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <iomanip>
#include <algorithm>
#include <queue>

const auto INF = std::numeric_limits<int>::max();

struct NodeDistance
{
    double weight;
    std::size_t indexTo;
};

struct Node
{
    std::string label;
    std::string latitude;
    std::string longitude;

    std::vector<NodeDistance> edges;

    std::size_t prevIndex;
    double weight;
    bool visited;
};

struct Graph
{
    std::vector<Node> nodes;

    void clear()
    {
        nodes.clear();
    }

    void clear_edges()
    {
        for (auto& node : nodes)
        {
            node.edges.clear();
        }
    }

    void init_start_values()
    {
        for (auto& node : nodes)
        {
            node.weight = INF;
            node.visited = false;
            node.prevIndex = INF;
        }
    }

    double pow(double x, unsigned int y)
    {
        if (y == 0)
            return 1;
        else if (y % 2 == 0)
            return pow(x, y / 2) * pow(x, y / 2);
        else
            return x * pow(x, y / 2) * pow(x, y / 2);
    }
    
    double sqrt(double num)
    {
        double lower_bound=0; 
        double upper_bound=num;
        double temp=0;

        int nCount = 50;

        while(nCount != 0)
        {
               temp=(lower_bound+upper_bound)/2;
               if(temp*temp==num) 
               {
                   return temp;
               }
               else if(temp*temp > num)
               {
                   upper_bound = temp;
               }
               else
               {
                   lower_bound = temp;
               }
            nCount--;
         }
        return temp;
    }

    double dist(Node n1, Node n2)
    {
        return sqrt(pow((std::stod(n1.latitude) - std::stod(n2.latitude)),2) + pow((std::stod(n1.longitude) - std::stod(n2.longitude)),2));

    }
};

struct MinPriorityQueue
{
    double weight;
    std::size_t index;

    MinPriorityQueue(double w, std::size_t i)
    {
        weight = w;
        index = i;
    }
};

struct compare
{
  public:
  bool operator()(MinPriorityQueue& current, MinPriorityQueue& other)
  {
        return current.weight > other.weight;
  }
};

bool read_nodes(std::istream& istream, std::size_t nodes_count, Graph& graph_out)
{
    graph_out.nodes.clear();

    for (std::size_t i = 0; i < nodes_count; i++)
    {
        decltype(Node::label) label;
        decltype(Node::latitude) latitude;
        decltype(Node::longitude) longitude;

        istream >> label;
        istream >> latitude;
        istream >> longitude;

        graph_out.nodes.push_back({ label, latitude, longitude });
    }

    return true;
}

bool read_edges(std::istream& istream, std::size_t edges_count, Graph& graph_out)
{
    graph_out.clear_edges();

    for (std::size_t i = 0; i < edges_count; i++)
    {
        std::string label, adj_label;
        std::string str;
        double weight;

        istream >> label;
        auto start_iter = std::find_if(graph_out.nodes.begin(), graph_out.nodes.end(), [label](const auto& node) { return node.label == label; });
        std::size_t start_index = std::distance(graph_out.nodes.begin(), start_iter);

        getline(istream, str);
        std::istringstream ss(str);

        while (ss >> adj_label)
        {
            auto& nodes_ref = graph_out.nodes;
            auto end_iter = std::find_if(nodes_ref.begin(), nodes_ref.end(), [adj_label](const auto& node) { return node.label == adj_label; });
            std::size_t end_index = std::distance(graph_out.nodes.begin(), end_iter);

            if (start_iter == nodes_ref.end() || end_iter == nodes_ref.end())
            {
                graph_out.clear_edges();

                return false;
            }
            std::size_t index = (end_iter - nodes_ref.begin());

            weight = graph_out.dist(graph_out.nodes[start_index], graph_out.nodes[end_index]);
            (*start_iter).edges.push_back(NodeDistance{ weight, index });
        }
    }

    return true;
}

Graph read_graph(const std::string& file_path)
{
    Graph graph;
    std::ifstream fin(file_path);
    if (fin)
    {
        std::string sourceLabel;
        std::string destinationLabel;

        int numberOfNodes;

        fin >> sourceLabel >> destinationLabel >> numberOfNodes;

        if (read_nodes(fin, numberOfNodes, graph))
        {
            read_edges(fin, numberOfNodes, graph);
        }
    }
    return graph;
}

std::vector<std::size_t> convert_graph_to_path(Graph& graph, std::size_t start_index, std::size_t end_index)
{
    std::vector<std::size_t> result;
    std::stack<std::size_t> tmp_path;
    std::size_t current_node = end_index;
    while (current_node != INF)
    {
        tmp_path.push(current_node);
        current_node = graph.nodes[current_node].prevIndex;
    }
    while (!tmp_path.empty())
    {
        result.push_back(tmp_path.top());
        tmp_path.pop();
    }
    return result;
}

std::vector<std::size_t> find_path_Dijkstra(Graph& graph, std::size_t start_index, std::size_t end_index, std::string destination_label)
{
    graph.init_start_values();

    std::priority_queue<MinPriorityQueue, std::vector<MinPriorityQueue>, compare> min_weight_map;
    graph.nodes[start_index].weight = 0;
    min_weight_map.push({ 0, start_index });
    while (!min_weight_map.empty())
    {
        auto [current_weight, current_index] = (min_weight_map.top());

        min_weight_map.pop();

        if (graph.nodes[current_index].visited)
        {
            continue;
        }
        
        graph.nodes[current_index].visited = true;
        
        if (graph.nodes[current_index].label == destination_label)
        {
            break;
        }
        
        for (std::size_t i = 0; i < graph.nodes[current_index].edges.size(); i++)
        {
            std::size_t index_to = graph.nodes[current_index].edges[i].indexTo;
            double edged_weight = graph.nodes[current_index].edges[i].weight;
            if (!graph.nodes[index_to].visited)
            {
                if (current_weight + edged_weight < graph.nodes[index_to].weight )
                {
                    graph.nodes[index_to].weight = current_weight + edged_weight;
                    graph.nodes[index_to].prevIndex = current_index;
                    min_weight_map.push({ graph.nodes[index_to].weight, index_to });
                }
            }
        }

    }

    return convert_graph_to_path(graph, start_index, end_index);
}

int main()
{
    auto graph = read_graph("task3.in");
    std::ifstream fin("task3.in");

    std::string sourceLabel;
    std::string destinationLabel;
    int numberOfNodes;

    fin >> sourceLabel >> destinationLabel >> numberOfNodes;

    auto start_node = std::find_if(graph.nodes.begin(), graph.nodes.end(), [sourceLabel](const auto& node) { return node.label == sourceLabel; });
    std::size_t start_index = std::distance(graph.nodes.begin(), start_node);

    auto end_node = std::find_if(graph.nodes.begin(), graph.nodes.end(), [destinationLabel](const auto& node) { return node.label == destinationLabel; });
    std::size_t end_index = std::distance(graph.nodes.begin(), end_node);

    auto path = find_path_Dijkstra(graph, start_index, end_index, destinationLabel);

    std::fstream fout;
    fout.open("task3.out", std::ios::out);

    fout << std::fixed << std::setprecision(5) << graph.nodes[end_index].weight << std::endl;

    for (auto path_node_index : path)
    {
        fout << graph.nodes[path_node_index].label << " ";
    }

    fout << std::endl;

    int totalVisited = 0;

    for (int i=0; i < numberOfNodes; i++)
    {
        if (graph.nodes[i].visited == true) totalVisited++;
    }

   fout << totalVisited << std::endl;
}