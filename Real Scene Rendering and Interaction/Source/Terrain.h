#ifndef TERRAIN
#define TERRAIN

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include "stb_image.h"
#include <limits>

#define TEXTURE_NUM 4


using namespace std;

typedef vector<glm::vec3> PositionBuffer;
typedef vector<glm::vec4> ColorBuffer;
typedef vector<glm::vec3> NormalBuffer;
typedef vector<glm::vec2> TexCoordBuffer;
typedef vector<GLuint> IndexBuffer;

class Terrain {
	int m_size;
	vector<vector<float>> points;
	float m_scale;
	float m_heightScale;

public:

	float MaxHeight;
	float MinHeight;

	float grassToRockPercentage;
	float rockToSnowPercentage;
	float mixingThreshold;

	Terrain(int size, float size_scale, float height_scale);

	void SetHeight(int x, int y, float val);

	float GetHeight(int x, int y);

	float GetHeightScale() const { return m_heightScale; }
	bool LoadTexture(const char* fileName, unsigned int textureIndex);

	void Generate(float mag, float roughness);

	void LogTerrain();

	void Render();

	glm::mat4x4 m_Xmw;

private:

	PositionBuffer m_PositionBuffer;
	ColorBuffer m_ColorBuffer;
	NormalBuffer m_NormalBuffer;
	IndexBuffer m_IndexBuffer;
	TexCoordBuffer m_TexCoordBuffer;

	GLuint  m_VertexBufferID;
	GLuint  m_NormalBufferID;
	GLuint  m_ColorBufferID;
	GLuint  m_IndexBufferID;
	GLuint	m_Tex1BufferID;
	GLuint	m_Tex2BufferID;
	GLuint	m_Tex3BufferID;
	GLuint	m_Tex4BufferID;

	GLuint	m_GLTextures[TEXTURE_NUM];

	void DiamondStep(int x, int y, int stepSize, float stepOffset);

	void SquareStep(int x, int y, int stepSize, float stepOffset);

	void GenerateIndexBuffer();

	void GenerateNormals();

	void GenerateVertexBuffers();
};

#endif