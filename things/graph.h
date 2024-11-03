#include <algorithm>
#include <list>
#include <vector>
#include <stack>
#include <string>

namespace graph {

using adjList = std::list<size_t>;

class Graph {
public:
    Graph(int V) : 
        m_adj(V, adjList())
    {}

    Graph(const Graph& other) :
        m_adj(other.m_adj),
        E(other.E)
    {}

    Graph(Graph&& other) :
        m_adj(std::move(other.m_adj)),
        E(other.E)
    {}

    Graph& operator=(const Graph& other) {
        m_adj = other.m_adj;
        E = other.E; 
    }

    Graph& operator=(Graph&& other) {
        m_adj = std::move(other.m_adj);
        E = other.E; 
    }
    
    bool contain(size_t v) const {
        return v < m_adj.size();
    }

    void addEdge(size_t v, size_t w) {
        if (std::find(m_adj[v].begin(), m_adj[v].end(), w) != m_adj[v].end() ||
            w >= m_adj.size() ||
            v >= m_adj.size()) 
        {
            return;
        }
        m_adj[v].push_back(w);
        m_adj[w].push_back(v);
        E++;
    }

    const adjList& adjAt(size_t v) const {
        return m_adj[v];
    }

    size_t V() const {
        return m_adj.size();
    }

private:
    size_t E = 0;
    std::vector<adjList> m_adj;
};


class Path {
public:
    Path(const Graph& g, size_t src) :
        marked(g.V()),
        edgeTo(g.V()),
        m_src(src),
        graph(g)
    {
        if (!g.contain(src)) {
            throw "Graph does not contain a vertex" + std::to_string(src);
        }
    }

    bool hasPathTo(size_t v) const {
        return marked.at(v);
    }

    size_t srs() const {
        return m_src;
    }

    std::list<size_t> getPathTo(size_t v) {
        std::list<size_t> path;
        if (graph.contain(v)) {
            return path;
        }
        for (size_t x = v; x != m_src; x = edgeTo[x]) {
            path.push_back(x);
        }
        path.push_back(m_src);
        return path;
    }

    void dfs(const Graph& g, size_t src) {
        std::stack<size_t> stack;
        for (const auto& n : g.adjAt(src)) {
            stack.push(n);
        }

        size_t v;
        while (stack.size()) {
            v = stack.top(); 
            stack.pop();
            for (const auto& s : g.adjAt(v)) {
                if (!marked[s]) {
                    marked[s] = true;
                    edgeTo[s] = v;
                    stack.push(s);
                }
            }
        }
    }

private:
    std::vector<bool> marked;
    std::vector<size_t> edgeTo;
    const Graph& graph;
    size_t m_src;
};


}