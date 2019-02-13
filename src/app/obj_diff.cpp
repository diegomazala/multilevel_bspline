#include <iostream>
#include "tinyobj.h"
#include <thread>

#define _USE_MATH_DEFINES
#include <cmath>


static auto get_rgb_color_ramp(float value)
{
	int aR = 0;   int aG = 0; int aB = 255;  // RGB for our 1st color (blue in this case).
	int bR = 255; int bG = 0; int bB = 0;    // RGB for our 2nd color (red in this case).

	float rgb[3] = 
	{
		(float)(bR - aR) * value + aR,    // Evaluated as -255*value + 255.
		(float)(bG - aG) * value + aG,    // Evaluates as 0.
		(float)(bB - aB) * value + aB     // Evaluates as 255*value + 0.
	};

	return rgb;
}

static auto get_heat_map_rgb(float value)
{
	const int NUM_COLORS = 4;
	static float color[NUM_COLORS][3] = { {0,0,1}, {0,1,0}, {1,1,0}, {1,0,0} };
	// A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.

	int idx1;        // |-- Our desired color will be between these two indexes in "color".
	int idx2;        // |
	float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.

	if (value <= 0) { idx1 = idx2 = 0; }    // accounts for an input <=0
	else if (value >= 1) { idx1 = idx2 = NUM_COLORS - 1; }    // accounts for an input >=0
	else
	{
		value = value * (NUM_COLORS - 1);        // Will multiply value by 3.
		idx1 = floor(value);                  // Our desired color will be after this index.
		idx2 = idx1 + 1;                        // ... and before this index (inclusive).
		fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).
	}

	float rgb[3] =
	{
		(color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0],
		(color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1],
		(color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2]
	};
	return rgb;
}

int main(int argc, char* argv[]) 
{ 
	if (argc < 3)
	{
        std::cerr << "Usage: app <filename_inA> <filename_inB> <filename_out>\n";
        return EXIT_FAILURE;
	}

	const std::string filename_inA = argv[1];
	const std::string filename_inB = argv[2];
	const std::string filename_out = argv[3];

	std::cout << "[Info] Loading obj file " << filename_inA << std::endl;
	tinyobj::scene_t sceneA, sceneB;
	bool success_loadingA, success_loadingB;


	std::vector<std::thread> thread;
	{
		std::cout << "Loading: " << filename_inA << std::endl;
		thread.push_back(std::thread([&] {
			success_loadingA = tinyobj::load(sceneA, filename_inA);;
		}));

		std::cout << "Loading: " << filename_inB << std::endl;
		thread.push_back(std::thread([&] {
			success_loadingB = tinyobj::load(sceneB, filename_inB);;
		}));
	}

	for (auto &t : thread)
		t.join();

	if (!success_loadingA)
	{
		std::cerr << "[Error] Loading obj file : " << filename_inA << std::endl;
		return EXIT_FAILURE;
	}

	if (!success_loadingB)
	{
		std::cerr << "[Error] Loading obj file : " << filename_inB << std::endl;
		return EXIT_FAILURE;
	}


	tinyobj::print_info(sceneA);
	tinyobj::print_info(sceneB);

	if (sceneA.attrib.vertices.size() != sceneB.attrib.vertices.size())
	{
		std::cerr << "[Error] The number of vertices must match. " 
			<< sceneA.attrib.vertices.size() << " != " << sceneB.attrib.vertices.size() << std::endl;
		return EXIT_FAILURE;
	}

	const std::size_t vcount = sceneA.attrib.vertices.size();
	auto& colors = sceneA.attrib.colors;
	auto& va = sceneA.attrib.vertices;
	auto& vb = sceneB.attrib.vertices;
	std::vector<tinyobj::real_t> dist(vcount / 3, 0);
	float mindist = FLT_MAX;
	float maxdist = FLT_MIN;
	for (std::size_t i = 0; i < vcount; i+=3)
	{
		std::size_t j = i / 3;
		auto x = fabs(va[i + 0] - vb[i + 0]);
		auto y = fabs(va[i + 1] - vb[i + 1]);
		auto z = fabs(va[i + 2] - vb[i + 2]);

		dist[j] = std::sqrt(x * x + y * y + z * z);
		
		if (mindist > dist[j])
			mindist = dist[j];
		if (maxdist < dist[j])
			maxdist = dist[j];
	}

	std::cout << "Distance (min, max) :" << mindist << ' ' << maxdist << std::endl;

	for (auto i = 0; i < vcount; i+=3)
	{
		const float dist_norm = (dist[i / 3] - mindist) / (maxdist - mindist);

		const auto rgb = get_heat_map_rgb(dist_norm);
			//get_rgb_color_ramp(dist_norm);
		colors[i + 0] = rgb[0];
		colors[i + 1] = rgb[1];
		colors[i + 2] = rgb[2];
	}

	std::cout << "[Info] Saving obj file : " << filename_out << std::endl;
	auto success_saving = tinyobj::save(sceneA, filename_out);

	if (!success_saving)
	{
		std::cerr << "[Error] Saving obj file : " << filename_out << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "---- x ----" << std::endl;



	return EXIT_SUCCESS;
}

