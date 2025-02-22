#include "Renderer.h"
#include "Walnut/Random.h"


namespace Utils {

	static uint32_t ConvertToRGAB(const glm::vec4 & color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;

		return result;


	}

}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		//no resize necessary 
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight())
			return;
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete m_ImageDAta;
	m_ImageDAta = new uint32_t[width * height];

}


void Renderer::Render()
{
	//render every pixel
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;

			glm::vec4 color = PerPixel(coord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
				
			m_ImageDAta[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGAB(color);
			
		}

	}
	
	m_FinalImage->SetData(m_ImageDAta);

}

const float MAX_ITER = 500.0f;

float Mandelbrot(glm::vec2 coord)
{
	glm::vec2 c = 4.0f * coord - glm::vec2(0.7f, 0.0f);
	glm::vec2 z = glm::vec2(0.0f, 0.0f);
	float iter = 0.0f;
	for (float i = 0.0f; i < MAX_ITER; i++)
	{
		z = glm::vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
		if (glm::dot(z, z) > 4.0)
			return iter / MAX_ITER;
		iter++;
	}
	return 0.0f;
}

glm::vec3 hash13(float m)
{
	float x = glm::fract(sin(m) * 5625.246);
	float y = glm::fract(sin(m + x) * 2216.486);
	float z = glm::fract(sin(x + y) * 8276.352);

	return glm::vec3(x, y, z);
}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	//uint8_t r = (uint8_t)(coord.x * 255.0f);
	//uint8_t g = (uint8_t)(coord.y * 255.0f);
	//return 0xff000000 | (g << 8) | r;

	glm::vec3 col = glm::vec3(0.0f);
	float m = Mandelbrot(coord);
	col += hash13(m);
	
	return glm::vec4(col, 1.0f);



	
}
