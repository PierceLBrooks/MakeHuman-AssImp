//
//  VoxelGrid.hpp
//  Surface Heat Diffuse
//
//  Created by MINGFENWANG on 2018/3/25.
//  Copyright © 2017年 MINGFENWANG. All rights reserved.
//

#ifndef VOXEL_GRID_HPP
#define VOXEL_GRID_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <iomanip>
#include <cfloat>
#include <ctime>
#include <cstring>

#include "Vector.hpp"
#include "raytri.hpp"
#include "tribox.hpp"
#include <random>

#ifndef MH_PI
#define MH_PI mh::vec::Pi
#endif

namespace mh {

struct Weight {
    int bone;
    float weight;
};

struct Bone_Heat {
    Bone_Heat();

    std::vector<float> static_heats;
    std::vector<float> ping_heats;
    std::vector<float> pong_heats;
};

struct Vertex {
    Vertex(const float x, const float y, const float z);

    void sort_weight();

    static bool greater_function(Weight a, Weight b);

    structvec3 pos;
    std::vector<Weight> weights;
    std::vector<int> neibours;
    Bone_Heat bone_heat;
};

struct Triangle {
    Triangle(const int v0, const int v1, const int v2);

    int v[3];
};

struct Bone {
    Bone(const std::string _name,
         const structvec3& _head,
         const structvec3& _tail);

    std::string name;
    structvec3 head;
    structvec3 tail;
};

struct Bone_Point {
    Bone_Point(const int _index, const structvec3& _pos);

    int index;
    structvec3 pos;
    float darkness;
    float radius;
};

// uniform voxel
struct Voxel {
    Voxel();

    std::vector<int> vertices;
    std::vector<int> triangles;
};

class PositionCompare {
private:
///Condition: exponents are same and high (24-FLOAT_CMP_IGNORE_BITS) bits in mantissa are same.
bool FloatEqual(float f1, float f2) const;

public:
bool operator()(const structvec3& a, const structvec3& b) const;
};

// 3d voxel grid class
class VoxelGrid {
public:
VoxelGrid(const std::string& _mesh_file,
          const std::string& _bone_file,
          const std::string& _weight_file,
          const int _max_grid_num,
          const int _max_diffuse_loop,
          const int _max_sample_num,
          const int _max_influence,
          const float _max_fall_off,
          const int _sharpness,
          const bool _detect_solidify);

void calculate_all_voxel_darkness();

void diffuse_all_heats();

int get_nearest_bone(const int index);

void generate_weight_for_vertices();

void export_bone_weights();

private:
void read_mesh_from_file(const std::string& filename);

void read_bone_from_file(const std::string& filename);

bool has_triangle(Voxel& voxel);

bool has_vertex(Voxel& voxel);

void get_vertex_bound_box(const Vertex& vertex,
                          structvec3&     bb_min,
                          structvec3&     bb_max);

void get_triangle_bound_box(const Triangle& triangle,
                            structvec3&     bb_min,
                            structvec3&     bb_max);

void add_all_triangles();

void add_triangle(int n);

void add_all_bones();

void add_bone(const std::vector<Bone>& bones, int bone_index);

void shrink_all_vertex_neibours();

void add_all_vertices();

void shrink_all_vertices();

void shrink_all_triangles();

void init_all_vertex_heats();

void bone_point_darkness(int index);

void bone_point_glow(int index);

void calculate_darkness_in_range(const int start, const int block_size);

void diffuse_vertex_in_range(const int start, const int block_size);

// get the nearest hit point and the darkness
void ray_cast_darkness(const structvec3& ray_origin,
                              const structvec3& ray_dir,
                              const Voxel&      voxel,
                              float&            darkness,
                              float&            distance);

// get the nearest hit point and the darkness
int ray_cast_triangle(const structvec3& ray_origin,
                             const structvec3& ray_dir,
                             const Voxel&      voxel,
                             const int vertex_index);

void search_vertices_in_range(const structvec3& pos,
                              const float radius,
                              std::vector<int>& result);

float vertex_heat_standard_error();

void diffuse_vertex(const int index);

// helper function
void split_string(const std::string&        s,
                  std::vector<std::string>& v,
                  const std::string&        c);

std::string mesh_file;
std::string bone_file;
std::string weight_file;
int max_grid_num;
int max_diffuse_loop;
int max_sample_num;
int max_influence;
float max_fall_off;
int sharpness;
int grid_num_x;
int grid_num_y;
int grid_num_z;
float grid_size;
structvec3 grid_offset;
std::vector<Voxel> voxels;
bool ping_pong;
std::vector<Vertex> vertices;
std::vector<Triangle> triangles;
std::vector<Bone> bones;
std::vector<Bone_Point> bone_points;
bool detect_solidify;
std::mt19937 mt_rand;
std::uniform_real_distribution<float> mt_dist_00_10;
std::uniform_real_distribution<float> mt_dist_05_05;
std::uniform_real_distribution<float> mt_dist_00_2PI;
std::uniform_real_distribution<float> mt_dist_N10_10;
};

}

#endif // VOXEL_GRID_HPP

