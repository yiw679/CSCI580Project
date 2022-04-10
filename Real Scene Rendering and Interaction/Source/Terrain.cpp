#include "Terrain.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Terrain::Terrain(int size, float scale, float heightScale)
: m_Xmw(1)
{
	m_size = size;
	points.resize(m_size, vector<float>(m_size,0));

	int squareSize = m_size * m_size;
	m_PositionBuffer.resize(squareSize);
	m_NormalBuffer.resize(squareSize);
	m_ColorBuffer.resize(squareSize);

	int numTriangles = (m_size - 1) * (m_size - 1) * 2;

	m_IndexBuffer.resize(numTriangles * 3);

	m_scale = (m_size - 1) * scale;
	m_heightScale = heightScale;
}

float Terrain::GetHeight(int x, int y) {
	return points[y][x];
}

void Terrain::SetHeight(int x, int y, float val) {
	points[y][x] = val;
}

void Terrain::Generate(float mag, float roughness) {
	srand(time(NULL));

	int stepSize = (m_size - 1) / 2;

	while (stepSize > 0) {

		for (int y = 0; y < m_size/(stepSize * 2); y++)
		{
			for (int x = 0; x < m_size / (stepSize * 2); x++)
			{
				SquareStep((x * stepSize * 2) + stepSize, (y * stepSize * 2) + stepSize, stepSize, rand() % int(2 * mag) - mag);
			}
		}

		bool flag = true;

		for (int y = 0; y < m_size; y += stepSize)
		{

			if (flag)
			{
				for (int x = stepSize; x < m_size; x += 2 * stepSize)
				{
					DiamondStep(x, y, stepSize, rand() % int(2 * mag) - mag);
				}
			}
			else
			{
				for (int x = 0; x < m_size; x += stepSize * 2)
				{
					DiamondStep(x, y, stepSize, rand() % int(2 * mag) - mag);
				}
			}

			flag = !flag;
		}

		mag *= roughness;
		stepSize /= 2;
	}

	for (int y = 0; y < m_size; y++)
	{
		for (int x = 0; x < m_size; x++)
		{
			int index = y * m_size + x;

			float m_x = x / (float)(m_size - 1) * m_scale - m_scale / 2;
			float m_y = points[y][x] * m_heightScale;
			float m_z = y / (float)(m_size - 1) * m_scale - m_scale / 2;

			m_PositionBuffer[index] = glm::vec3(m_x, m_y, m_z);
			m_NormalBuffer[index] = glm::vec3(0,0,0);
			m_ColorBuffer[index] = glm::vec4(m_y, 1.0f, 1.0f, 1.0f);
		}
	}

	GenerateIndexBuffer();
	GenerateNormals();
	GenerateVertexBuffers();
}

void Terrain::GenerateIndexBuffer() {
	int index = 0;

	for (int y = 0; y < m_size - 1; y++)
	{
		for (int x = 0; x < m_size - 1; x++)
		{
			int vertexIndex = y * m_size + x;

			m_IndexBuffer[index++] = vertexIndex;
			m_IndexBuffer[index++] = vertexIndex + m_size + 1;
			m_IndexBuffer[index++] = vertexIndex + 1;

			m_IndexBuffer[index++] = vertexIndex;
			m_IndexBuffer[index++] = vertexIndex + m_size;
			m_IndexBuffer[index++] = vertexIndex + m_size + 1;
		}
	}
}

void Terrain::GenerateNormals() {
	for (int i = 0; i < m_IndexBuffer.size(); i += 3)
	{
		glm::vec3 v1 = m_PositionBuffer[m_IndexBuffer[i + 0]];
		glm::vec3 v2 = m_PositionBuffer[m_IndexBuffer[i + 1]];
		glm::vec3 v3 = m_PositionBuffer[m_IndexBuffer[i + 2]];

		glm::vec3 normal = glm::normalize(glm::cross(v2-v1, v3-v1));

		m_NormalBuffer[m_IndexBuffer[i + 0]] += normal;
		m_NormalBuffer[m_IndexBuffer[i + 1]] += normal;
		m_NormalBuffer[m_IndexBuffer[i + 2]] += normal;
	}

	for (int i = 0; i < m_NormalBuffer.size(); i++)
	{
		m_NormalBuffer[i] = glm::normalize(m_NormalBuffer[i]);
	}
}

void Terrain::GenerateVertexBuffers() {
	glGenBuffers(1,&m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_PositionBuffer.size(), &(m_PositionBuffer[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &m_NormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_NormalBuffer.size(), &(m_NormalBuffer[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &m_ColorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * m_ColorBuffer.size(), &(m_ColorBuffer[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * m_IndexBuffer.size(), &(m_IndexBuffer[0]), GL_STATIC_DRAW);
}

void Terrain::Render() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(m_Xmw));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, m_ColorBufferID);
	glColorPointer(4, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBufferID);
	glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Terrain::LogTerrain() {
	for (int x = 0; x < m_size; x++)
	{
		for (int y = 0; y < m_size; y++)
		{
			cout << points[y][x];
		}
		cout << endl;
	}
}

void Terrain::SquareStep(int x, int y, int stepSize, float stepOffset) {
	points[y][x] =
		(points[y + stepSize][x + stepSize] +
			points[y + stepSize][x - stepSize] +
			points[y - stepSize][x + stepSize] +
			points[y - stepSize][x - stepSize]) / 4 + stepOffset;
}

void Terrain::DiamondStep(int x, int y, int stepSize, float stepOffset) {

	int divident = 0;
	float sum = 0;

	if (x - stepSize >= 0) {
		sum += points[y][x - stepSize];
		divident++;
	}
	if (x + stepSize < m_size) {
		sum += points[y][x + stepSize];
		divident++;
	}
	if (y - stepSize >= 0) {
		sum += points[y - stepSize][x];
		divident++;
	}
	if (y + stepSize < m_size) {
		sum += points[y + stepSize][x];
		divident++;
	}

	points[y][x] = sum / divident + stepOffset;
}
