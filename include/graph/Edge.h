#pragma once

template <class T> class Vertex;
template <class T> class Graph;


template <class T>
class Edge {
	Vertex<T>* dest;
	bool visited;
public:
	Edge(Vertex<T>* d);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T>* d) : dest(d), visited(false) {}