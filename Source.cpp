#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <stdlib.h>
#include <iostream>
#include <C:\Users\Othman\source\repos\OpenGL\OpenGL\Shader.h>
#include <cstdint>
#include "SimplexNoise.h"
#include <vector>
#include <iterator>
#include<omp.h>
#include <chrono>
#include <mutex>

using namespace std;

int edgeTable[256] = {
0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0 };
int triTable[256][16] =
{ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1} };

// Window Variables
float width = 1920;
float height = 1080;

// Time Variables
float deltaTime = 0;

// Camera Variables
float theta = 90;
float phi = -90;
float mouseSensitivity = 0.2f;
float cameraSpeed = 1.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float _near = 0.1f;
float _far = 100.0f;


// Mouse Variables
double lastX;
double lastY;
bool firstMouse = true;

// Voxel Variables
float voxelDimension = 0.2f;
float threshold = 0.5f;
const int chunkSize = 4;
const int chunkNo = 10;
float worldWidth = chunkSize * chunkNo * voxelDimension * 0.1f;
bool updateBuffer = false;

// Perlin Noise
float sampleScale = 0.09f;

// Vertices
const int verticesSize = 50000000;
float vertices[verticesSize];
vector<float> vertexList;

float colors[6][3] = { {87.0f / 255.0f,  152.0f / 255.0f, 186.0f / 255.0f },
{ 59.0f / 255.0f,  160.0f / 255.0f, 177.0f / 255.0f },
{ 210.0f / 255.0f, 169.0f / 255.0f, 168.0f / 255.0f },
{ 130.0f / 255.0f, 177.0f / 255.0f, 159.0f / 255.0f },
{ 90.0f / 255.0f, 186.0f / 255.0f, 150.0f / 255.0f },
{ 77.0f / 255.0f, 194.0f / 255.0f, 141.0f / 255.0f } };


// Light Information
glm::vec3 lightPos = glm::vec3(0, 0, 0);
glm::vec3 lightCol = glm::vec3(1, 1, 1);

// Marching Cube
float isovalue = 0.0f;
bool enableMarching = true;

vector<float> face1(glm::vec3 translation)  {
    int randomInt = rand() % 6;

    vector<float> output = { -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 0, -1, 
                            voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 0, -1,
                            voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 0, -1,
                            voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 0, -1,
                           -voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 0, -1,
                           -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 0, -1 };
    return output;
};

vector<float> face2(glm::vec3 translation) {
    int randomInt = rand() % 6;

    vector<float> output = { -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 0, 1,
                            voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],0, 0, 1,
                            voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1], voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],0, 0, 1,
                            voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1], voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],0, 0, 1,
                           -voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1], voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],0, 0, 1,
                           -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],0, 0, 1 };
    return output;
};

vector<float> face3(glm::vec3 translation) {
    int randomInt = rand() % 6;

    vector<float> output = { -voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], -1, 0, 0,
                           -voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], -1, 0, 0,
                           -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], -1, 0, 0,
                           -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], -1, 0, 0,
                           -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], -1, 0, 0,
                           -voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], -1, 0, 0 };
    return output;
};

vector<float> face4(glm::vec3 translation) {
    int randomInt = rand() % 6;

    vector<float> output = {  voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],1, 0, 0,
                            voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],1, 0, 0,
                            voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],1, 0, 0,
                            voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],1, 0, 0,
                            voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],1, 0, 0,
                            voxelDimension * 0.5f + translation[0],  voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2],  colors[randomInt][0], colors[randomInt][1], colors[randomInt][2],1, 0, 0 };
    return output;
};

vector<float> face5(glm::vec3 translation) {
    int randomInt = rand() % 6;

    vector<float> output = { -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, -1, 0,
                            voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, -1, 0,
                            voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, -1, 0,
                            voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, -1, 0,
                           -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, -1, 0,
                           -voxelDimension * 0.5f + translation[0], -voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2],  colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, -1, 0 };
    return output;
};

vector<float> face6(glm::vec3 translation) {
    int randomInt = rand() % 6;

    vector<float> output = { -voxelDimension * 0.5f + translation[0], voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 1, 0,
                            voxelDimension * 0.5f + translation[0], voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 1, 0,
                            voxelDimension * 0.5f + translation[0], voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 1, 0,
                            voxelDimension * 0.5f + translation[0], voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 1, 0,
                           -voxelDimension * 0.5f + translation[0], voxelDimension * 0.5f + translation[1],  voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 1, 0,
                           -voxelDimension * 0.5f + translation[0], voxelDimension * 0.5f + translation[1], -voxelDimension * 0.5f + translation[2], colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], 0, 1, 0, };
    return output;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
float * mergeArrays(float array1[], float array2[], float result[]) {
    const int length1 = (sizeof(array1) / sizeof(array1[0])) + 1;
    const int length2 = (sizeof(array2) / sizeof(array2[0])) + 1;

    const int length = length1 + length2;

    for (int i = 0; i < length1; i++) {
        result[i] = array1[i];
    }

    for (int i = length1; i < length; i++) {
        result[i] = array2[i - length1];
    }

    return result;
}

void printArray(float * array, int size) {
    std::cout << "[";
  
    for (int i = 0; i < size; i++) {
        if (i == size) {
            std::cout << array[i];
        }
        else {
            std::cout << array[i] << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (firstMouse) {
        firstMouse = false;
        lastX = xposIn;
        lastY = yposIn;
    }

    double deltaX = (xposIn - lastX) * mouseSensitivity;
    double deltaY = (yposIn - lastY) * mouseSensitivity;

    theta += deltaX;
    phi -= deltaY;

    if (phi > -0.5f) {
        phi = -0.5f;
    }
    else if (phi < -179.5f) {
        phi = -179.5f;
    }

    cameraFront = glm::normalize(glm::vec3(sin(glm::radians(phi)) * cos(glm::radians(theta)), cos(glm::radians(phi)), sin(glm::radians(phi)) * sin(glm::radians(theta))));

    lastX = xposIn;
    lastY = yposIn;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;

    glfwSetCursorPosCallback(window, mouse_callback);
}
float noiseFunction(glm::vec3 position) {
    return SimplexNoise::noise(position[0] * sampleScale, position[1] * sampleScale, position[2] * sampleScale);
}


vector<float> generateChunk(glm::vec3 position) {
    vector<float> output; 
    bool chunkVoxels[chunkSize][chunkSize][chunkSize];

#pragma omp parallel for collapse(3)
    for (int i = 0; i < chunkSize; i++) {
        for (int j = 0; j < chunkSize; j++) {
            for (int k = 0; k < chunkSize; k++) {
                glm::vec3 voxelPos = position / voxelDimension + glm::vec3(i, j, k);
                float noiseValue = noiseFunction(voxelPos);

                if (noiseValue > threshold) {
                    chunkVoxels[i][j][k] = true;
                }
                else {
                    chunkVoxels[i][j][k] = false;
                }
            }
        }
    }

    //#pragma omp parallel for collapse(3)
    for (int i = 0; i < chunkSize; i++) {
        for (int j = 0; j < chunkSize; j++) {
            for (int k = 0; k < chunkSize; k++) {
                glm::vec3 translation = position + glm::vec3(i * voxelDimension, j * voxelDimension, k * voxelDimension);

                if (chunkVoxels[i][j][k]) {
                    if (i == 0) {
                        glm::vec3 voxelPos = position / voxelDimension + glm::vec3(-1, j, k);
                        float noiseValue = noiseFunction(voxelPos);

                        if (!(noiseValue > threshold)) {
                            vector<float> face = face3(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }

                        if (!chunkVoxels[i + 1][j][k]) {
                            vector<float> face = face4(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }
                    else if (i == chunkSize - 1) {
                        glm::vec3 voxelPos = position / voxelDimension + glm::vec3(chunkSize, j, k);
                        float noiseValue = noiseFunction(voxelPos);

                        if (!(noiseValue > threshold)) {
                            vector<float> face = face4(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }

                        if (!chunkVoxels[i - 1][j][k]) {
                            vector<float> face = face3(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }
                    else {
                        if (!chunkVoxels[i - 1][j][k]) {
                            vector<float> face = face3(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                        if (!chunkVoxels[i + 1][j][k]) {
                            vector<float> face = face4(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }

                    if (j == 0) {

                        glm::vec3 voxelPos = position / voxelDimension + glm::vec3(i, -1, k);
                        float noiseValue = noiseFunction(voxelPos);

                        if (!(noiseValue > threshold)) {
                            vector<float> face = face5(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }


                        if (!chunkVoxels[i][j + 1][k]) {
                            vector<float> face = face6(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }
                    else if (j == chunkSize - 1) {
                        glm::vec3 voxelPos = position / voxelDimension + glm::vec3(i, chunkSize, k);
                        float noiseValue = noiseFunction(voxelPos);

                        if (!(noiseValue > threshold)) {
                            vector<float> face = face6(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }

                        if (!chunkVoxels[i][j - 1][k]) {
                            vector<float> face = face5(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }
                    else {
                        if (!chunkVoxels[i][j - 1][k]) {
                            vector<float> face = face5(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                        if (!chunkVoxels[i][j + 1][k]) {
                            vector<float> face = face6(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }

                    if (k == 0) {
                        glm::vec3 voxelPos = position / voxelDimension + glm::vec3(i, j, -1);
                        float noiseValue = noiseFunction(voxelPos);

                        if (!(noiseValue > threshold)) {
                            vector<float> face = face1(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }


                        if (!chunkVoxels[i][j][k + 1]) {
                            vector<float> face = face2(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }
                    else if (k == chunkSize - 1) {
                        glm::vec3 voxelPos = position / voxelDimension + glm::vec3(i, j, chunkSize);
                        float noiseValue = noiseFunction(voxelPos);

                        if (!(noiseValue > threshold)) {
                            vector<float> face = face2(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }

                        if (!chunkVoxels[i][j][k - 1]) {
                            vector<float> face = face1(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }
                    else {
                        if (!chunkVoxels[i][j][k - 1]) {
                            vector<float> face = face1(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                        if (!chunkVoxels[i][j][k + 1]) {
                            vector<float> face = face2(translation);
                            output.insert(output.end(), face.begin(), face.end());
                        }
                    }

                }

            }
        }
    }

    return output;
}

glm::vec3 VertexInterp(float isolevel, glm::vec3 p1, glm::vec3 p2, float p1V, float p2V) {
    if (abs(isolevel - p1V) < 0.00001f)
        return(p1);
    if (abs(isolevel - p2V) < 0.00001f)
        return(p2);
    if (abs(p1V - p2V) < 0.00001f)
        return(p1);

    float t = (isolevel - p1V) / (p2V - p1V);
    return p1 + t * (p2 - p1);
}

vector<float> generateMarchingChunk(glm::vec3 position) {
    vector<float> output;
    float chunkVoxels[chunkSize + 1][chunkSize + 1][chunkSize + 1];
    glm::vec3 voxelPoses[chunkSize + 1][chunkSize + 1][chunkSize + 1];

#pragma omp parallel for collapse(3)
    for (int i = 0; i < chunkSize + 1; i++) {
        for (int j = 0; j < chunkSize + 1; j++) {
            for (int k = 0; k < chunkSize + 1; k++) {
                float noiseValue = noiseFunction(position / voxelDimension + glm::vec3(i, j, k));
                chunkVoxels[i][j][k] = noiseValue;
                voxelPoses[i][j][k] = position + glm::vec3(i * voxelDimension, j * voxelDimension, k * voxelDimension);
            }
        }
    }

    mutex mtx;

#pragma omp parallel for collapse(3)
    for (int i = 0; i < chunkSize; i++) {
        for (int j = 0; j < chunkSize; j++) {
            for (int k = 0; k < chunkSize; k++) {

                int cubeindex = 0;


                if (chunkVoxels[i][j][k] < isovalue) cubeindex |= 1; // 0
                if (chunkVoxels[i + 1][j][k] < isovalue) cubeindex |= 2; // 1
                if (chunkVoxels[i + 1][j][k + 1] < isovalue) cubeindex |= 4; // 2
                if (chunkVoxels[i][j][k + 1] < isovalue) cubeindex |= 8; // 3
                if (chunkVoxels[i][j + 1][k] < isovalue) cubeindex |= 16; // 4
                if (chunkVoxels[i + 1][j + 1][k] < isovalue) cubeindex |= 32; // 5 
                if (chunkVoxels[i + 1][j + 1][k + 1] < isovalue) cubeindex |= 64; // 6
                if (chunkVoxels[i][j + 1][k + 1] < isovalue) cubeindex |= 128; // 7



                glm::vec3 vertlist[12];
                /* Find the vertices where the surface intersects the cube */

                if (edgeTable[cubeindex] & 1)
                    vertlist[0] = VertexInterp(isovalue, voxelPoses[i][j][k], voxelPoses[i + 1][j][k], chunkVoxels[i][j][k], chunkVoxels[i + 1][j][k]);
                if (edgeTable[cubeindex] & 2)
                    vertlist[1] = VertexInterp(isovalue, voxelPoses[i + 1][j][k], voxelPoses[i + 1][j][k + 1], chunkVoxels[i + 1][j][k], chunkVoxels[i + 1][j][k + 1]);
                if (edgeTable[cubeindex] & 4)
                    vertlist[2] = VertexInterp(isovalue, voxelPoses[i + 1][j][k + 1], voxelPoses[i][j][k + 1], chunkVoxels[i + 1][j][k + 1], chunkVoxels[i][j][k + 1]);
                if (edgeTable[cubeindex] & 8)
                    vertlist[3] = VertexInterp(isovalue, voxelPoses[i][j][k + 1], voxelPoses[i][j][k], chunkVoxels[i][j][k + 1], chunkVoxels[i][j][k]);
                if (edgeTable[cubeindex] & 16)
                    vertlist[4] = VertexInterp(isovalue, voxelPoses[i][j + 1][k], voxelPoses[i + 1][j + 1][k], chunkVoxels[i][j + 1][k], chunkVoxels[i + 1][j + 1][k]);
                if (edgeTable[cubeindex] & 32)
                    vertlist[5] = VertexInterp(isovalue, voxelPoses[i + 1][j + 1][k], voxelPoses[i + 1][j + 1][k + 1], chunkVoxels[i + 1][j + 1][k], chunkVoxels[i + 1][j + 1][k + 1]);
                if (edgeTable[cubeindex] & 64)
                    vertlist[6] = VertexInterp(isovalue, voxelPoses[i + 1][j + 1][k + 1], voxelPoses[i][j + 1][k + 1], chunkVoxels[i + 1][j + 1][k + 1], chunkVoxels[i][j + 1][k + 1]);
                if (edgeTable[cubeindex] & 128)
                    vertlist[7] = VertexInterp(isovalue, voxelPoses[i][j + 1][k + 1], voxelPoses[i][j + 1][k], chunkVoxels[i][j + 1][k + 1], chunkVoxels[i][j + 1][k]);
                if (edgeTable[cubeindex] & 256)
                    vertlist[8] = VertexInterp(isovalue, voxelPoses[i][j][k], voxelPoses[i][j + 1][k], chunkVoxels[i][j][k], chunkVoxels[i][j + 1][k]);
                if (edgeTable[cubeindex] & 512)
                    vertlist[9] = VertexInterp(isovalue, voxelPoses[i + 1][j][k], voxelPoses[i + 1][j + 1][k], chunkVoxels[i + 1][j][k], chunkVoxels[i + 1][j + 1][k]);
                if (edgeTable[cubeindex] & 1024)
                    vertlist[10] = VertexInterp(isovalue, voxelPoses[i + 1][j][k + 1], voxelPoses[i + 1][j + 1][k + 1], chunkVoxels[i + 1][j][k + 1], chunkVoxels[i + 1][j + 1][k + 1]);
                if (edgeTable[cubeindex] & 2048)
                    vertlist[11] = VertexInterp(isovalue, voxelPoses[i][j][k + 1], voxelPoses[i][j + 1][k + 1], chunkVoxels[i][j][k + 1], chunkVoxels[i][j + 1][k + 1]);

                /* Create the triangle */

                for (int p = 0; triTable[cubeindex][p] != -1; p += 3) {

                    int randomInt = rand() % 6;

                    glm::vec3 trianglePt1 = glm::vec3(vertlist[triTable[cubeindex][p]].x, vertlist[triTable[cubeindex][p]].y, vertlist[triTable[cubeindex][p]].z);
                    glm::vec3 trianglePt2 = glm::vec3(vertlist[triTable[cubeindex][p + 1]].x, vertlist[triTable[cubeindex][p + 1]].y, vertlist[triTable[cubeindex][p + 1]].z);
                    glm::vec3 trianglePt3 = glm::vec3(vertlist[triTable[cubeindex][p + 2]].x, vertlist[triTable[cubeindex][p + 2]].y, vertlist[triTable[cubeindex][p + 2]].z);

                    glm::vec3 normal =glm::cross(trianglePt3 - trianglePt1, trianglePt2 - trianglePt1);

                    vector<float> tempList = {
                    trianglePt1.x, trianglePt1.y, trianglePt1.z, colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], normal.x, normal.y, normal.z,
                    trianglePt2.x, trianglePt2.y, trianglePt2.z, colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], normal.x, normal.y, normal.z,
                    trianglePt3.x, trianglePt3.y, trianglePt3.z, colors[randomInt][0], colors[randomInt][1], colors[randomInt][2], normal.x, normal.y, normal.z
                    };

                    mtx.lock();

                    output.insert(output.end(), tempList.begin(), tempList.end());

                    mtx.unlock();
                }
            }
        }
    }

    return output;
}

int mod(float x, float mod) {
    int a = floor(x / mod);
    x = x - floor(x / mod) * mod;
    return static_cast<int>(x);
}

class Chunk {

public:

    glm::vec3 position;
    int index[3];
    vector<float> vertexInfo;
    bool mustUpdate = false;

    Chunk(glm::vec3 pos) {
        this->position = pos;

        if (enableMarching) {
            vertexInfo = generateMarchingChunk(pos);
        }
        else {
            vertexInfo = generateChunk(pos);
        }


    }

    Chunk() {

    }


    void updateIndex() {

        glm::vec3 relativePos = position - cameraPos;

        //ssacout << index[0] << " " << index[1] << " " << index[2] << endl;
        float unit = chunkSize * voxelDimension * chunkNo / 2.0f;

        if (relativePos[0] < -unit || relativePos[0] > unit || relativePos[1] < -unit || relativePos[1] > unit || relativePos[2] < -unit || relativePos[2] > unit) {

            if (relativePos[0] < -unit) {
                position[0] = position[0] + 2 * unit;
            }
            if (relativePos[0] > unit) {
                position[0] = position[0] - 2 * unit;
            }
            if (relativePos[1] < -unit) {
                position[1] = position[1] + 2 * unit;
            }
            if (relativePos[1] > unit) {
                position[1] = position[1] - 2 * unit;
            }
            if (relativePos[2] < -unit) {
                position[2] = position[2] + 2 * unit;
            }
            if (relativePos[2] > unit) {
                position[2] = position[2] - 2 * unit;

            }

            updateBuffer = true;

            if (enableMarching) {
                vertexInfo = generateMarchingChunk(position);
            }
            else {
                vertexInfo = generateChunk(position);
            }
        }
    }
};

Chunk chunks[chunkNo][chunkNo][chunkNo];


void initializeChunks() {
    for (int i = 0; i < chunkNo; i++) {
        for (int j = 0; j < chunkNo; j++) {
            for (int k = 0; k < chunkNo; k++) {
                glm::vec3 position = cameraPos + glm::vec3((i - chunkNo / 2.0f) * chunkSize * voxelDimension, (j - chunkNo / 2.0f) * chunkSize * voxelDimension, (k - chunkNo / 2.0f) * chunkSize * voxelDimension);
                chunks[i][j][k] = Chunk(position);
            }
        }
    }
}
void combineChunks() {

    vertexList.clear();
    std::fill_n(vertices, verticesSize, 0);

    for (int i = 0; i < chunkNo; i++) {
        for (int j = 0; j < chunkNo; j++) {
            for (int k = 0; k < chunkNo; k++) {
                vertexList.insert(vertexList.end(), chunks[i][j][k].vertexInfo.begin(), chunks[i][j][k].vertexInfo.end());
            }
        }
    }

    std::copy(vertexList.begin(), vertexList.end(), vertices);
}

int main()
{

    initializeChunks();
    combineChunks();

    std::cout << vertexList.size() << endl;
    

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Hello", glfwGetPrimaryMonitor(), nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    Shader ourShader("vs.glsl", "fs.glsl");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), width / height, _near, _far);
    glm::mat4 trans = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   
    while (!glfwWindowShouldClose(window))
    {
        lightPos = cameraPos;

        float time = glfwGetTime();

        //for (;;) {
        //    if (glfwGetTime() - time > 1 / 60.0f - deltaTime) {
        //        break;
        //    }
        //}

        glfwPollEvents();

        processInput(window);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        int modelLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));

        modelLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));

        modelLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));

        modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        #pragma omp parallel for collapse(3)
        for (int i = 0; i < chunkNo; i++) {
            for (int j = 0; j < chunkNo; j++) {
                for (int k = 0; k < chunkNo; k++) {
                    chunks[i][j][k].updateIndex();
                }
            }
        }

        if (updateBuffer) {

            combineChunks();

            cout << sizeof(vertices) << endl;
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(float), vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            updateBuffer = false; 
        }

        ourShader.use();
        ourShader.setFloat("time", 0.05f * sin(time * 10.0f) + 0.95f);
        ourShader.setFloat("worldWidth", worldWidth);
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setVec3("lightCol", lightCol);
        ourShader.setVec3("cameraDirection", cameraFront);

        glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (6 * sizeof(float)));

        glfwSwapBuffers(window);

        deltaTime = glfwGetTime() - time;
        std::cout << "fps: " << 1 / (deltaTime) << std::endl;
    }


    glfwTerminate();
    return 0;
}

