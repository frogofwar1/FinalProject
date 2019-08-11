#ifndef cw_LOAD_OBJECT
#define cw_LOAD_OBJECT

#include "gl_mesh.hpp"

#include <vector>

namespace cw
{
	struct LoadOBJ {
		bool isTriangle = true;

		std::vector<Vertex> vertices;
		std::vector<GLushort> indices;

		LoadOBJ(const char* filePath);
		void LoadFileOBJ(const char* filePath);
		virtual ~LoadOBJ();
	};
}
#endif