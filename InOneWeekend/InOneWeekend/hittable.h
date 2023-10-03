#ifndef HITTABLE_H
#define HITTABLE_H
// 헤더 가드를 위한 전처리기 선언

#include "ray.h" // hittable(피충돌 물체)와의 충돌을 검사할 반직선을 정의할 ray 클래스 포함

// hit_record(충돌 정보) 클래스 정의
class hit_record
{
public:
	point3 p; // 반직선과 충돌한 지점의 좌표값
	vec3 normal; // 반직선과 충돌한 지점의 노멀벡터
	double t; // 반직선 상에서 충돌한 지점이 위치한 비율값 t
};

// hittable (피충돌 물체) 추상 클래스 정의
class hittable
{
public:
	virtual ~hittable() = default; // 하단 필기 '가상 소멸자와 default' 내용 참고

	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0; // 하단 필기 '순수 가상 함수와 추상 클래스' 내용 참고 
};

#endif // !HITTABLE_H

/*
	가상 소멸자(destructor)와 default


	1. 소멸자

	C++ 은 다른 언어의 클래스들과 달리,
	생성자와 반대되는 개념인 '소멸자(destructor)' 라는 문법을 갖추고 있음.

	new 키워드를 사용하거나 변수 타입을 해당 클래스로 지정하는 방식으로 
	클래스를 객체로 생성할 때, 객체 생성에 필요한 메모리들이 할당되는데,
	이 시점에 호출되는 클래스 내의 특별한 함수가 '생성자(constructor)'임.

	생성자 내에서는 주로 멤버변수 초기화 등의 작업을 수행하는 것이고...
	이 정도까지는 상식이고 잘 알고 있던 부분이지?

	반대로, '소멸자(destructor)' 란 무엇일까?

	생성자와 정 반대로, 해당 객체가 생성된 scope 가 종료되거나,
	delete 키워드로 객체에 동적으로 할당된 메모리를 제거하는 시점에
	호출되는 클래스 내의 특별한 함수가 '소멸자' 임.

	생성자와 반대로 동적으로 할당된 메모리나 리소스 등을 정리하는 작업을
	수행하는 데 사용되고, 생성자와 구분하기 위해 물결(~) 표시로 선언함.


	2. 가상(virtual) 소멸자

	그런데 소멸자에 virtual 이 붙는 이유는 뭘까?

	class Parent 
	{
	public:
		~Parent()
		{
			// ...
		}
	}

	class Child : public Parent
	{
	public:
		~Child()
		{
			// ...
		}
	}

	위와 같은 상속관계를 갖는 두 클래스에서
	각각 일반 소멸자를 선언해둔 상태라면,

	int main()
	{
		Parent* ptr = new Child();

		delete ptr;

		return 0;
	}

	위와 같이 부모 클래스를 타입으로 하는 포인터 변수에
	자식 클래스를 객체로 생성하여 동적으로 메모리를 할당할 때,

	delete 키워드로 객체의 메모리를 해제하는 시점에
	부모 클래스의 소멸자인 ~Parent() 만 제대로 호출되고,
	자식 클래스의 소멸자인 ~Child() 는 호출되지 않는 문제점이 있음.

	만약, 자식 클래스 소멸자에서 특정 리소스 해제를 관리하고 있다면,
	리소스 해제가 제대로 발생하지 않아 메모리 누수로 이어질 수도 있겠지!

	따라서, 위와 같은 상황에서도 자식 클래스의 소멸자까지 깔끔하게 호출하고 싶다면,
	부모 클래스 소멸자에 virtual 키워드를 붙여 가상 소멸자로 만들어야 함!

	이렇게 함으로써, 부모 클래스를 상속받는 자식 클래스들의 소멸자들도
	모두 '가상 소멸자'로 자동 선언되고,

	객체의 소멸자가 호출되는 시점(delete 사용 등)에
	상속 계층 구조 상 맨 아래에 존재하는 자식 클래스의 소멸자부터 호출되고,
	순차적으로 부모 클래스의 소멸자까지 호출시킴으로써,

	상속 계층에 존재하는 모든 클래스의 소멸자를 호출시켜
	메모리 누수를 완전 방지할 수 있다!


	3. default

	생성자(constructor) 또는 소멸자(destructor) 에는
	default 키워드를 붙여서 '디폴트 생성자' 또는 '디폴트 소멸자'를 선언할 수 있음.

	디폴트는 별 다른 게 아니고,
	만약 어떤 클래스의 생성자와 소멸자가 아래와 같이 구현되어 있다면,

	class A
	{
	public:
		A()
		{
			// 구현부에서 아무런 작업도 하지 않는 생성자
		}

		~A()
		{
			// 구현부에서 아무런 작업도 하지 않는 소멸자
		}
	}
	
	위와 같이 클래스 내에 정의한 생성자와 소멸자가
	구현부에서 아무런 작업도 하지 않는다면, 
	굳이 저렇게 코드를 '구현부까지' 작성하기 귀찮잖아?

	이럴 때 아래와 같이 default 키워드를 사용해서
	컴파일러에게 아무런 작업도 수행하지 않는 기본 생성자와 기본 소멸자를
	자동으로 만들어두라고 명시적으로 선언하는 키워드라고 보면 됨.

	class A
	{
	public:
		A() = default;
		~A() = default;
	}

	이렇게 하면 똑같이 아무런 작업도 수행하지 않는
	기본 생성자와 기본 소멸자를 간결한 코드로 작성할 수 있다는 장점이 있음!
*/

/*
	순수 가상 함수(= 0) 와 추상클래스


	위에 hittable 클래스에서, 
	반직선과 피충돌 물체(hittable 객체)의 충돌을 검사하는 hit 메서드를 선언할 때,
	아래와 같이 선언했었지?

	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;

	일단, 이 선언부에서 주의깊게 봐야 할 3가지 키워드가 존재함.


	1. virtual
	
	이 함수는 일단 가상 함수로 만들어졌기 때문에,
	이 부모 클래스에서 파생된 자식 클래스에서 override 키워드로 
	함수를 재정의할 수 있음.


	2. const

	이 함수는 객체 내의 데이터(멤버변수)를 변경하지 않음을 뜻하는 
	'불변성'을 보장하는 함수라는 의미에서 '상수 멤버함수' 로 선언해 놓음.
	(https://github.com/jooo0922/opengl-study/blob/main/GettingStarted/Shaders/MyHeaders/shader_s.h 상수 멤버함수 관련 필기 참고)


	3. = 0 (순수 가상 함수)

	'= 0' 문법을 사용하여 이 함수를 '순수 가상 함수'로 선언함.

	클래스 내의 어떤 함수를 '순수 가상 함수'로 정의하면,
	이 함수의 본문(구현부)는 해당 클래스 내에서 정의할 수 없으며,
	해당 클래스를 상속받는 '자식 클래스 내에서 해당 함수의 구현부를 반드시 재정의(override)' 해야 함!
	
	만약 자식 클래스 내에서 해당 함수를 재정의하지 않으면 컴파일 에러를 발생시킴!

	또한, 이렇게 순수 가상 함수가 하나라도 포함된 클래스는
	'추상 클래스(abstract class)'가 되어버림.

	'추상 클래스'가 되어버리면,

	hittable object; // 컴파일 에러 발생
	hittable object = new hittable() // 컴파일 에러 발생

	이런 식으로, 
	해당 클래스의 객체를 생성하려고 시도할 때마다 에러가 발생함.

	말그대로 클래스가 '추상화' 되어버려서,
	단일한 객체를 생성하는 목적으로 사용할 수가 없고,
	다만, 어떤 자식 클래스가 이 추상 클래스를 상속받는 용도로만 사용할 수 있도록 제한됨.

	즉, 마치 하나의 '인터페이스(intercace)' 처럼 작동한다고 보면 됨.

	그래서, 추상 클래스를 만드는 가장 바람직한 방법은,

	멤버변수나 일반적인 함수는 일체 정의하지 않으면서,
	오로지 '순수 가상 함수' 만 정의해두는 방식으로 만들어서
	'인터페이스' 역할만 하도록 구현하는 것이 가장 좋다고 함.
*/