#include <iostream>
#include <sstream>
#include <cstdio>

#define NANOSVG_IMPLEMENTATION
#define NANOSVGRAST_IMPLEMENTATION

#include "libs/nanosvg/src/nanosvg.h"
#include "libs/nanosvg/src/nanosvgrast.h"
#include "parser/cmdline.h"

using namespace std;
using namespace cmdline;

typedef struct PointMap {
	float x;
	float y;
} PointMap;

// cygwin to_string patch
namespace patch {
	template<typename T>
	std::string to_string(const T &n) {
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}

/**
 *
 * @param map
 */
void printContextDrawer(vector<PointMap> *map);

/**
 *
 * @param map
 * @param pretty
 */
void printJsonArray(vector<PointMap> *map, bool pretty = true, FILE *file = NULL);

/**
 * @param map Empty map
 * @param raster
 * @return
 */
void fillPointMap(vector<PointMap> *map, NSVGrasterizer *raster);

int main(int argc, char **argv) {
	int scale = 4;
	int stride = 4;
	float density = 96.0f;
	bool printJS = false;
	bool debug = false;
	const char *outputFile = NULL;
	FILE *file = NULL;

	cmdline::parser *parser = new cmdline::parser();
	parser->add<string>("svg", 'p', "Path to svg", true);
	parser->add<string>("output", 'f', "Output file path: /tmp/test.json", false);
	parser->add<int>("scale", 's', "Image scale", false, scale);
	parser->add<bool>("debug", 'v', "Prints some debug info", false, debug);
	parser->add<bool>("js", 'j', "Prints js context drawer", false, printJS);

	parser->parse_check(argc, argv);
	scale = parser->get<int>("scale");
	const char *filePath = parser->get<string>("svg").c_str();
	debug = parser->get<bool>("debug");
	printJS = parser->get<bool>("js");
	if(parser->exist("output")) {
		outputFile = parser->get<string>("output").c_str();
	}

	NSVGimage *image = NULL;
	NSVGrasterizer *raster = nsvgCreateRasterizer();
	int width, height;

	image = nsvgParseFromFile(filePath, "px", density);
	if (image == NULL) {
		printf("Could not open SVG image.\n");
		return EXIT_FAILURE;
	}
	width = (int) image->width;
	height = (int) image->height;

	unsigned char *img = new unsigned char[(long) (image->width * image->height * 4)];

	nsvgRasterizeFull(raster, image, 0, 0, scale, scale, img, width, height, stride);
	vector<PointMap> map((unsigned long) raster->npoints);

	if(debug) {
		cout << "NPoints: " << raster->npoints << endl;
		cout << "NPoints2: " << raster->npoints2 << endl;
		cout << "NEdges: " << raster->nedges << endl;
		cout << "Vector size: " << map.size() << endl;
	}

	fillPointMap(&map, raster);

	if(outputFile != NULL) {
		file = fopen(outputFile, "w");
	}

	printJsonArray(&map, true, file);

	if(printJS) {
		printContextDrawer(&map);
	}

	delete parser;
	delete[] img;
	nsvgDeleteRasterizer(raster);
	nsvgDelete(image);
	fclose(file);

	return EXIT_SUCCESS;
}

void printContextDrawer(vector<PointMap> *map) {
	for (vector<PointMap>::iterator item = map->begin(); item != map->end(); ++item) {
		printf("ctx.moveTo(%f, %f);\n", (*item).x, (*item).y);
		printf("ctx.strokeRect(%f, %f, 1, 1);\n", (*item).x, (*item).y);
	}
}

void printJsonArray(vector<PointMap> *map, bool pretty, FILE *file) {
	string newLine = "";
	string tabChar = "";
	if (pretty) {
		newLine = "\n";
		tabChar = "\t";
	}

	string buffer;
	buffer.append("[").append(newLine);

	int i = 0;
	for(vector<PointMap>::iterator item = map->begin(); item != map->end(); ++item) {

		buffer
			.append(tabChar)
			.append("[")
			.append(patch::to_string((*item).x))
			.append(",")
			.append(patch::to_string((*item).y))
			.append("]");

		if ((i + 1) != map->size()) {
			buffer.append(",");
		}
		buffer.append(newLine);
		i++;
	}
	buffer.append("]");
	buffer.append(newLine);

	if(file != NULL) {
		fwrite(buffer.c_str(), sizeof(char), buffer.length(), file);
		return;
	}

	cout << buffer << endl;
}

void fillPointMap(vector<PointMap> *map, NSVGrasterizer *raster) {
	for(unsigned long i = 0; i < raster->npoints; i++) {
		PointMap tmp = PointMap();
		tmp.x = raster->points[i].x;
		tmp.y = raster->points[i].y;
		(*map)[i] = tmp;
	}
}