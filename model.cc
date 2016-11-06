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

#include "model.h"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <GL/glew.h>

namespace wvu {
Model::Model(const Eigen::Vector3f& orientation,
             const Eigen::Vector3f& position,
             const Eigen::MatrixXf& vertices) {
  orientation_ = orientation;
  position_ = position;
  vertices_ = vertices;
  vertex_buffer_object_id_ = 0;
  vertex_array_object_id_ = 0;
  element_buffer_object_id_ = 0;
}

Model::Model(const Eigen::Vector3f& orientation,
             const Eigen::Vector3f& position,
             const Eigen::MatrixXf& vertices,
             const std::vector<GLuint>& indices) {
  orientation_ = orientation;
  position_ = position;
  vertices_ = vertices;
  indices_ = indices;
  vertex_buffer_object_id_ = 0;
  vertex_array_object_id_ = 0;
  element_buffer_object_id_ = 0;
}

Model::~Model() {
  // TODO: Delete the buffers in GPU.
}

// Builds the model matrix from the orientation and position members.
Eigen::Matrix4f Model::ComputeModelMatrix() {
  // TODO(vfragoso): Implment me!
  return Eigen::Matrix4f::Random();
}

// Setters set members by *copying* input parameters.
void Model::set_orientation(const Eigen::Vector3f& orientation) {
  orientation_ = orientation;
}

// Setters set members by *copying* input parameters.
void Model::set_position(const Eigen::Vector3f& position) {
  position_ = position;
}

Eigen::Vector3f* Model::mutable_orientation() {
  return &orientation_;
}

Eigen::Vector3f* Model::mutable_position() {
  return &position_;
}

const Eigen::Vector3f& Model::orientation() {
  return orientation_;
}

const Eigen::Vector3f& Model::position() {
  return position_;
}

const Eigen::MatrixXf& Model::vertices() const {
  return vertices_;
}

const std::vector<GLuint>& Model::indices() const {
  return indices_;
}

const GLuint Model::vertex_buffer_object_id() const {
  return vertex_buffer_object_id_;
}

const GLuint Model::vertex_buffer_object_id() {
  return vertex_buffer_object_id_;
}

const GLuint Model::vertex_array_object_id() const {
  return vertex_array_object_id_;
}

const GLuint Model::vertex_array_object_id() {
  return vertex_array_object_id_;
}

const GLuint Model::element_buffer_object_id() const {
  return element_buffer_object_id_;
}

const GLuint Model::element_buffer_object_id() {
  return element_buffer_object_id_;
}

void Model::SetVerticesIntoGpu() {
  // TODO(vfragoso): Implement me!
}

void Model::Draw(const Eigen::Matrix4f& projection,
                 const Eigen::Matrix4f& view) {
  // TODO(vfragoso): Implement me!
}

}  // namespace wvu
