#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// 주어진 반직선(ray)에 대한 특정 색상을 반환하는 함수
color ray_color(const ray& r)
{
	// 반직선을 길이가 1인 단위벡터로 정규화한 뒤,
	// 정규화된 단위벡터의 y값에 따라 색상을 혼합하여 수직방향 그라디언트를 적용해 봄.
	vec3 unit_direction = unit_vector(r.direction()); // vec3.h 에 정의된 벡터 정규화 유틸 함수 사용
	auto a = 0.5 * (unit_direction.y() + 1.0); // -1 ~ 1 사이의 정규화된 단위벡터 y 값 범위를 0 ~ 1 사이로 맵핑함.
	
	// linear interpolation(선형보간)으로 흰색과 파란색을 0 ~ 1 사이로 맵핑된 a값에 따라 혼합하여 최종 색상 반환
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

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


	// Camera

	auto focal_length = 1.0; // 카메라 중점(eye point)과 viewport 사이의 거리 (현재는 단위 거리 1로 지정함.)
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
	auto camera_center = point3(0, 0, 0); // 3D Scene 상에서 카메라 중점(eye point) > viewport 로 casting 되는 모든 ray 의 출발점이기도 함.

	/*
		여기서부터 계산되는 변수들은
		https://raytracing.github.io/books/RayTracingInOneWeekend.html > Figure 4 에 정리된
		viewport 구조에 존재하는 벡터와 정점들을 선언 및 초기화한 것임.
	*/
	// 뷰포트 왼쪽 끝에서 오른쪽 끝으로 향하는 수평 방향 벡터(viewport_u) 와
	// 뷰포트 위쪽 끝에서 아래쪽 끝으로 향하는 수직 방향 벡터(viewport_v) 정의
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	// pixel grid 의 각 픽셀 사이의 수평 방향 간격을 나타내는 벡터(pixel_delta_u)와
	// pixel grid 의 각 픽셀 사이의 수직 방향 간격을 나타내는 벡터(pixel_delta_v) 정의
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	// 뷰포트의 좌상단 꼭지점의 '3D 공간 상의' 좌표 계산 (이미지 좌표 아님 주의!!) (Figure 4 에서 Q 로 표시)
	// 카메라 원점에서 focal_length 만큼 음의 z축으로 이동 후, 뷰포트 수평 길이의 절반만큼 왼쪽으로,
	// 뷰포트 수직 길이의 절반만큼 위쪽으로 이동
	auto viewport_upper_left = camera_center
							- vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

	// 'pixel grid'의 좌상단 픽셀(이미지 좌표 상으로 (0,0)에 해당하는 픽셀)의 '3D 공간 상의' 좌표 계산 (Figure 4 에서 P0,0 으로 표시)
	// '뷰포트 좌상단 꼭지점'에서 픽셀 간격의 절반씩만큼 오른쪽, 아래쪽으로 이동
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


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
			// 뷰포트 각 픽셀 중점의 '3D 공간 상의' 좌표 계산
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			
			// 카메라 중점 ~ 뷰포트 각 픽셀 중점까지 향하는 방향벡터 계산
			auto ray_direction = pixel_center - camera_center;

			// 카메라 ~ 뷰포트 각 픽셀 중점까지 향하는 반직선(ray) 타입 변수 r 선언 및 초기화
			ray r(camera_center, ray_direction);

			auto pixel_color = ray_color(r); // 주어진 반직선(ray) r 을 입력받아 특정 색상을 반환받아 픽셀 색상 계산
			write_color(std::cout, pixel_color); // color 객체에 정의된 색상값을 스트림 출력하는 유틸 함수 호출
		}
	}

	std::clog << "\rDone.					\n"; // 반복문이 종료되면 .ppm 에 출력할 색상 계산이 완료되었음을 std::clog 로 콘솔 출력함.
}