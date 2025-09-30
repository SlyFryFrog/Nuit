module;
#include <fstream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <print>
#include <vector>
module map;

Map::Map()
{
}

Map::Map(int width, int height)
{
}

Map::Map(int width, int height, const std::shared_ptr<Window>& window,
		 const std::shared_ptr<std::vector<Ray>>& rays) :
	m_rays(rays), m_window(window)
{
}

void Map::draw_grid_view(GLShaderProgram& shader)
{
}

void Map::draw_perspective_view(GLShaderProgram& shader)
{
	m_halfWidth = m_window->get_frame_buffer_width() / 2;
	m_fullHeight = m_window->get_frame_buffer_height();

	// uint32_t textureBuffer[halfWidth * fullHeight];

	Window::set_viewport(m_halfWidth, 0, m_halfWidth, m_fullHeight);

	const glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(m_halfWidth) * 2,
									  static_cast<float>(m_fullHeight), 0.0f, -1.0f, 1.0f);

	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", glm::mat4(1.0f));
	shader.set_uniform("uModel", glm::mat4(1.0f));

	draw_walls(shader);
	shader.unbind(); // Optionally call unbind
}

void Map::generate()
{
}

void Map::load_map(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::println(std::cerr, "Failed to open file: ", path);
		return;
	}

	std::vector<std::vector<int>> map;

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::vector<int> row;
		int value;

		while (iss >> value) {
			row.push_back(value);
		}

		map.push_back(row);
	}

	generatedMap = map;
}

void Map::save_map(const std::string& path)
{
	std::stringstream content;

	// Iterate through map and append to string
	for (const auto& i : generatedMap)
	{
		for (const auto& j : i)
		{
			content << j << " ";
		}

		content << "\n";
	}

	File::write(path, content.str());
}

void Map::draw_walls(GLShaderProgram& shader)
{
	const auto projectPlaneDistance = m_window->get_frame_buffer_width() /
														 (2 * tan(FOV / 2));

	for (int col = 0; col < m_rays->size(); col++)
	{
		auto& ray = m_rays->at(col);

		// Ignore rays that never hit a wall
		if (!ray.Hit)
		{
			continue;
		}

		// Calculate distance from start to end of ray.
		// Then define wall height based on the distance
		const float distance = glm::length(ray.EndPosition - ray.Position);

		const float normDistance = distance *
			cos(glm::radians(FOV / 2 - col * rate)); // Remove fisheye effect

		const float wallHeight = (WallTileSize * projectPlaneDistance) / normDistance;

		// Calculate wall top and bottom relative to OpenGL's coordinate systems
		const float yTop = (static_cast<float>(m_fullHeight) / 2) - (wallHeight / 2);
		const float yBot = yTop + wallHeight;

		// Map column to screen-space within right viewport
		const float colX = (col * Step * 2);

		// Set uniforms based on tile number
		if (generatedMap[ray.MapPosition.y][ray.MapPosition.x] == 2)
		{
			shader.set_uniform("uColor", glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
		}
		else if (generatedMap[ray.MapPosition.y][ray.MapPosition.x] == 3)
		{
			shader.set_uniform("uColor", glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
		}
		else
		{
			shader.set_uniform("uColor", glm::vec4{0.0f, 0.0f, 1.0f, 1.0f});
		}

		ray.draw_vertical_line(colX, yTop, colX + 2 * Step, yBot);
	}
}

void Map::draw_floors(GLShaderProgram& shader)
{
}

void Map::draw_ceilings(GLShaderProgram& shader)
{
}
