module;
#include <memory>
#include <sstream>
#include <string>
#include <vector>
export module map;

import ray_2d;
import nuit;
using namespace Nuit;

export constexpr float WallTileSize = 1.25f; // Changes the ratio for the height of the walls
export constexpr float FOV = 90;
export constexpr float Step = 2.0f;			// Step size used when drawing walls
export constexpr float Zoom = 10.0f;
export float rate;

export inline std::vector<std::vector<int>> generatedMap;

export class Map
{
	std::shared_ptr<std::vector<Ray>> m_rays;
	std::shared_ptr<Window> m_window;

	int m_halfWidth{};
	int m_fullHeight{};
	int m_width{};
	int m_height{};

public:
	Map();
	Map(int width, int height);
	Map(int width, int height, const std::shared_ptr<Window>& window,
		const std::shared_ptr<std::vector<Ray>>& rays);

	void draw_grid_view(GLShaderProgram& shader);
	void draw_perspective_view(GLShaderProgram& shader);

	void generate();
	void load_map(const std::string& path);
	void save_map(const std::string& path);

	void use_texture(int tile);

private:
	void draw_walls(GLShaderProgram& shader);
	void draw_floors(GLShaderProgram& shader);
	void draw_ceilings(GLShaderProgram& shader);
};
