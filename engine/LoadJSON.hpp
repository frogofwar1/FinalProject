#ifndef  gl_LOAD_JSON
#define  gl_LOAD_JSON

#include "cw_header.hpp"

#include <vector>
#include <fstream>

#include "../nlohmann/json.hpp"

namespace cw {
	struct ModelWTexture
	{
		std::string model;
		std::string texture;
	};

	struct LoadJSON{
		std::vector<std::string> musics;
		std::vector<ModelWTexture> models;

		void loadJsonFile() {

			std::ifstream ifs("resources/configuration.json");
			nlohmann::json jsonFile = nlohmann::json::parse(ifs);

			for (int i = 0; i < static_cast<int>(jsonFile["musics"]["count"]); i++)
			{
				musics.push_back(jsonFile["musics"][std::to_string(i)]["name"]);
			}

			for (int i = 0; i < static_cast<int>(jsonFile["models"]["count"]); i++)
			{
				models.push_back({ jsonFile["models"][std::to_string(i)]["name"], jsonFile["models"][std::to_string(i)]["texture"] });
			}

			PROMISE(!musics.empty());
			PROMISE(!models.empty());
		}
	};
}
#endif