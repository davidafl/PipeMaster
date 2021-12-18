#pragma once

template <class T> class Edge;
template <class T> class Graph;

template <class T>
class Vertex {

	T data;
	std::vector<Edge<T>> adj;

	bool visited;  // for DFS
	bool starter; // is it a starting node for DFS

	void addEdge(Vertex<T>* dest);
	bool removeEdgeTo(Vertex<T>* d);

public:
	Vertex(T in, bool start);
	Vertex(const Vertex<T>& v);
	T getData() const;
	friend class Graph<T>;
};



template <class T>
T Vertex<T>::getData() const {
	return data;
}

template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T>* d) {
	typename std::vector<Edge<T> >::iterator it = adj.begin();
	typename std::vector<Edge<T> >::iterator ite = adj.end();
	while (it != ite) {
		if (it->dest == d) {
			adj.erase(it);
			return true;
		}
		else it++;
	}
	return false;
}

template <class T>
Vertex<T>::Vertex(T in, bool start) : data(in), visited(false), starter(start) {}


template <class T>
Vertex<T>::Vertex(const Vertex<T>& in) : data(in.data), visited(in.visited) {
}

template <class T>
void Vertex<T>::addEdge(Vertex<T>* dest) {
	Edge<T> edgeD(dest);
	adj.push_back(edgeD);
}

