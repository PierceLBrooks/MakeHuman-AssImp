
#ifndef MH_OBJ_MODEL_HPP
#define MH_OBJ_MODEL_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace mh
{
    class ObjModel
    {
        public:
            struct Vertex
            {
                public:
                    Vector3f position;
                    Vector2f texCoords;
                    Vector3f normal;
                    unsigned int positionIndex;
                    unsigned int texCoordsIndex;
                    unsigned int normalIndex;
            };

            ObjModel();
            virtual ~ObjModel();

        private:
            struct FaceData
            {
                public:
                    unsigned int position0;
                    unsigned int position1;
                    unsigned int position2;
                    unsigned int textureCoordinate0;
                    unsigned int textureCoordinate1;
                    unsigned int textureCoordinate2;
                    unsigned int normal0;
                    unsigned int normal1;
                    unsigned int normal2;
            };

            Vertex makeVertex(const std::string& indices);

        public:
            unsigned int getVertexCount() const;
            void addVertex(const Vertex& vertex);
            void addFace(unsigned int vertex0, unsigned int vertex1, unsigned int vertex2);
            bool loadFromFile(const std::string& filename);
            bool saveToFile(const std::string& filename, bool includeVertexPositions = true, bool includeVertexTextureCoordinates = true, bool includeVertexNormals = true, bool includeFaces = true, std::string markSeparator = " ", std::string fieldSeparator = " ", std::string indexSeparator = "/");

        private:
            std::vector<Vector3f> m_vertexPositions;
            std::vector<Vector2f> m_vertexTextureCoordinates;
            std::vector<Vector3f> m_vertexNormals;
            std::vector<FaceData> m_faceData;
            std::vector<Vertex> m_vertices;
    };
}

#endif

