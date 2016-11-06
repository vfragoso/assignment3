// Copyright (C) 2016 West Virginia University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of West Virginia University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// Author: Victor Fragoso (victor.fragoso@mail.wvu.edu)

#include "camera_utils.h"

#define _USE_MATH_DEFINES  // For using M_PI.
#include <cmath>
#include <Eigen/Core>
#include <GL/glew.h>

namespace wvu {
namespace {
// Mathematical constants. The right way to get PI in C++ is to use the
// macro M_PI. To do so, we have to include math.h or cmath and define
// the _USE_MATH_DEFINES macro to enable these constants. See the header
// section.
constexpr GLfloat kHalfPi = 0.5f * static_cast<GLfloat>(M_PI);

// Compute cotangent. Since C++ does not provide cotangent, we implement it
// as follows. Recall that cotangent is essentially tangent flipped and
// translated 90 degrees (or PI / 2 radians). To do the flipping and translation
// we have to do PI / 2 - angle. Subtracting the angle flips the curve.
// See the plots for http://mathworld.wolfram.com/Cotangent.html and
// http://mathworld.wolfram.com/Tangent.html
inline GLfloat ComputeCotangent(const GLfloat angle) {
  return tan(kHalfPi - angle);
}

}  // namespace

// Computes the perspective camera projection metrix.
// Params:
//   field_of_view  The field of view angle in radians.
//   aspect_ratio  The width / height ratio of the window dimensions.
//   near  The near distance plane.
//   far  The far distance plane.
Eigen::Matrix4f ComputePerspectiveProjectionMatrix(const GLfloat field_of_view,
                                                   const GLfloat aspect_ratio,
                                                   const GLfloat near,
                                                   const GLfloat far) {
  // Create the projection matrix.
  const GLfloat y_scale = ComputeCotangent(0.5f * field_of_view);
  const GLfloat x_scale = y_scale / aspect_ratio;
  const GLfloat planes_distance = far - near;
  const GLfloat z_scale =
      -(near + far) / planes_distance;
  const GLfloat homogeneous_scale =
      -2 * near * far / planes_distance;
  Eigen::Matrix4f projection_matrix;
  projection_matrix << x_scale, 0.0f, 0.0f, 0.0f,
      0.0f, y_scale, 0.0f, 0.0f,
      0.0f, 0.0f, z_scale, homogeneous_scale,
      0.0f, 0.0f, -1.0f, 0.0f;
  return projection_matrix;
}

}  // namespace wvu

