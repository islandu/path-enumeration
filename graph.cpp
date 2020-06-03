//
// islandu/
//   path-enumeration/
//     graph.cpp
//
// Daniel Thompson, 2020
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Graph {

public:

  Graph( const std::string filename );
  ~Graph();

  void showMatrix();
  bool getEdge( const int a, const int b );
  std::vector<int> getStartVertices();
  std::vector<int> getEndVertices();
  std::vector<int> getAdjacents( const int v );

  void pathsBetween(
    std::vector< std::vector<int> > &dest,
    const int current,
    const int end,
    bool *touched,
    int *path,
    int &pathPos
  );

  void enumeratePaths( std::vector< std::vector<int> > &dest );

private:

  bool *matrix; // Adjacency matrix (flat array)
  int n; // Number of vertices
};

Graph::Graph( const std::string filename )
{
  std::ifstream file;
  file.open( filename );

  // Read first line of file, number of vertices in graph
  file >> this->n;

  // Allocate space for array
  int matrixSize = this->n * this->n;
  this->matrix = new bool[ matrixSize ];

  // Populate array with values from the file's adjacency matrix
  for ( int i=0; i < matrixSize; ++i )
    file >> this->matrix[i];

  file.close();
}

Graph::~Graph()
{
  delete[] this->matrix;
}

void Graph::showMatrix()
{
  for ( int i=0; i < this->n; ++i ) {
    for ( int j=0; j < this->n; ++j ) {
      std::cout << this->getEdge( i, j ) << " ";
    }
    std::cout << std::endl;
  }
}

bool Graph::getEdge( const int a, const int b )
{
  return this->matrix[ a * this->n + b ];
}

std::vector<int> Graph::getStartVertices()
{
  std::vector<int> result;
  result.reserve( this->n );

  for ( int i=0; i < this->n; ++i ) {
    bool isStart=true;

    for ( int j=0; j < this->n; ++j ) {
      if ( this->getEdge( j, i ) ) {
        isStart = false;
        break;
      }
    }

    if ( isStart )
      result.push_back( i );
  }

  result.shrink_to_fit();
  return result;
}

std::vector<int> Graph::getEndVertices()
{
  std::vector<int> result;
  result.reserve( this->n );

  for ( int i=0; i < this->n; ++i ) {
    bool isEnd=true;

    for ( int j=0; j < this->n; ++j ) {
      if ( this->getEdge( i, j ) ) {
        isEnd = false;
        break;
      }
    }

    if ( isEnd )
      result.push_back( i );
  }

  result.shrink_to_fit();
  return result;
}

std::vector<int> Graph::getAdjacents( const int v )
{
  std::vector<int> result;
  result.reserve( 12 );

  for ( int i=0; i < this->n; ++i ) {
    if ( this->getEdge( v, i ) )
      result.push_back( i );
  }

  result.shrink_to_fit();
  return result;
}

void Graph::enumeratePaths( std::vector< std::vector<int> > &dest )
{
  // Search matrix for all start & end vertices
  auto startVertices = this->getStartVertices();
  auto endVertices = this->getEndVertices();

  // Find paths for every pair of start & end vertices
  for ( const int start : startVertices ) {
    for ( const int end : endVertices ) {
      // Path array and index marker
      int *path = new int[ this->n ];
      int pathIndex=0;

      // Array indicating vertices that have been traversed
      bool *touched = new bool[ this->n ];
      for ( int i=0; i < this->n; ++i )
        touched[i] = false;

      // Search for paths between start and end
      this->pathsBetween( dest, start, end, touched, path, pathIndex );

      delete[] path;
      delete[] touched;
    }
  }
}

void Graph::pathsBetween(
  std::vector< std::vector<int> > &dest,
  const int current,
  const int end,
  bool *touched,
  int *path,
  int &pathPos
)
{
  // Add current vertex to path
  touched[ current ] = true;
  path[ pathPos++ ] = current;

  if ( current == end ) {
    // Store path in vector and add to vector of paths (dest)
    std::vector<int> pathVector( pathPos );
    for ( int i=0; i < pathPos; ++i )
      pathVector[i] = path[i];
    dest.push_back( pathVector );
  }
  else {
    // Search adjacent vertices until end reached
    auto adjacents = this->getAdjacents( current );
    for ( const int vertex : adjacents ) {
      if ( !touched[ vertex ] )
        this->pathsBetween( dest, vertex, end, touched, path, pathPos );
    }
  }

  --pathPos;
  touched[ current ] = false;
}

// Test program
int main()
{
  // Destination vector for paths
  std::vector< std::vector<int> > allPaths;

  // Instantiating test graph 1
  Graph graph1( "/Users/DT/Code/Projects/path-enumeration/test-graphs/test1.txt" );

  // Populating vector with all possible paths
  graph1.enumeratePaths( allPaths );

  // Print results
  std::cout << "Test graph 1 paths..." << std::endl;
  for ( const std::vector<int> &p : allPaths ) {
    for ( const int v : p )
      std::cout << v << " ";
    std::cout << std::endl;
  }

  allPaths.clear();
  std::cout << std::endl;

  // Instantiating test graph 2
  Graph graph2( "/Users/DT/Code/Projects/path-enumeration/test-graphs/test2.txt" );

  // Populating vector with all possible paths
  graph2.enumeratePaths( allPaths );

  // Print results
  std::cout << "Test graph 2 paths..." << std::endl;
  for ( const std::vector<int> &p : allPaths ) {
    for ( const int v : p )
      std::cout << v << " ";
    std::cout << std::endl;
  }

  return 0;
}
