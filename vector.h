#pragma once
#include <iostream>
#include <cmath>
#include <complex>

namespace algebra {

namespace vector{
template <typename Val>
struct Vector {
	Val x, y, z;
	Vector (Val x, Val y, Val z) : x(x), y(y), z(z) { }
	Val module() {
		return std::sqrt(x*x + y*y + z*z);
	}
};

template <typename Val>
Vector<Val> conj(const Vector<Val>& a) {
	//return {std::conj(a.x), std::conj(a.y), std::conj(a.z)};
	return a;
}

template <typename Val>
Val ScalarMul(const Vector<Val>& a, const Vector<Val>& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename Val>
Vector<Val> ConstMul(Val a, const Vector<Val>& b) {
	return {a * b.x, a* b.y, a * b.z};
}

template <typename Val>
Vector<Val> VectorMul(const Vector<Val>& a, const Vector<Val>& b) {
	return {
		a.y * b.z - a.z * b.y, 
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

template <typename Val>
Vector<Val> Sum(const Vector<Val>& a, const Vector<Val>& b) {
	return {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	};
}

template <typename Val>
Vector<Val> Substraction(const Vector<Val>& a, const Vector<Val>& b) {
	return {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	};
}

template <typename Val>
Vector<Val> operator+(const Vector<Val>& a, const Vector<Val>& b) {
	return Sum(a, b);
}

template <typename Val>
Vector<Val>& operator+=(Vector<Val>& a, const Vector<Val>& b) {
	a = a + b;
	return a;
}

template <typename Val>
Vector<Val> operator-(const Vector<Val>& a, const Vector<Val>& b) {
	return Substraction(a, b);
}

template <typename Val>
Vector<Val>& operator-=(Vector<Val>& a, const Vector<Val>& b) {
	a = a - b;
	return a;
}

template <typename Val>
Val operator*(const Vector<Val>& a, const Vector<Val>& b) {
	return ScalarMul(a, b);
}

template <typename Val>
Vector<Val> operator*(Val a, const Vector<Val>& b) {
	return ConstMul(a, b);
}

template <typename Val>
Vector<Val> operator*(const Vector<Val>& b, Val a) {
	return a * b;
}

template <typename Val>
Vector<Val> operator^(const Vector<Val>& a, const Vector<Val>& b) {
	return VectorMul(a, b);
}

template <typename Val>
std::ostream& operator<<(std::ostream& os, const Vector<Val>& v) {
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

}
}
