//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-05 16:37:32 taubin>
//------------------------------------------------------------------------
//
// dgpPrt.cpp
//
// Software developed for the course
// Digital Geometry Processing
// Copyright (c) 2025, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "dgpPrt.hpp"

#include "core/faces.hpp"

const char* tv(bool value) { return (value)?"true":"false"; }

void printIndexedFaceSetInfo
(ostream& ostr, const string& shapeName,
 const int& iIfs, IndexedFaceSet& ifs, const string& indent) {

  // print some information about ifs using this format
  //
  //  IndexedFaceSet[0] {
  //    shapeName        = "SURFACE"
  //    numberOfVertices = 34835
  //    numberOfFaces    = 69473
  //    isTriangleMesh   = true
  //    colorBinding     = NONE
  //    normalBinding    = PER_FACE
  //    texCoordBinding  = NONE
  //  }

  ostr << indent << "IndexedFaceSet[" << iIfs << "] {" << endl;

  // TODO
  
  int nV = ifs->getNumberOfVertices();
  
  int nF = ifs->getNumberOfFaces();
  
  isTriangleMesh = ifs->isTriangleMesh();
  
  string colorBinding = ifs->stringBinding(ifs->getColorBinding());
  
  string normalBinding = ifs->stringBinding(ifs->getNormalBinding());
  
  string texCoordBinding = ifs->stringBinding(ifs->getTexCoordBinding());
  
  ostr << indent << "  shapeName        = \"" << shapeName << "\"" << endl;
  ostr << indent << "  numberOfVertices = " << nV << endl;
  ostr << indent << "  numberOfFaces    = " << nF << endl;
  ostr << indent << "  isTriangleMesh   = " << isTriangleMesh << endl;
  ostr << indent << "  colorBinding     = " << colorBinding << endl;
  ostr << indent << "  normalBinding    = " << colorBinding << endl;
  ostr << indent << "  texCoordBinding  = " << colorBinding << endl;

  ostr << indent << "}" << endl;

}
