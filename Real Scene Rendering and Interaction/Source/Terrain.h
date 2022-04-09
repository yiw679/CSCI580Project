#ifndef TERRAIN
#define TERRAIN

#include <vector>
#include <glm/glm.hpp>

using namespace std;

class Terrain {
	int size;
	vector<vector<float>> points;

public:

	Terrain(int size);

	void SetHeight(int x, int y, float val);
	float GetHeight(int x, int y);

	void Generate(float roughness);

	void LogTerrain();

private:
	void DiamondStep(int x, int y, int stepSize, float stepOffset);

	void SquareStep(int x, int y, int stepSize, float stepOffset);

};

#endif