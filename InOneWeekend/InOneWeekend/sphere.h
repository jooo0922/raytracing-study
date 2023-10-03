#ifndef SPHERE_H
#define SPHERE_H
// 헤더 가드를 위한 전처리기 선언

#include "hittable.h" // 구체 클래스가 상속받을 hittable(피충돌 물체) 추상 클래스 사용을 위해 포함
#include "vec3.h" // hit() 메서드 재정의 시, 벡터 연산을 수행하기 위해 vec3 클래스 포함

// sphere(구체) 클래스를 hittable(피충돌 물체) 추상 클래스로부터 상속받아 정의
class sphere : public hittable
{
public:
	// 구체의 중점 좌표와 반지름을 매개변수로 받는 생성자 정의
	// 멤버변수 초기화 리스트 사용 (https://github.com/jooo0922/raytracing-study/blob/main/InOneWeekend/InOneWeekend/ray.h 관련 필기 참고)
	sphere(point3 _center, double _radius) : center(_center), radius(_radius) {}

	// 순수 가상 함수 hit 재정의(override) -> 하단 필기 참고
	bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override
	{
		// 반직선-구체 교차 여부를 검증하는 판별식 구현 (main.cpp > '근의 공식과 판별식 관련 필기 참고')
		vec3 oc = r.origin() - center; // 반직선 출발점 ~ 구체의 중점까지의 벡터 (본문 공식에서 (A-C) 에 해당)
		auto a = r.direction().length_squared(); // 반직선 방향벡터 자신과의 내적 (본문 공식에서 b⋅b 에 해당) > 벡터 자신과의 내적을 벡터 길이 제곱으로 리팩터링
		auto half_b = dot(oc, r.direction()); // 2 * 반직선 방향벡터와 (A-C) 벡터와의 내적 (본문 공식에서 2tb⋅(A−C) 에 해당) > half_b 로 변경
		auto c = oc.length_squared() - radius * radius; // (A-C) 벡터 자신과의 내적 - 반직선 제곱 (본문 공식에서 (A−C)⋅(A−C)−r^2 에 해당) > 벡터 자신과의 내적을 벡터 길이 제곱으로 리팩터링

		auto discriminant = half_b * half_b - a * c; // 근의 공식 판별식 계산 (b^2-4ac 에 해당. discriminant 는 근의 공식의 판별식을 뜻하는 영단어) > b = 2h 로 치환해서 근의 공식 간소화
		
		// 판별식이 0보다 작아 이차방정식의 해 t 가 존재하지 않을 경우,
		// 즉, 구체와 반직선이 서로 교차하는 지점이 없을 경우, false 반환하고 함수 종료
		if (discriminant < 0) return false;

		// 계산상의 편의를 위해 근의 공식에서 제곱근에 해당하는 항을 미리 구해놓음.
		auto sqrtd = sqrt(discriminant);

		// main.cpp 에 필기 정리 해놓았던 것처럼, 
		// 반직선과 구체의 교차점들 중에서 더 가까운 교차점에 해당하는 반직선 상의 비율값 t 를 먼저 계산
		auto root = (-half_b - sqrtd) / a;

		if (root <= ray_tmin || ray_tmax <= root)
		{
			// 만약, 더 가까운 교차점에 해당하는 반직선 상의 비율값 t 가 반직선의 유효범위를 벗어난다면,
			// 나머지 교차점에 대한 반직선 상의 비율값 t 를 다시 계산함.
			root = (-half_b + sqrtd) / a;
		
			if (root <= ray_tmin || ray_tmax <= root)
			{
				// 만약, 나머지 교차점에 대한 반직선 상의 비율값 t 조차도
				// 반직선 유효범위를 벗어난다면, 그냥 교차점이 없는 것으로 판단해서 false 반환하고 함수 종료
				return false;
			}
		}

		// 반직선 유효범위 내의 비율값 t 가 존재한다면,
		// 이 비율값을 가지고 충돌정보를 출력 매개변수 rec 에 저장함.
		// (https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/shadingwithsurfacenormals > 구체 표면 상의 노멀벡터 계산 관련 Figure 6 참고)
		rec.t = root; // 반직선 상에서 충돌 지점이 위치하는 비율값 t 저장
		rec.p = r.at(rec.t); // 충돌 지점의 좌표값 저장
		rec.normal = (rec.p - center) / radius; // 구체 표면 상에서 충돌 지점의 노멀벡터(방향벡터) 저장
		
		// 반직선 유효범위 내의 비율값 t가 존재한다면, 구체와 반직선의 충돌 지점이 존재하는 것으로 판단하여 true 반환
		return true;
	}

private:
	// 구체를 정의하는 데이터를 private 멤버변수로 정의
	point3 center; // 구체의 중심점 좌표 멤버변수
	double radius; // 구체의 반지름 멤버변수
};

#endif // !SPHERE_H


/*
	순수 가상 함수 hit() 재정의(override) 및 매개변수 설명


	hittable.h 에서 정리했던 것처럼,
	상속받는 추상 클래스의 순수 가상 함수는
	자식 클래스에서 반드시 재정의해줘야 함.

	이때, hit 함수는
	첫 번째 매개변수로 상수 참조변수 r (반직선) 을 입력받음.

	어떤 매개변수를 상수 참조변수로 선언할 경우,
	해당 매개변수는 외부에서 넘겨준 변수의 값을 또 다른 메모리 공간에
	복사하지 않아도 되면서,

	해당 함수 내부에서 값을 변경하지 못하게 막을 수 있음.

	즉, 순수한 '입력 매개변수'의 역할을 하는 것임.


	반면, 네 번째 매개변수 rec 는
	상수가 아닌 참조변수로 선언되어 있음.

	이는, 외부에서 넘겨준 변수 자체를 그대로 전달받음과 동시에,
	해당 외부 변수의 값을 직접 수정할 수 있게 해줌.

	그래서, '출력 매개변수'의 역할에 적합함.

	hit_record 타입의 매개변수는
	충돌에 관한 정보를 기록해두고 저장해야 하니
	
	당연히 '출력 매개변수'로 선언하는 것이 적합하겠지!

	
	참조에 의한 매개변수 전달 관련해서 
	아래에 관련 내용을 정리해놨으니 참고할 것!

	https://github.com/jooo0922/cpp-study/blob/main/TBCppChapter7/Chapter7_03/Chapter7_03.cpp


	또한, 두 번째와 세 번째 매개변수인
	ray_tmin, ray_tmax 는 뭐냐면,

	반직선 상의 비율값 t 가
	반직선의 유효범위 내에 존재하는지 검사하기 위해 넘겨준 값임.

	즉, 반직선이 양쪽 방향으로 무한하게 뻗어있는 것은 아니기 때문에,
	
	반직선의 출발 지점에 해당하는 비율값 t 와
	반직선의 끝 지점에 해당하는 비율값 t 를
	각각 ray_tmin, ray_tmax 라는 이름의 매개변수로 받음으로써,

	이 반직선 범위 내에 있는 비율값 t 지점에서 
	구체와 반직선이 교차한 경우에만 유효한 충돌로 보고,

	충돌에 관한 정보를 hit_record 타입의 출력 매개변수에 
	저장하려는 것임.
*/