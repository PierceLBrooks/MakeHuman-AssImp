
#include "ObjModel.hpp"

mh::ObjModel::ObjModel()
{
}

mh::ObjModel::~ObjModel()
{
    m_vertexPositions.clear();
    m_vertexTextureCoordinates.clear();
    m_vertexNormals.clear();
    m_faceData.clear();
    m_vertices.clear();
}

mh::ObjModel::Vertex mh::ObjModel::makeVertex(const std::string& indices)
{
    Vertex vertex;
    std::istringstream indiceStream(indices);

    unsigned int positionIndex = 0;
    unsigned int textureCoordinateIndex = 0;
    unsigned int normalIndex = 0;
    char separator = 0;

    if (!(indiceStream >> positionIndex >> separator).good())
    {
        return vertex;
    }

    if (!(indiceStream >> textureCoordinateIndex >> separator).good())
    {
        return vertex;
    }

    if (!(indiceStream >> normalIndex).eof())
    {
        return vertex;
    }

    // .obj indices start at 1
    positionIndex -= 1;
    textureCoordinateIndex -= 1;
    normalIndex -= 1;

    if ((textureCoordinateIndex >= m_vertexTextureCoordinates.size()) && (m_vertexTextureCoordinates.empty()))
    {
        m_vertexTextureCoordinates.push_back(Vector2f());
    }

    if ((positionIndex >= m_vertexPositions.size()) ||
        (textureCoordinateIndex >= m_vertexTextureCoordinates.size()) ||
        (normalIndex >= m_vertexNormals.size()))
    {
        return vertex;
    }

    vertex.positionIndex = positionIndex;
    vertex.position = m_vertexPositions[positionIndex];
    vertex.texCoordsIndex = textureCoordinateIndex;
    vertex.texCoords = m_vertexTextureCoordinates[textureCoordinateIndex];
    vertex.normalIndex = normalIndex;
    vertex.normal = m_vertexNormals[normalIndex];

    return vertex;
}

unsigned int mh::ObjModel::getVertexCount() const
{
    return m_vertices.size();
}

void mh::ObjModel::addVertex(const Vertex& vertex)
{
    m_vertices.push_back(vertex);
}

void mh::ObjModel::addFace(unsigned int vertex0, unsigned int vertex1, unsigned int vertex2)
{
    FaceData face;
    face.position0 = m_vertices[vertex0].positionIndex;
    face.position1 = m_vertices[vertex1].positionIndex;
    face.position2 = m_vertices[vertex2].positionIndex;
    face.textureCoordinate0 = m_vertices[vertex0].texCoordsIndex;
    face.textureCoordinate1 = m_vertices[vertex1].texCoordsIndex;
    face.textureCoordinate2 = m_vertices[vertex2].texCoordsIndex;
    face.normal0 = m_vertices[vertex0].normalIndex;
    face.normal1 = m_vertices[vertex1].normalIndex;
    face.normal2 = m_vertices[vertex2].normalIndex;
    m_faceData.push_back(face);
}

bool mh::ObjModel::loadFromFile(const std::string& filename)
{
    // Open our model file
    std::ifstream modelFile(filename.c_str());
    if (!modelFile.is_open())
    {
        return false;
    }

    // Parse the model file line by line
    std::string line;
    std::istringstream lineStream;
    std::string token;
    std::regex pattern;

    pattern = std::regex("//");

    while (true)
    {
        std::getline(modelFile, line);

        // Break on error or failure to read (end of file)
        if ((modelFile.bad()) || (modelFile.fail()))
        {
            break;
        }

        line = std::regex_replace(line, pattern, "/1/");

        lineStream.clear();
        lineStream.str(line);
        lineStream >> token;

        if (token == "v")
        {
            // Handle vertex positions
            Vector3f position;
            lineStream >> position.x >> position.y >> position.z;
            m_vertexPositions.push_back(position);
        }
        else if (token == "vt")
        {
            // Handle vertex texture coordinates
            Vector2f coordinate;
            lineStream >> coordinate.x >> coordinate.y;
            m_vertexTextureCoordinates.push_back(coordinate);
        }
        else if (token == "vn")
        {
            // Handle vertex normals
            Vector3f normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            m_vertexNormals.push_back(normal);
        }
        else if (token == "f")
        {
            // Handle faces
            std::string vertexString0, vertexString1, vertexString2;

            lineStream >> vertexString0 >> vertexString1 >> vertexString2;

            Vertex vertex0(makeVertex(vertexString0));
            Vertex vertex1(makeVertex(vertexString1));
            Vertex vertex2(makeVertex(vertexString2));

            addVertex(vertex0);
            addVertex(vertex1);
            addVertex(vertex2);

            unsigned int index = getVertexCount();

            addFace(index - 3, index - 2, index - 1);
        }
    }

    return true;
}

bool mh::ObjModel::saveToFile(const std::string& filename, bool includeVertexPositions, bool includeVertexTextureCoordinates, bool includeVertexNormals, bool includeFaces, std::string markSeparator, std::string fieldSeparator, std::string indexSeparator)
{
    unsigned int inclusions = 0;
    std::ofstream output;
    output.open(filename);
    if (!output.is_open())
    {
        return false;
    }
    if (includeVertexPositions)
    {
        inclusions += 1;
        for (unsigned int i = 0; i < m_vertexPositions.size(); ++i)
        {
            output << "v" << markSeparator << m_vertexPositions[i].x << fieldSeparator << -m_vertexPositions[i].z << fieldSeparator << m_vertexPositions[i].y << std::endl;
        }
    }
    if (includeVertexTextureCoordinates)
    {
        inclusions += 1;
        for (unsigned int i = 0; i < m_vertexTextureCoordinates.size(); ++i)
        {
            output << "vt" << markSeparator << m_vertexTextureCoordinates[i].x << fieldSeparator << m_vertexTextureCoordinates[i].y << std::endl;
        }
    }
    if (includeVertexNormals)
    {
        inclusions += 1;
        for (unsigned int i = 0; i < m_vertexNormals.size(); ++i)
        {
            output << "vn" << markSeparator << m_vertices[i].normal.x << fieldSeparator << m_vertexNormals[i].y << fieldSeparator << m_vertexNormals[i].z << std::endl;
        }
    }
    if ((includeFaces) && (inclusions > 0))
    {
        unsigned int temp = inclusions;
        for (unsigned int i = 0; i < m_faceData.size(); ++i)
        {
            output << "f" << markSeparator;
            for (unsigned int j = 0; j < 3; ++j)
            {
                if (includeVertexPositions)
                {
                    switch (j)
                    {
                        case 0:
                            output << m_faceData[i].position0;
                            break;
                        case 1:
                            output << m_faceData[i].position1;
                            break;
                        case 2:
                            output << m_faceData[i].position2;
                            break;
                    }
                    temp -= 1;
                    if (temp == 0)
                    {
                        if (j < 2)
                        {
                            output << fieldSeparator;
                        }
                    }
                    else
                    {
                        output << indexSeparator;
                    }
                }
                if (includeVertexTextureCoordinates)
                {
                    switch (j)
                    {
                        case 0:
                            output << m_faceData[i].textureCoordinate0;
                            break;
                        case 1:
                            output << m_faceData[i].textureCoordinate1;
                            break;
                        case 2:
                            output << m_faceData[i].textureCoordinate2;
                            break;
                    }
                    temp -= 1;
                    if (temp == 0)
                    {
                        if (j < 2)
                        {
                            output << fieldSeparator;
                        }
                    }
                    else
                    {
                        output << indexSeparator;
                    }
                }
                if (includeVertexNormals)
                {
                    switch (j)
                    {
                        case 0:
                            output << m_faceData[i].normal0;
                            break;
                        case 1:
                            output << m_faceData[i].normal1;
                            break;
                        case 2:
                            output << m_faceData[i].normal2;
                            break;
                    }
                    temp -= 1;
                    if (temp == 0)
                    {
                        if (j < 2)
                        {
                            output << fieldSeparator;
                        }
                    }
                    else
                    {
                        output << indexSeparator;
                    }
                }
                if (j == 2)
                {
                    output << std::endl;
                }
                temp = inclusions;
            }
        }
    }
    output.close();
    return true;
}

