#include <iostream>

int main()
{
	// Image

	// .ppm 파일의 rows 와 column(너비와 높이 해상도) 정의
	int image_width = 256;
	int image_height = 256;

	// Render

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n"; // PPM Example 의 1번째에서 3번째 줄까지의 메타 정보를 출력

	// 각 픽셀의 색상값(r, g, b)을 0 ~ 256 사이의 scale 된 정수값 범위로 출력
	for (int j = 0; j < image_height; ++j)
	{
		for (int i = 0; i < image_width; ++i)
		{
			// 관례적으로, r, g, b 값은 0 ~ 1 사이의 범위의 실수형으로 맵핑해서 계산해놓음.
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0;

			// 그러나, .ppm 에 출력할 때에는 .ppm 의 rows와 columns 에 해당하는
			// 0 ~ 256 범위 내의 정수형으로 형변환하여 출력함.
			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
}