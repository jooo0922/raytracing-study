#include "color.h"
#include "vec3.h"

#include <iostream>

int main()
{
	// Image

	// 이미지(.ppm 파일)의 rows 와 column(너비와 높이 해상도) 정의
	/*
		우선 이미지 사이즈는 rows 와 columns 개수를 정의하다보니
		반드시 정수형으로 떨어져야 한다는 전제조건이 있음.

		또한, 이미지의 aspect_ratio 를 16:9 로 설정했더라도,
		두 가지 이유에 의해 정확히 16:9 로 비율이 딱 떨어지진 않음.

		1. 우선 이미지 너비를 aspect_ratio 와 곱한 결과를 
		int 타입으로 형변환하므로(이미지 사이즈가 정수형이어야 한다고 했었지?), 
		가장 가까운 정수형으로 소수점이 버림되어서 
		정확한 aspect_ratio 에 딱 맞는 height 을 계산할 수 없음.

		2. 심지어 1번 처럼 계산한 image_height 이 1보다 작아지면
		무조건 1로 덮어쓰기 때문에, 16:9 비율로 딱 떨어지게 계산될 수 없음.

		aspect_ratio 로 설정한 이미지의 종횡비는
		어디까지나 이상적인 값이며, 실제 이미지 크기는
		여기에 근사된 수치로 정해진다.
	*/
	auto aspect_ratio = 16.0 / 9.0; // 이미지의 종횡비를 16:9 로 설정 
	int image_width = 400; // 이미지 너비는 상수이며, 이미지 너비 값에 따라 이미지 높이 값이 종횡비와 곱해져서 계산됨.
	int image_height = static_cast<int>(image_width / aspect_ratio); // 이미지 높이는 정수형이므로, 너비와 종횡비를 곱한 실수값을 정수형으로 형변환함.
	image_height = (image_height < 1) ? 1 : image_height; // 이미지 너비는 항상 1보다는 크도록 함.

	// 실제로 계산된 색상을 저장하는 이미지 외에도, 3D Scene 에 존재하는 가상의 viewport 사이즈도 정의해야 함.
	/*
		viewport

		카메라 지점(눈 지점)으로부터 ray 를 쏘면,
		이 ray 가 가상의 viewport 에 일정 간격으로 정렬된 픽셀들을 통과시킴.

		이러한 픽셀들이 grid 형태로 정렬되어 있는
		3D Scene 안에 존재하는 가상의 직사각형을
		'viewport' 라고 함.

		이때, viewport 의 가로 / 세로 종횡비는
		이미지의 가로 / 세로 종횡비와 일치시켜야 함.

		단, viewport 사이즈는 이미지처럼 rows, column 으로
		사용되는 개념이 아니기 때문에, 반드시 정수형일 필요가 없어
		실수형으로 계산할 것임.
	*/
	auto viewport_height = 2.0; // 이번엔 뷰포트 높이 먼저 정의
	auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height); // aspect_ratio 는 실제 이미지 사이즈의 종횡비와 달라, 실제 이미지 크기로부터 종횡비를 다시 계산해서 적용함.

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
			// r, g, b 값을 color 객체(vec3 클래스 별칭)로 만들어 둠.
			auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
			write_color(std::cout, pixel_color); // color 객체에 정의된 색상값을 스트림 출력하는 유틸 함수 호출
		}
	}

	std::clog << "\rDone.					\n"; // 반복문이 종료되면 .ppm 에 출력할 색상 계산이 완료되었음을 std::clog 로 콘솔 출력함.
}