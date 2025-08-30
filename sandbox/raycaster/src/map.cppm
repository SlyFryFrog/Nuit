module;
#include <memory>
#include <sstream>
#include <string>
#include <vector>
export module map;

import nuit;
using namespace Nuit;

export constexpr float WallTileSize = 1.0f; // Changes the ratio for the height of the walls
export constexpr float FOV = 90;
export float rate;

export std::vector<std::vector<int>> generatedMap = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 1},
						{1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 1},
						{1, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 1},
						{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 3, 3, 0, 0, 1},
						{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 3, 0, 0, 0, 1},
						{1, 0, 1, 3, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 3, 3, 0, 0, 0, 1},
						{1, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
						{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

export class Map
{
	std::shared_ptr<std::vector<Ray>> m_rays;
	std::shared_ptr<Window> m_window;

	int m_halfWidth;
	int m_fullHeight;

public:
	Map();
	Map(int width, int height);
	Map(int width, int height, const std::shared_ptr<Window>& window,
		const std::shared_ptr<std::vector<Ray>>& rays);

	void draw_grid_view(const GLShaderProgram& shader);

	void draw_perspective_view(const GLShaderProgram& shader);

	void generate();

	void use_texture(int tile);

	static void load_map(const std::string& path);

	static void save_map(const std::string& path);

private:
	void draw_walls(const GLShaderProgram& shader);
	void draw_floors(const GLShaderProgram& shader);
	void draw_ceilings(const GLShaderProgram& shader);
};
