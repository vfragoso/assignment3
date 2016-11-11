// Copyright (C) 2016  Victor Fragoso <victor.fragoso@mail.wvu.edu>
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
//     * Neither the name of the West Virginia University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL VICTOR FRAGOSO BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

// C headers.
#define _USE_MATH_DEFINES  // For using M_PI.
#include <stdlib.h>  // For random.
#include <math.h>

// C++ headers.
#include <algorithm>  // For std::reverse.
#include <numeric>  // For std::accumulate.
#include <random>  // For random operations.
#include <unordered_set>
#include <vector>

// System specific headers.
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "glog/logging.h"
#include "gtest/gtest.h"

#include "transformations.h"
#include "model.h"

#define GLEW_STATIC
#include <GL/glew.h>
// The header of GLFW. This library is a C-based and light-weight library for
// creating windows for OpenGL rendering.
// See http://www.glfw.org/ for more information.
#include <GLFW/glfw3.h>

// These are unit tests using google gtest library. A binary will be created
// automatically to run the tests below.
namespace wvu {
namespace {
// GLSL shaders.
// Every shader should declare its version.
// Vertex shader follows standard 3.3.0.
// This shader declares/expexts an input variable named position. This input
// should have been loaded into GPU memory for its processing. The shader
// essentially sets the gl_Position -- an already defined variable -- that
// determines the final position for a vertex.
// Note that the position variable is of type vec3, which is a 3D dimensional
// vector. The layout keyword determines the way the VAO buffer is arranged in
// memory. This way the shader can read the vertices correctly.
const std::string vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "\n"
    "void main() {\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
    "}\n";

// Fragment shader follows standard 3.3.0. The goal of the fragment shader is to
// calculate the color of the pixel corresponding to a vertex. This is why we
// declare a variable named color of type vec4 (4D vector) as its output. This
// shader sets the output color to a (1.0, 0.5, 0.2, 1.0) using an RGBA format.
const std::string fragment_shader_src =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

struct ModelTest : public ::testing::Test {
  static void SetUpTestCase() {
    // Initialize the GLFW library.
    if (!glfwInit()) {
      LOG(FATAL) << "GLFW did not initialize correctly";
    }
    // Sets properties of windows and have to be set before creation.
    // GLFW_CONTEXT_VERSION_{MAJOR|MINOR} sets the minimum OpenGL API version
    // that this program will use.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // Sets the OpenGL profile.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Sets the property of resizability of a window.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    // Create a window and its OpenGL context.
    const std::string window_name = "Hello Triangle";
    window = glfwCreateWindow(480,
                              640,
                              window_name.c_str(),
                              nullptr,
                              nullptr);
    glfwMakeContextCurrent(window);
    // Initialize GLEW.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      glfwTerminate();
      LOG(FATAL) << "Glew did not initialize properly!";
    }
  }

  static void TearDownTestCase() {
    // Destroy window.
    glfwDestroyWindow(window);
    // Tear down GLFW library.
    glfwTerminate();
  }

  static GLFWwindow* window;
};

GLFWwindow* ModelTest::window = nullptr;

}  // namespace

TEST(TransformationsTest, TranslationMatrixCorrectness) {
  const Eigen::Vector3f offset = Eigen::Vector3f::Random();
  const Eigen::Matrix4f translation = ComputeTranslationMatrix(offset);
  const Eigen::Vector3f probe = Eigen::Vector3f::Random();
  const Eigen::Vector4f translated_probe = translation * probe.homogeneous();
  EXPECT_NEAR(((translated_probe.head(3) - offset) - probe).norm(), 0.0f, 1e-3);
}

TEST(TransformationsTest, TranslationMatrixInverse) {
  const Eigen::Vector3f offset1 = Eigen::Vector3f::Random();
  const Eigen::Matrix4f translation1 = ComputeTranslationMatrix(offset1);
  const Eigen::Vector3f offset2 = -1.0f * offset1;
  const Eigen::Matrix4f translation2 = ComputeTranslationMatrix(offset2);
  EXPECT_NEAR((translation1 * translation2).sum(), 4.0f, 1e-3);
}

TEST(TransformationsTest, RotationMatrixCorrectness) {
  const Eigen::Vector3f axis = Eigen::Vector3f::UnitZ();
  const float angle = M_PI / 4.0f;
  const Eigen::Matrix4f rotation = ComputeRotationMatrix(axis, angle);
  Eigen::Vector3f probe = Eigen::Vector3f::Random();
  probe.z() = 0.0f;
  const Eigen::Vector4f rotated_probe = rotation * probe.homogeneous();
  EXPECT_NEAR(probe.normalized().dot(rotated_probe.head(3).normalized()),
              cos(angle), 1e-3);
}

TEST(TransformationsTest, RotationMatrixInverse) {
  Eigen::Vector3f axis;
  axis.setRandom();
  axis.normalize();  // In place normalization.
  const float angle = M_PI / 3.0f;
  const Eigen::Matrix4f rotation1 = ComputeRotationMatrix(axis, angle);
  const Eigen::Matrix4f rotation2 = ComputeRotationMatrix(axis, -angle);
  EXPECT_NEAR((rotation1 * rotation2).sum(), 4.0f, 1e-3);
}

TEST(TransformationsTest, ScalingMatrixCorrectness) {
  // Seed with a real random value, if available.
  std::random_device rnd;
  // Choose a random mean between 1 and 6
  std::default_random_engine engine(rnd());
  std::uniform_real_distribution<float> uniform_dist(0.0f, 10.0f);
  const float scale = uniform_dist(engine);
  const Eigen::Matrix4f scaling = ComputeScalingMatrix(scale);
  const Eigen::Vector3f probe = Eigen::Vector3f::Random();
  const Eigen::Vector4f scaled_probe = scaling * probe.homogeneous();
  EXPECT_NEAR((scale * probe - scaled_probe.head(3)).norm(), 0.0f, 1e-3);
}

TEST(TransformationsTest, ScalingMatrixInverse) {
  // Seed with a real random value, if available.
  std::random_device rnd;
  // Choose a random mean between 1 and 6
  std::default_random_engine engine(rnd());
  std::uniform_real_distribution<float> uniform_dist(0.0f, 10.0f);
  const float scale = uniform_dist(engine);
  const Eigen::Matrix4f scaling1 = ComputeScalingMatrix(scale);
  const Eigen::Matrix4f scaling2 = ComputeScalingMatrix(1.0f / scale);
  EXPECT_NEAR((scaling1 * scaling2).sum(), 4.0f, 1e-3);
}

TEST_F(ModelTest, ComputeModelMatrix) {
  const float angle = M_PI / 8.0f;
  Eigen::Vector3f angle_axis = Eigen::Vector3f::Random();
  angle_axis.normalize();
  angle_axis *= angle;
  const Eigen::Vector3f position = Eigen::Vector3f::Random();
  const Eigen::MatrixXf vertices = Eigen::MatrixXf::Random(3, 3);
  Model model(angle_axis, position, vertices);
  const Eigen::MatrixXf model_matrix = model.ComputeModelMatrix();
  const Eigen::Vector3f probe = Eigen::Vector3f::Random();
  const Eigen::AngleAxisf rotation(angle, angle_axis.normalized());
  const Eigen::Vector3f expected_result = rotation.matrix() * probe + position;
  EXPECT_NEAR((expected_result.homogeneous() -
               model_matrix * probe.homogeneous()).norm(), 0.0f, 1e-3);
}

TEST_F(ModelTest, VerifyNonZeroVaoAndVboIds) {
  const float angle = M_PI / 8.0f;
  Eigen::Vector3f angle_axis = Eigen::Vector3f::Random();
  angle_axis.normalize();
  angle_axis *= angle;
  const Eigen::Vector3f position = Eigen::Vector3f::Random();
  const Eigen::MatrixXf vertices = Eigen::MatrixXf::Random(3, 3);
  Model model(angle_axis, position, vertices);
  model.SetVerticesIntoGpu();
  EXPECT_GT(model.vertex_array_object_id(), 0);
  EXPECT_GT(model.vertex_buffer_object_id(), 0);
}

TEST_F(ModelTest, VerifyNonZeroVaoVboAndEboIds) {
  const float angle = M_PI / 8.0f;
  Eigen::Vector3f angle_axis = Eigen::Vector3f::Random();
  angle_axis.normalize();
  angle_axis *= angle;
  const Eigen::Vector3f position = Eigen::Vector3f::Random();
  const Eigen::MatrixXf vertices = Eigen::MatrixXf::Random(3, 3);
  std::vector<GLuint> indices(3, 0);
  Model model(angle_axis, position, vertices, indices);
  model.SetVerticesIntoGpu();
  EXPECT_GT(model.vertex_array_object_id(), 0);
  EXPECT_GT(model.vertex_buffer_object_id(), 0);
  EXPECT_GT(model.element_buffer_object_id(), 0);
}

}  // namespace wvu
