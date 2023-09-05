#ifndef VEC3_H
#define VEC3_H
// 헤더 가드를 위한 전처리기 선언

#include <cmath> // std::sqrt() 사용하기 위해 포함
#include <iostream>

// using 을 이용해서 namespace 안의 특정 함수만 가져올 수 있음.
// https://github.com/jooo0922/cpp-study/blob/main/TBCppStudy/Chapter4_3/Chapter4_3.cpp 참고
using std::sqrt; 

// vec3 클래스 구현
class vec3 {
public:
    double e[3]; // vec3 의 세 컴포넌트 멤버변수를 double 타입 배열로 선언

    // vec3 생성자 선언
    vec3() : e{ 0,0,0 } {} // 매개변수가 없는 기본생성자 > 영벡터로 초기화
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {} // vec3 의 세 컴포넌트를 직접 매개변수로 전달받을 때의 생성자 오버로딩

    // vec3 각 컴포넌트에 대한 getter 메서드
    // 따라서, 객체 내의 데이터를 변경하지 않기 때문에, const 를 붙여서
    // 객체 내 데이터 상태 불변을 보장하는 상수함수로 만듦. > '상수 객체'에서도 마음껏 호출 가능!
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } // -(vec3(a, b, c)) 연산자 오버로딩 정의

    /*
        두 [] 연산자 오버로딩 차이점

        1. 상수함수(const) 로 정의한 [] 연산자 오버로딩은
        vec3 내의 컴포넌트 배열 요소의 값을 복사하여 반환하기 때문에,
        vec3 객체 내의 상태를 변경하지 않는다.

        즉, 아래 코드와 같이 vec3 내의 컴포넌트 요소를 '읽을 때' 사용함.

        vec3 v(a, b, c);
        x = v[0];
        y = v[1];
        z = v[2]

        2. 반면, 반환타입이 참조(double&)로 선언된 [] 연산자 오버로딩은
        vec3 내의 컴포넌트 배열 요소의 원래 변수, 즉, e[i]에 대한 별칭을 반환함.

        참고로, & 연산자는 변수 앞에 붙으면 
        해당 변수가 가리키는 메모리 공간의 주소값을 의미하지만(주소 연산자),
        타입 뒤에 붙으면, 특정 변수에 대한 '별칭' 으로 사용될 수 있도록 함(참조 연산자).

        예를 들어, 
        double a = 10.0;
        double& b = a;
        이렇게 작성할 경우, 변수 b 는 변수 a 에 대한 별칭으로써,
        결국 10.0이 저장된 동일한 메모리 공간을 가리키도록 함.

        즉, 이 연산자 오버로딩은 e[i] 에 대한 '참조'를 반환하므로,
        해당 메모리 공간의 값을 직접 변경함으로써 객체의 상태를 변경한다.

        예를 들어,
        아래 코드와 같이 vec3 내의 컴포넌트 요소를 '변경'할 때 사용된다.

        vec3 v(a, b, c);
        v[1] = 2;
    */
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    // += 연산자 오버로딩
    /*
        매개변수로 전달받는 vec3 는 변경하지 않아서 const 로 선언했고,
        현재의 vec3 인스턴스 자신의 상태값을 변경함.

        또, c++ 에서 this 는
        현재 메서드가 호출되고 있는 vec3 객체에 대한 포인터를 의미함.
        javascript 의 this 와 유사하다고 보면 됨.

        이때, *this 는 역참조 연산자로써,
        vec3 객체를 가리키는 포인터가 가리키는 메모리 공간에 저장된 값,
        즉, 객체 자체를 나타내는 값을 반환함.

        이렇게 객체 자신을 반환함으로써 장점이 뭐냐면,
        메서드 체이닝이 가능하다는 것.

        예를 들어, 
        vec3& doSomething1()
        {
            return *this
        }

        vec3& doSomething2()
        {
            return *this
        }

        로 메서드가 구현되어 있다면,
        vec3 v;
        v.doSomething1().doSomething2();

        이런 식으로 메서드 체이닝을 하는 데 용이하다는 것!
    */
    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // *= 연산자 오버로딩
    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    // /= 연산자 오버로딩
    vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    // 벡터의 길이를 계산하는 상수 함수들 정의
    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
};

// vec3 에 대한 별칭으로써 point3 선언
/*
    c++ 에서 using 키워드는 여러 가지 용도로 사용된다.

    주로 namespace 의 요소를 가져오는 경우 많이 사용되지만,
    vec3 클래스에 대한 별칭(alias)을 만드는 용도로도 사용된다.

    이를 통해 구체적으로 어떤 vec3 인지,
    코드의 맥락에 따라 별칭을 명시함으로써
    가독성을 높일 수 있다.
*/
using point3 = vec3;

// vec3 관련 utils (주로 연산자 오버로딩으로 구현됨.)
/*
    대부분의 연산자 오버로딩에 inline 키워드가 붙어있지?

    이는 함수를 컴파일하는 과정에서, 컴파일러가
    해당 함수의 호출 부분을 만나면 함수의 실제 코드로 직접 삽입해서
    컴파일하도록 요청하는 것을 의미함.

    이러한 행위를 '함수를 인라인한다' 라고 함.

    함수 내의 코드들을 호출부에 직접 인라인하면 뭐가 좋냐면,
    함수가 호출될 때마다 스택 프레임이 생성되는 것을 방지하고,
    매개변수 전달로 인한 값 복사를 방지하고,
    호출된 함수와 호출부 코드 간 컨텍스트 전환을 방지하는 등

    함수 호출에 의한 전반적인 오버헤드를 감소시킬 수 있음!

    다만, 함수 코드를 직접 삽입하다보면
    가독성이 떨어지고 코드의 크기가 증가하는 문제가 있으므로,
    
    함수의 코드가 짧고, 함수가 자주 호출되는 경우 
    함수를 인라인으로 컴파일하는 것이 권장됨.

    물론, inline 키워드를 붙인다고 하더라도,
    컴파일러가 최적화를 위해 상황에 따라 함수를 인라인 처리하지 않을수도 있음.
    어디까지나 컴파일러에게 '제안'을 하는 것에 불과함.
*/
/*
    매개변수들 중에서 피연산자인 vec3 들은
    객체 내부 데이터를 변경하지 않을 것이기 때문에,
    상수 객체로 선언하기 위해 const 를 붙임.
*/

// << 연산자 오버로딩
/*
    std::ostream 은 std::cout 같은
    스트림 출력을 담당하는 클래스 타입을 의미함.

    << 연산자를 오버로딩할 경우,
    아래와 같이 std::cout 같은 표준 스트림 출력 클래스를 사용해서
    현재 vec3 클래스에 관한 콘솔 출력을 사용자가 직접 정의할 수 있음.
    
    vec3 v(1.0, 2.0, 3.0);
    std::cout << v; // 1.0 2.0 3.0 요렇게 콘솔에 출력되겠지
*/
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// + 연산자 오버로딩
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// - 연산자 오버로딩
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// * 연산자 오버로딩
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// 매개변수에 따른 * 연산자 오버로딩 세분화
inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

// / 연산자 오버로딩
inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

// 벡터 내적 연산
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

// 벡터 외적 연산
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// 벡터 정규화 연산
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

#endif // !VEC3_H
