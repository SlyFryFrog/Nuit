module;
#include <memory>
#include <vector>
export module nuit:app;

import :layer;
import :timer;
import :window;
import :gl_renderer;

namespace Nuit
{
	export class App
	{
		std::string m_title{"Untitled Project"};
		std::vector<std::unique_ptr<Layer>> m_layers;
		Timer m_timer;
		Window m_window;

	public:
		App() = default;
		explicit App(const std::string& title) : m_title{title}
		{
		}

		~App() = default;

		void init()
		{
			m_window.init();
			m_window.set_title(m_title);
			GLRenderer::init();
		}

		void run()
		{
			m_timer.start();

			while (!m_window.is_done())
			{
				const double delta = m_timer.delta();
				process_layers(delta);
			}
		}

		void push_layer(std::unique_ptr<Layer> layer)
		{
			m_layers.push_back(std::move(layer));
		}

		void init_layers() const
		{
			for (const auto& layer : m_layers)
			{
				layer->_init();
			}
		}

		void process_layers(const double delta) const
		{
			for (const auto& layer : m_layers)
			{
				layer->_process(delta);
			}
		}

		void render_layers() const
		{
			for (const auto& layer : m_layers)
			{
				layer->_render();
			}
		}
	};
} // namespace Nuit
