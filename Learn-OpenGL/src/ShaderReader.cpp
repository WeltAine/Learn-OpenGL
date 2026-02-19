#include "ShaderReader.h"
#include <fstream>
#include <sstream>

namespace LearnOpenGL {

	std::string ShaderReader::Read(std::string path) {

		std::ifstream in(path);
		std::stringstream buffer;
		buffer << in.rdbuf();
		in.close();
		return buffer.str();

	}

}
