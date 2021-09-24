#pragma once

namespace PhysicsEngine {

	typedef float real;

	class PhysicsVector
	{
	public:

		real x;
		real y;
		real z;

	private:
		real pad;

	public:
		PhysicsVector() : x(0), y(0), z(0) {}
		PhysicsVector(const real x, const real y, const real z) : x(x), y(y), z(z) {}

		// 벡터를 주어진 스칼라 값만큼 곱한다.
		void operator*=(const real value)
		{
			x *= value;
			y *= value;
			z *= value;
		}

		// 주어진 스칼라 값만큼 곱한 벡터를 리턴
		PhysicsVector operator*(const real value) const
		{
			return PhysicsVector(x*value, y*value, z*value);
		}

		// 주어진 벡터만큼 더해준다
		void operator+=(const PhysicsVector& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}

		// 주어진 벡터만큼 더한 벡터를 반환
		PhysicsVector operator+(const PhysicsVector& v) const
		{
			return PhysicsVector(x + v.x, y + v.y, z + v.z);
		}

		// 주어진 벡터만큼 뺌
		void operator-=(const PhysicsVector& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		// 주어진 벡터만큼 밴 벡터를 반환
		PhysicsVector operator-(const PhysicsVector& v) const
		{
			return PhysicsVector(x - v.x, y - v.y, z - v.z);
		}

		// 주어진 벡터를 주어진 스칼라만큼 더한 다음 이 벡터에 더함
		void addScaledVector(const PhysicsVector& vector, real scale)
		{
			x += vector.x * scale;
			y += vector.y * scale;
			z += vector.z * scale;
		}

		// 주어진 벡터의 성분을 이 벡터의 각 성분에 곱한 다음 반환
		PhysicsVector componentProduct(const PhysicsVector& vector) const
		{
			return PhysicsVector(x * vector.x, y * vector.y, z * vector.z);
		}

		// 주어진 벡터의 성분을 이 벡터의 각 성분에 곱함
		void componentProductUpdate(const PhysicsVector& vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
		}

		// 이 벡터와 주어진 벡터의 내적을 계산하여 반환
		real scalarProduct(const PhysicsVector& vector) const
		{
			return x * vector.x + y * vector.y + z * vector.z;
		}

		// 이 벡터와 주어진 벡터의 내적을 계산하여 반환
		real operator *(const PhysicsVector& vector) const
		{
			return x * vector.x + y * vector.y + z * vector.z;
		}

		// 이 벡터와 주어진 벡터의 벡터곱을 계산하여 반환
		PhysicsVector vectorProduct(const PhysicsVector& vector) const
		{
			return PhysicsVector(y*vector.z - z * vector.y,
				z*vector.x - x * vector.z,
				x*vector.y - y * vector.x);
		}

		// 이 벡터와 주어진 벡터의 벡터곱을 계산한 다음 이 벡터에 업데이트
		void operator %=(const PhysicsVector& vector)
		{
			*this = vectorProduct(vector);
		}

		PhysicsVector operator%(const PhysicsVector& vector) const
		{
			return PhysicsVector(y*vector.z - z * vector.y,
				z*vector.x - x * vector.z,
				x*vector.y - y * vector.x); 
		}

		void invert()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		real real_sqrt(real data);

		// 벡터의 크기 계산
		real magnitude() const
		{
			//return real_sqrt(x*x + y * y + z * z);
		}

		// 벡터의 크기의 제곱을 계산
		real squareMagnitude() const
		{
			return x * x + y * y + z * z;
		}

		// 벡터를 단위 벡터로 변환
		void normalize()
		{
			real l = magnitude();
			if (l > 0)
			{
				(*this) *= (((real)1) / l);
			}
		}



		~PhysicsVector();
		

	};
}
