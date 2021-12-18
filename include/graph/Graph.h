#pragma once

#include <vector>
#include <list>
#include <queue>

#include "Edge.h"
#include "Vertex.h"

template <class T>
class Graph {

	std::vector<Vertex<T>*> vertices;
	void dfs(Vertex<T>* v, std::vector<T>& res) const;

public:

	bool addVertex(const T& in, bool start);
	bool addEdge(const T& source, const T& dest);
	bool removeVertex(const T& in);
	bool removeEdge(const T& source, const T& dest);
	void clear();
	std::vector<T> dfs() const;

	std::vector<Vertex<T>* > getVertices() const;
	int getNumVertex() const;

};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertices.size();
}

template <class T>
void Graph<T>::clear() {
	vertices.clear();
}

template <class T>
std::vector<Vertex<T>* > Graph<T>::getVertices() const {
	return vertices;
}

// add vertex in graph
// start is a boolean to mark starting nodes for DFS
template <class T>
bool Graph<T>::addVertex(const T& in, bool start) {
	typename std::vector<Vertex<T>*>::iterator it = vertices.begin();
	typename std::vector<Vertex<T>*>::iterator ite = vertices.end();
	for (; it != ite; it++)
		if ((*it)->data == in) return false;
	Vertex<T>* v1 = new Vertex<T>(in, start);
	vertices.push_back(v1);
	return true;
}

// delete vertex - not used but expected !
template <class T>
bool Graph<T>::removeVertex(const T& in) {
	typename std::vector<Vertex<T>*>::iterator it = vertices.begin();
	typename std::vector<Vertex<T>*>::iterator ite = vertices.end();
	for (; it != ite; it++) {
		if ((*it)->info == in) {
			Vertex<T>* v = *it;
			vertices.erase(it);
			typename std::vector<Vertex<T>*>::iterator it1 = vertices.begin();
			typename std::vector<Vertex<T>*>::iterator it1e = vertices.end();
			for (; it1 != it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}
			delete v;
			return true;
		}
	}
	return false;
}

// create an edge between to vertices
template <class T>
bool Graph<T>::addEdge(const T& source, const T& dest) {
	typename std::vector<Vertex<T>*>::iterator it = vertices.begin();
	typename std::vector<Vertex<T>*>::iterator ite = vertices.end();
	int found = 0;
	Vertex<T>* edgeS = NULL, * edgeD = NULL;
	while (found != 2 && it != ite) {
		if ((*it)->data == source)
		{
			edgeS = *it; found++;
		}
		if ((*it)->data == dest)
		{
			edgeD = *it; found++;
		}
		it++;
	}
	if (found != 2) return false;
	edgeS->addEdge(edgeD);
	return true;
}

// delete edge between 2 vertices
template <class T>
bool Graph<T>::removeEdge(const T& source, const T& dest) {
	typename std::vector<Vertex<T>*>::iterator it = vertices.begin();
	typename std::vector<Vertex<T>*>::iterator ite = vertices.end();
	int found = 0;
	Vertex<T>* edgeS = NULL, * edgeD = NULL;
	while (found != 2 && it != ite) {
		if ((*it)->data == source)
		{
			edgeS = *it; found++;
		}
		if ((*it)->data == dest)
		{
			edgeD = *it; found++;
		}
		it++;
	}
	if (found != 2) return false;
	return edgeS->removeEdgeTo(edgeD);
}

// RUN DFS and return the list of nodes visited
template <class T>
std::vector<T> Graph<T>::dfs() const {

	typename std::vector<Vertex<T>*>::const_iterator it = vertices.begin();
	typename std::vector<Vertex<T>*>::const_iterator ite = vertices.end();

	// init 
	for (; it != ite; it++)
		(*it)->visited = false;

	// the result
	std::vector<T> res;

	it = vertices.begin();

	for (; it != ite; it++)
		if ( (*it)->starter == true &&   (*it)->visited == false)
			dfs(*it, res);
	return res;
}

// the actual recursion of DFS
template <class T>
void Graph<T>::dfs(Vertex<T>* v, std::vector<T>& res) const {
	v->visited = true;
	res.push_back(v->data);
	typename std::vector<Edge<T> >::iterator it = (v->adj).begin();
	typename std::vector<Edge<T> >::iterator ite = (v->adj).end();
	for (; it != ite; it++)
		if (it->dest->visited == false)
			dfs(it->dest, res);
}


