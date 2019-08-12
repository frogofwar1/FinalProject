#include "LoadOBJ.hpp"

#include "cw_header.hpp"

#include <iostream>
#include <fstream>

using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace cw
{

	struct coordinate {
		float x, y, z;
		coordinate(float a, float b, float c) : x(a), y(b), z(c) {};
	};

	//for faces, it can contain triangles and quads as well, the four variable contain which is that
	struct face {
		int faces[4];
		int tfaces[4];
		int nfaces[4];
		face(int f1, int f2, int f3,
			int tf1, int tf2, int tf3,
			int nf1, int nf2, int nf3) {
			faces[0] = f1;
			faces[1] = f2;
			faces[2] = f3;
			tfaces[0] = tf1;
			tfaces[1] = tf2;
			tfaces[2] = tf3;
			nfaces[0] = nf1;
			nfaces[1] = nf2;
			nfaces[2] = nf3;
		}
		face(int f1, int f2, int f3, int f4,
			int tf1, int tf2, int tf3, int tf4,
			int nf1, int nf2, int nf3, int nf4) {
			faces[0] = f1;
			faces[1] = f2;
			faces[2] = f3;
			faces[3] = f4;
			tfaces[0] = tf1;
			tfaces[1] = tf2;
			tfaces[2] = tf3;
			tfaces[3] = tf4;
			nfaces[0] = nf1;
			nfaces[1] = nf2;
			nfaces[2] = nf3;
			nfaces[3] = nf4;
		}
	};

	LoadOBJ::LoadOBJ(const char* filePath)
	{
		LoadFileOBJ(filePath);
	}

	LoadOBJ::~LoadOBJ()
	{
		//vertices.clear();
		//indices.clear();
	}

	void LoadOBJ::LoadFileOBJ(const char* filePath)
	{
		REQUIRE(filePath != nullptr);

		std::vector<std::string*> coord;        //read every single line of the obj file as a string
		std::vector<coordinate*> vertex;
		std::vector<face*> faces;
		std::vector<coordinate*> normals;       //normal vectors for every face
		std::vector<coordinate*> textures;		//texture vectors in 3-dimensions but use 2-dimensions
		std::ifstream in(filePath);     //open the .obj file
		bool hasTexture = false;
		bool hasNormal = false;

		if (!in.is_open())
		{
			std::cout << "Can't open file." << std::endl;
		}
		char buf[256];
		//read in every line to coord
		while (!in.eof())
		{
			in.getline(buf, 256);

			coord.push_back(new std::string(buf));
		}
		//go through all of the elements of coord, and decide what kind of element is that
		for (unsigned int i = 0; i < coord.size(); i++)
		{
			if (coord[i]->c_str()[0] == '#')   //if it is a comment (the first character is #)
				continue;       //we don't care about that
			else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == ' ') //if vector
			{
				float tmpx, tmpy, tmpz;
				sscanf_s(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);       //read in the 3 float coordinate to tmpx,tmpy,tmpz
				vertex.push_back(new coordinate(tmpx, tmpy, tmpz));       //and then add it to the end of our vertex list
			}
			else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == 'n')        //if normal vector
			{
				if (!hasNormal)
					hasNormal = true;
				float tmpx, tmpy, tmpz;
				sscanf_s(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
				normals.push_back(new coordinate(tmpx, tmpy, tmpz));
			}
			else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == 't')        //if texture vector
			{
				if (!hasTexture)
					hasTexture = true;
				float tmpx, tmpy;
				sscanf_s(coord[i]->c_str(), "vt %f %f", &tmpx, &tmpy);
				textures.push_back(new coordinate(tmpx, tmpy, 0.0f));
			}
			else if (coord[i]->c_str()[0] == 'f')     //if face
			{
				int a, b, c, d, e, f, g, h, k, l, m, n;
				if (hasTexture && hasNormal) {
					if (count(coord[i]->begin(), coord[i]->end(), '/') == 6)
					{
						sscanf_s(coord[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &d, &g, &b, &e, &h, &c, &f, &k);
						faces.push_back(new face(a, b, c, d, e, f, g, h, k));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						if (!isTriangle)
							isTriangle = true;
					}
					else {
						sscanf_s(coord[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &a, &e, &k, &b, &f, &l, &c, &g, &m, &d, &h, &n);
						faces.push_back(new face(a, b, c, d, e, f, g, h, k, l, m, n));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(d);
						if (isTriangle)
							isTriangle = false;
					}
				}
				else if (hasTexture)
				{
					if (count(coord[i]->begin(), coord[i]->end(), '/') == 3)
					{
						sscanf_s(coord[i]->c_str(), "f %d/%d %d/%d %d/%d", &a, &d, &b, &e, &c, &f);
						faces.push_back(new face(a, b, c, d, e, f, 0, 0, 0));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						if (!isTriangle)
							isTriangle = true;
					}
					else {
						sscanf_s(coord[i]->c_str(), "f %d/%d %d/%d %d/%d %d/%d", &a, &e, &b, &f, &c, &g, &d, &h);
						faces.push_back(new face(a, b, c, d, e, f, g, h, 0, 0, 0, 0));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(d);
						if (isTriangle)
							isTriangle = false;
					}
				}
				else if (hasNormal)
				{
					if (count(coord[i]->begin(), coord[i]->end(), '/') == 6)
					{
						sscanf_s(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &d, &b, &e, &c, &f);
						faces.push_back(new face(a, b, c, 0, 0, 0, d, e, f));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						if (!isTriangle)
							isTriangle = true;
					}
					else {
						sscanf_s(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &e, &b, &f, &c, &g, &d, &h);
						faces.push_back(new face(a, b, c, d, 0, 0, 0, 0, e, f, g, h));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(d);
						if (isTriangle)
							isTriangle = false;
					}
				}
				else
				{
					if (count(coord[i]->begin(), coord[i]->end(), ' ') == 3)
					{
						sscanf_s(coord[i]->c_str(), "f %d %d %d", &a, &b, &c);
						faces.push_back(new face(a, b, c, 0, 0, 0, 0, 0, 0));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						if (!isTriangle)
							isTriangle = true;
					}
					else {
						sscanf_s(coord[i]->c_str(), "f %d %d %d %d", &a, &b, &c, &d);
						faces.push_back(new face(a, b, c, d, 0, 0, 0, 0, 0, 0, 0, 0));
						indices.emplace_back(a);
						indices.emplace_back(b);
						indices.emplace_back(c);
						indices.emplace_back(d);
						if (isTriangle)
							isTriangle = false;
					}
				}
			}
		}

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			vec4 tmpc = vec4(0.5f, 0.5f, 0.5f, 1.0f);
			if (!isTriangle)
			{
				if (hasTexture && hasNormal) {
					for (int j = 0; j < 4; j++) {
						vec3 tmpv = vec3(vertex[faces[i]->faces[j] - 1]->x, vertex[faces[i]->faces[j] - 1]->y, vertex[faces[i]->faces[j] - 1]->z);
						vec2 tmpvt = vec2(textures[faces[i]->tfaces[j] - 1]->x, textures[faces[i]->tfaces[j] - 1]->y);
						vec3 tmpvn = vec3(normals[faces[i]->nfaces[j] - 1]->x, normals[faces[i]->nfaces[j] - 1]->y, normals[faces[i]->nfaces[j] - 1]->z);
						vertices.emplace_back(Vertex{ tmpv, tmpvn, tmpc, tmpvt });
					}
				}
				else if (hasTexture) {
					for (int j = 0; j < 4; j++) {
						vec3 tmpv = vec3(vertex[faces[i]->faces[j] - 1]->x, vertex[faces[i]->faces[j] - 1]->y, vertex[faces[i]->faces[j] - 1]->z);
						vec2 tmpvt = vec2(textures[faces[i]->tfaces[j] - 1]->x, textures[faces[i]->tfaces[j] - 1]->y);
						vec3 tmpvn = tmpv;
						vertices.emplace_back(Vertex{ tmpv, tmpvn, tmpc, tmpvt });
					}
				}
				else if (hasNormal) {
					for (int j = 0; j < 4; j++) {
						vec3 tmpv = vec3(vertex[faces[i]->faces[j] - 1]->x, vertex[faces[i]->faces[j] - 1]->y, vertex[faces[i]->faces[j] - 1]->z);
						vec2 tmpvt = vec2(0.0f, 0.0f);
						vec3 tmpvn = vec3(normals[faces[i]->nfaces[j] - 1]->x, normals[faces[i]->nfaces[j] - 1]->y, normals[faces[i]->nfaces[j] - 1]->z);
						vertices.emplace_back(Vertex{ tmpv, tmpvn, tmpc, tmpvt });
					}
				}
				else
				{
					for (int j = 0; j < 4; j++) {
						vec3 tmpv = vec3(vertex[faces[i]->faces[j] - 1]->x, vertex[faces[i]->faces[j] - 1]->y, vertex[faces[i]->faces[j] - 1]->z);
						vec2 tmpvt = vec2(0.0f, 0.0f);
						vec3 tmpvn = tmpv;
						vertices.emplace_back(Vertex{ tmpv, tmpvn, tmpc, tmpvt });
					}
				}
			}
			else {
				if (hasTexture && hasNormal) {
					for (int j = 0; j < 3; j++) {
						vec3 tmpv = vec3(vertex[faces[i]->faces[j] - 1]->x, vertex[faces[i]->faces[j] - 1]->y, vertex[faces[i]->faces[j] - 1]->z);
						vec2 tmpvt = vec2(textures[faces[i]->tfaces[j] - 1]->x, textures[faces[i]->tfaces[j] - 1]->y);
						vec3 tmpvn = vec3(normals[faces[i]->nfaces[j] - 1]->x, normals[faces[i]->nfaces[j] - 1]->y, normals[faces[i]->nfaces[j] - 1]->z);
						vertices.emplace_back(Vertex{ tmpv, tmpvn, tmpc, tmpvt });
					}
				}
				else if (hasTexture) {
					for (int j = 0; j < 3; j++) {
						vec3 tmpv = vec3(vertex[faces[i]->faces[j] - 1]->x, vertex[faces[i]->faces[j] - 1]->y, vertex[faces[i]->faces[j] - 1]->z);
						vec2 tmpvt = vec2(textures[faces[i]->tfaces[j] - 1]->x, textures[faces[i]->tfaces[j] - 1]->y);
						vec3 tmpvn = tmpv;
						vertices.emplace_back(Vertex{ tmpv, tmpvn, tmpc, tmpvt });
					}
				}
				else if (hasNormal) {
					for (int j = 0; j < 3; j++) {
						vec3 tmpv = vec3(vertex[faces[i]->faces[j] - 1]->x, vertex[faces[i]->faces[j] - 1]->y, vertex[faces[i]->faces[j] - 1]->z);
						vec2 tmpvt = vec2(0.0f, 0.0f);
						vec3 tmpvn = vec3(normals[faces[i]->nfaces[j] - 1]->x, normals[faces[i]->nfaces[j] - 1]->y, normals[faces[i]->nfaces[j] - 1]->z);
						vertices.emplace_back(Vertex{ tmpv, tmpvn, tmpc, tmpvt });
					}
				}
				else
				{
					for (int j = 0; j < 3; j++) {
						vec3 tmpv = vec3(vertex[faces[i]->faces[j] - 1]->x, vertex[faces[i]->faces[j] - 1]->y, vertex[faces[i]->faces[j] - 1]->z);
						vec2 tmpvt = vec2(0.0f, 0.0f);
						vec3 tmpvn = tmpv;
						vertices.emplace_back(Vertex{ tmpv, tmpvn, tmpc, tmpvt });
					}
				}
			}
		}

		in.close();
		PROMISE(!vertices.empty());
	}
}