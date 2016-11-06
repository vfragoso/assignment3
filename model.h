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

// Header guards below. This prevents to include in an infinite loop the header
// file. This prevents compilation errors.
#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <Eigen/Core>
#include <GL/glew.h>

namespace wvu {
// Class that holds the necessary information of a 3D model in OpenGL.
class Model {
public:
  // Constructor.
  // Params
  //  orientation  Axis of rotation whose norm is the angle
  //     (aka Rodrigues vector).
  //  position  The position of the object in the world.
  //  vertices  The vertices forming the object.
  Model(const Eigen::Vector3f& orientation,
        const Eigen::Vector3f& position,
        const Eigen::MatrixXf& vertices);

  // Constructor.
  // Params
  //  orientation  Axis of rotation whose norm is the angle
  //     (aka Rodrigues vector).
  //  position  The position of the object in the world.
  //  vertices  The vertices forming the object.
  //  indices  Indices for EBO.
  Model(const Eigen::Vector3f& orientation,
        const Eigen::Vector3f& position,
        const Eigen::MatrixXf& vertices,
        const std::vector<GLuint>& indices);

  // Destructor.
  // NOTE: Destructors need to be called when instances of this class are
  // created in the heap by using new operator.
  ~Model();

  // Builds the model matrix from the orientation and position members.
  Eigen::Matrix4f ComputeModelMatrix() const;

  // Sets the VAO, VBO and EBO.
  void SetVerticesIntoGpu();

  // Draws the model. Executes OpenGL calls to render the set VAO.
  // Params:
  //   projection  The camera projection matrix.
  //   view  The camera pose matrix (world -> camera transformation matrix).
  void Draw(const Eigen::Matrix4f& projection,
            const Eigen::Matrix4f& view);

  // Sets the orientation or pose of the object using the Rodrigues
  // vector: angle-axis vector where the angle is the norm of the vector.
  void set_orientation(const Eigen::Vector3f& orientation);

  // Sets the position of the model.
  void set_position(const Eigen::Vector3f& position);

  // If we want to avoid copying, we can return a pointer to
  // the member. Note that making public the attributes work
  // if we want to modify directly the members. However, this
  // is a matter of design.

  // Returns a mutable orientation vector.
  Eigen::Vector3f* mutable_orientation();

  // Returns a mutable position.
  Eigen::Vector3f* mutable_position();

  // Getters, return a const reference to the member.
  // Gets the orientation or pose of the object in the world.
  const Eigen::Vector3f& orientation();

  // Gets the position of the object in the world.
  const Eigen::Vector3f& position();

  // Returns a const reference of the vertices.
  const Eigen::MatrixXf& vertices() const;

  // Returns a const reference of the indices for an EBO.
  const std::vector<GLuint>& indices() const;

  // Returns the VBO id associated to this model.
  const GLuint vertex_buffer_object_id() const;

  // Returns the VAO id associated to this model.
  const GLuint vertx_array_object_id() const;

  // Returns the EBO id assotiated to this model.
  const GLuint element_buffer_object_id() const;

private:
  // Attributes.
  // The convention we will use is to define a '_' after the name
  // of the attribute.
  // Orientation or pose of the object in the world.
  Eigen::Vector3f orientation_;
  // Position of the object in the world.
  Eigen::Vector3f position_;
  // Vertex matrix.
  Eigen::MatrixXf vertices_;
  // Indices for EBO.
  std::vector<GLuint> indices_;
  // Vertex buffer object id.
  GLuint vertex_buffer_object_id_;
  // Vertex array object id.
  GLuint vertex_array_object_id_;
  // Element buffer object id.
  GLuint element_buffer_object_id_;
};

}  // namespace wvu

#endif  // MODEL_H_
