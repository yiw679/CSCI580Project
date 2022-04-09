#include "Terrain.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;

Terrain::Terrain(int _size) {
	size = _size;
	points.resize(size, vector<float>(size,0));
}

float Terrain::GetHeight(int x, int y) {
	return points[y][x];
}

void Terrain::SetHeight(int x, int y, float val) {
	points[y][x] = val;
}

void Terrain::Generate(float roughness) {
	srand(time(NULL));


	int stepSize = (size - 1) / 2;
	int step = 0;
	float stepOffset;

	while (stepSize > 0) {
		int curStep = pow(2, step);

		for (int y = 0; y < curStep; y++)
		{
			for (int x = 0; x < curStep; x++)
			{
				stepOffset = rand() % (int)roughness * stepSize * 2 - stepSize * roughness;

				SquareStep((x * stepSize * 2) + stepSize, (y * stepSize * 2) + stepSize, stepSize, stepOffset);
			}
		}

		bool flag = true;

		for (int y = 0; y < size; y += stepSize)
		{

			if (flag)
			{
				for (int x = stepSize; x < size; x += 2 * stepSize)
				{
					stepOffset = rand() % (int)roughness * stepSize * 2 - stepSize * roughness;

					DiamondStep(x, y, stepSize, stepOffset);
				}
			}
			else
			{
				for (int x = 0; x < size; x += stepSize * 2)
				{
					stepOffset = rand() % (int)roughness * stepSize * 2 - stepSize * roughness;

					DiamondStep(x, y, stepSize, stepOffset);
				}
			}

			flag = !flag;
		}

		step++;
		stepSize /= 2;
	}
}

void Terrain::LogTerrain() {
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
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
	if (x + stepSize < size) {
		sum += points[y][x + stepSize];
		divident++;
	}
	if (y - stepSize >= 0) {
		sum += points[y - stepSize][x];
		divident++;
	}
	if (y + stepSize < size) {
		sum += points[y + stepSize][x];
		divident++;
	}

	points[y][x] = sum / divident + stepOffset;
}
