//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-05 16:34:29 taubin>
//------------------------------------------------------------------------
//
// PolygonMesh.cpp
//
// Software developed for the course
// Digital Geometry Processing
// Copyright (c) 2025, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//     * Neither the name of the Brown University nor the names of its
//       contributors may be used to endorse or promote products
//       derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GABRIEL
// TAUBIN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.

#include <iostream>
#include "PolygonMesh.hpp"
#include "Partition.hpp"

PolygonMesh::PolygonMesh(const int nVertices, const vector<int>& coordIndex):
  HalfEdges(nVertices,coordIndex),
  _nPartsVertex(),
  _isBoundaryVertex()
{
  int nV = getNumberOfVertices();
  int nE = getNumberOfEdges(); // Edges method
  // int nF = getNumberOfFaces();
  int nC = getNumberOfCorners();

  // 1) classify the vertices as boundary or internal
  int iV;
  for(iV=0;iV<nV;iV++)
    _isBoundaryVertex.push_back(false);
  // TODO
  // - for edge boundary iE label its two end vertices as boundary
  for(int iE=0; iE<nE; iE++){
      int nIncidentFaces = getNumberOfEdgeHalfEdges(iE);
      if(nIncidentFaces == 1){      //La arista es borde sii tiene una sola cara incidente (i.e., un solo half-edge incidente)
          int V0 = getVertex0(iE);
          int V1 = getVertex1(iE);
          _isBoundaryVertex[V0] = true;
          _isBoundaryVertex[V1] = true;
      }
  }
  
  // 2) create a partition of the corners in the stack
  Partition partition(nC);
  // 3) for each regular edge
  //    - get the two half edges incident to the edge
  //    - join the two pairs of corresponding corners accross the edge
  //    - you need to take into account the relative orientation of
  //      the two incident half edges

  for(int iE=0; iE<nE; iE++){
      int nIncidentFaces = getNumberOfEdgeHalfEdges(iE);
      if(nIncidentFaces == 2){      //La arista es regular sii tiene dos caras incidentes
          int C0 = getEdgeHalfEdge(iE, 0);
          int C1 = getEdgeHalfEdge(iE, 1);
          if(coordIndex[C0] == coordIndex[getNext(C1)]){
              // Las esquinas incidentes a iE, C0 y C1, estan consistentemente orientadas
              partition.join(C0, getNext(C1));
              partition.join(C1, getNext(C0));
          } else {
              //Las esquinas incidentes a iE, c0 y c1, no están consistentemente orientadas
              partition.join(C0, C1);
              partition.join(getNext(C0), getNext(C1));
          }

      }
  }

  // consistently oriented
  /* \                  / */
  /*  \ iC01 <-- iC00  /  */
  /*   X ---- iE ---- X   */
  /*  / iC10 --> iC11  \  */
  /* /                  \ */

  // oposite orientation
  /* \                  / */
  /*  \ iC01 --> iC00  /  */
  /*   X ---- iE ---- X   */
  /*  / iC10 --> iC11  \  */
  /* /                  \ */

  // a decision has to be made about inconsistently oriented faces
  // incident to the same edge, as well as how to deal with singular
  // edges; for the moment let's assume that the mesh does not have
  // singular edges, and that pairs of corners corresponding to the
  // same vertex across inconsistently oriented faces will be joined

  // note that the partition will end up with the corner separators as
  // singletons, but it doesn't matter for the last step, and
  // the partition will be deleteted upon return
  
  // 4) count number of parts per vertex
  //    - initialize _nPartsVertex array to 0's
  //    - for each corner iC which is a representative of its subset, 
  //    - get the corresponding vertex index iV and increment _nPartsVertex[iV]
  //    - note that all the corners in each subset share a common
  //      vertex index, but multiple subsets may correspond to the
  //      same vertex index, indicating that the vertex is singular
  for(int i=0; i<nV; i++){
      _nPartsVertex.push_back(0);
  }
  for(int iC=0; iC<nC; iC++){
      if(partition.find(iC)==iC && coordIndex[iC]>=0){
          iV = coordIndex[iC];
          _nPartsVertex[iV]++;
      }
  }
}

int PolygonMesh::getNumberOfFaces() const {
  // TODO
  int nF = 0;
  for(int i=0; i<getNumberOfCorners(); i++){
      if(_coordIndex[i]<0) nF++;
  }
  return nF;
}

int PolygonMesh::getNumberOfEdgeFaces(const int iE) const {
  return getNumberOfEdgeHalfEdges(iE);
}

int PolygonMesh::getEdgeFace(const int iE, const int j) const {
  // TODO
  int iC = getEdgeHalfEdge(iE, j);
  return getFace(iC);               //Si los parámetros son inválidos, getEdgeHalfEdge retorna -1 y getFace(-1) retorna -1
}

bool PolygonMesh::isEdgeFace(const int iE, const int iF) const {
  // TODO
  bool result = false;
  int nHE = getNumberOfEdgeHalfEdges(iE);
  for(int i=0; i<nHE; i++){
      int iC = getEdgeHalfEdge(iE, i);
      if(getFace(iC)==iF){
          result = true;
          break;
      }
  }
  return result;
}

// classification of edges

bool PolygonMesh::isBoundaryEdge(const int iE) const {
  // TODO
  return (getNumberOfEdgeFaces(iE)==1);
}

bool PolygonMesh::isRegularEdge(const int iE) const {
  // TODO
  return (getNumberOfEdgeFaces(iE)==2);
}

bool PolygonMesh::isSingularEdge(const int iE) const {
  // TODO
  return (getNumberOfEdgeFaces(iE)>=3);
}

// classification of vertices

bool PolygonMesh::isBoundaryVertex(const int iV) const {
  int nV = getNumberOfVertices();
  return (0<=iV && iV<nV)?_isBoundaryVertex[iV]:false;
}

bool PolygonMesh::isInternalVertex(const int iV) const{
  int nV = getNumberOfVertices();
  return (0<=iV && iV<nV)?(!isBoundaryVertex(iV)):false;
}

bool PolygonMesh::isSingularVertex(const int iV) const {
  int nV = getNumberOfVertices();
  return ((0<=iV && iV<nV) && _nPartsVertex[iV]>1);
}

// properties of the whole mesh

bool PolygonMesh::isRegular() const {
  // TODO
  bool result = true;
  int nE = getNumberOfEdges();
  int nV = getNumberOfVertices();
  for(int iE=0; iE<nE; iE++){
      if(isSingularEdge(iE)){
          result = false;
          break;
      }
  }
  if(result){
      for(int iV=0; iV<nV; iV++){
          if(isSingularVertex(iV)){
              result = false;
              break;
          }
      }
  }
  return result;
}

bool PolygonMesh::hasBoundary() const {
  // TODO
  bool result = false;
  int nE = getNumberOfEdges();
  for(int iE=0; iE<nE; iE++){
      if(isBoundaryEdge(iE)){
          result = true;
          break;
      }
  }
  return result;
}
