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
		// ppm 에 출력할 색상값을 한 줄(row)씩 처리할 때마다 남아있는 줄을 std::clog 로 출력함 > 에러 출력 스트림
		// 참고로, std::flush 는 스트림에 대기중인 버퍼의 내용을 비운 후, 강제로 출력시킴.
		// std::cout 은 색상값 출력 시 사용되므로, 이에 대한 대체제로써 std::clog 를 사용한 것!
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
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

	std::clog << "\rDone.					\n"; // 반복문이 종료되면 .ppm 에 출력할 색상 계산이 완료되었음을 std::clog 로 콘솔 출력함.
}