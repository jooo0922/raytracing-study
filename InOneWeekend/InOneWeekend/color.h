#ifndef COLOR_H
#define COLOR_H
// 헤더 가드를 위한 전처리기 선언

#include "vec3.h" // color 를 vec3 클래스에 대한 별칭으로 선언하기 위해 포함

#include <iostream>

// vec3 에 대한 별칭으로써 color 선언
/*
    c++ 에서 using 키워드는 여러 가지 용도로 사용된다.

    주로 namespace 의 요소를 가져오는 경우 많이 사용되지만,
    vec3 클래스에 대한 별칭(alias)을 만드는 용도로도 사용된다.

    이를 통해 구체적으로 어떤 vec3 인지,
    코드의 맥락에 따라 별칭을 명시함으로써
    가독성을 높일 수 있다.
*/
using color = vec3;

/*
    main.cpp > main() 함수에서 했던
    색상 데이터를 cout 으로 출력하던 작업을 util 함수로 만든 것.

    다만 차이점이 있다면,
    color(vec3) 클래스로 생성된 객체를
    매개변수로 전달받아서 출력한다는 점.

    vec3 클래스를 만들어놨으면 써먹어야지!
*/
/*
    참고로, 매개변수를 참조(별칭, 참조 연산자)로 선언할 때, 
    std::ostream& out 로 선언하나
    std::ostream &out 로 선언하나 모두 동일함.

    주의할 점은, &out 으로 썼다고 해서, 
    절대로 주소 연산자로 쓰인 게 아니라는 점 주의!

    주소 연산자를 쓰려면 타입 정의 없이
    변수명만 단독으로 선언된 상태에서 & 기호가 붙어야 함.

    float num = 3.14;
    float &numRef = num; (또는 float& numRef = num; 도 가능)
    float* numPtr = &num;

    여기서 &numRef 는 참조 연산자(별칭)으로 쓰인 것이고,
    &num 이 주소 연산자로 쓰인 것임!
*/
void write_color(std::ostream& out, color pixel_color)
{
    // pixel_color 는 요소의 값이 0 ~ 1 사이로 정규화된 vec3 로 생성되어 전달되겠군.
    // pixel_color 내의 정규화된 요소의 값을 0 ~ 256 범위 내의 정수형으로 형변환하여 출력함.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif // !COLOR_H
