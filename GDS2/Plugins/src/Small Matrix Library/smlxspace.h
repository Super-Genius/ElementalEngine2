///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                INTEL CORPORATION PROPRIETARY INFORMATION                  //
//                                                                           //
//  This program has been developed by Intel Corporation.                    //
//  You have Intel's permission to incorporate this code into your product,  //
//  royalty free.  Intel has various intellectual property rights            //
//  which it may assert under certain circumstances.                         //            
//                                                                           //
//  Intel specifically disclaims all warranties, express or implied,         //
//  and all liability, including consequential and other indirect damages,   //
//  for the use of this code, including liability for infringement           //
//  of any proprietary rights, and including the warranties                  //
//  of merchantability and fitness for a particular purpose.                 //
//  Intel does not assume any responsibility for any errors which may        //
//  appear in this code nor any responsibility to update it.                 //
//                                                                           //
//         Copyright (c) 1998 Intel Corporation. All Rights Reserved.        //
//                                                                           //
//  Purpose:                                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef SMLXSPACE_H
#define SMLXSPACE_H

#include <smlxmatrix.h>

///////////////////
// Transformations.
///////////////////

namespace JointType {
	enum { SIXDOF, REVOLUTE, PRISMATIC, SPHERICAL, PLANAR };
}

// Transpose(Phi) * Matrix6X6
SMLLIBENTRY SMLXTmpMatrix& MotionSpaceRevolute (SMLXTmpMatrix &A);
SMLLIBENTRY SMLXTmpMatrix& MotionSpaceSpherical(SMLXTmpMatrix &A);
SMLLIBENTRY SMLXTmpMatrix& MotionSpaceSixDOF   (SMLXTmpMatrix &A);
SMLLIBENTRY SMLXTmpMatrix& MotionSpacePrismatic(SMLXTmpMatrix &A);

// Phi * Vector
SMLLIBENTRY SMLXTmpMatrix& WorldSpaceRevolute (const SMLXSpatialVector &A);
SMLLIBENTRY SMLXTmpMatrix& WorldSpaceSpherical(const SMLXSpatialVector &A);
SMLLIBENTRY SMLXTmpMatrix& WorldSpaceSixDOF   (const SMLXSpatialVector &A);
SMLLIBENTRY SMLXTmpMatrix& WorldSpacePrismatic(const SMLXSpatialVector &A);

// Phi * Matrix
SMLLIBENTRY SMLXTmpMatrix& WorldSpaceRevolute (SMLXMatrix &A);
SMLLIBENTRY SMLXTmpMatrix& WorldSpaceSpherical(SMLXMatrix &A);
SMLLIBENTRY SMLXTmpMatrix& WorldSpaceSixDOF   (SMLXMatrix &A);
SMLLIBENTRY SMLXTmpMatrix& WorldSpacePrismatic(SMLXMatrix &A);

#endif
