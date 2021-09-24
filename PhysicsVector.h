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

		// ���͸� �־��� ��Į�� ����ŭ ���Ѵ�.
		void operator*=(const real value)
		{
			x *= value;
			y *= value;
			z *= value;
		}

		// �־��� ��Į�� ����ŭ ���� ���͸� ����
		PhysicsVector operator*(const real value) const
		{
			return PhysicsVector(x*value, y*value, z*value);
		}

		// �־��� ���͸�ŭ �����ش�
		void operator+=(const PhysicsVector& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}

		// �־��� ���͸�ŭ ���� ���͸� ��ȯ
		PhysicsVector operator+(const PhysicsVector& v) const
		{
			return PhysicsVector(x + v.x, y + v.y, z + v.z);
		}

		// �־��� ���͸�ŭ ��
		void operator-=(const PhysicsVector& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		// �־��� ���͸�ŭ �� ���͸� ��ȯ
		PhysicsVector operator-(const PhysicsVector& v) const
		{
			return PhysicsVector(x - v.x, y - v.y, z - v.z);
		}

		// �־��� ���͸� �־��� ��Į��ŭ ���� ���� �� ���Ϳ� ����
		void addScaledVector(const PhysicsVector& vector, real scale)
		{
			x += vector.x * scale;
			y += vector.y * scale;
			z += vector.z * scale;
		}

		// �־��� ������ ������ �� ������ �� ���п� ���� ���� ��ȯ
		PhysicsVector componentProduct(const PhysicsVector& vector) const
		{
			return PhysicsVector(x * vector.x, y * vector.y, z * vector.z);
		}

		// �־��� ������ ������ �� ������ �� ���п� ����
		void componentProductUpdate(const PhysicsVector& vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
		}

		// �� ���Ϳ� �־��� ������ ������ ����Ͽ� ��ȯ
		real scalarProduct(const PhysicsVector& vector) const
		{
			return x * vector.x + y * vector.y + z * vector.z;
		}

		// �� ���Ϳ� �־��� ������ ������ ����Ͽ� ��ȯ
		real operator *(const PhysicsVector& vector) const
		{
			return x * vector.x + y * vector.y + z * vector.z;
		}

		// �� ���Ϳ� �־��� ������ ���Ͱ��� ����Ͽ� ��ȯ
		PhysicsVector vectorProduct(const PhysicsVector& vector) const
		{
			return PhysicsVector(y*vector.z - z * vector.y,
				z*vector.x - x * vector.z,
				x*vector.y - y * vector.x);
		}

		// �� ���Ϳ� �־��� ������ ���Ͱ��� ����� ���� �� ���Ϳ� ������Ʈ
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

		// ������ ũ�� ���
		real magnitude() const
		{
			//return real_sqrt(x*x + y * y + z * z);
		}

		// ������ ũ���� ������ ���
		real squareMagnitude() const
		{
			return x * x + y * y + z * z;
		}

		// ���͸� ���� ���ͷ� ��ȯ
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
