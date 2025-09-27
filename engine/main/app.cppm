module;
#include <memory>
#include <vector>
export module nuit:app;

import :layer;
import :timer;

namespace Nuit
{
	export class App
	{
		std::vector<std::unique_ptr<Layer>> m_layers;
		Timer m_timer;

	public:
		App() = default;
		~App() = default;

		void run()
		{

			m_timer.start();

			while (true)
			{
				const double delta = m_timer.delta();
				process_layers(delta);
			}
		}

		void push_layer(std::unique_ptr<Layer> layer)
		{
			m_layers.push_back(std::move(layer));
		}

		void init_layers()
		{
			for (const auto& layer : m_layers)
			{
				layer->_init();
			}
		}

		void process_layers(const double delta)
		{
			for (const auto& layer : m_layers)
			{
				layer->_process(delta);
			}
		}

		void render_layers()
		{
			for (const auto& layer : m_layers)
			{
				layer->_render();
			}
		}
	};
} // namespace Nuit
