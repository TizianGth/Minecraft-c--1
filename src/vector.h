#pragma once

namespace Vector3 {
	class UnsignedChar {
	public:
		inline UnsignedChar() {};

		inline UnsignedChar(unsigned char x, unsigned char y, unsigned char z) {
			this->x = x;
			this->y = y;
			this->z = z;
		};
		~UnsignedChar() = default;

		UnsignedChar operator=(const UnsignedChar& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		unsigned char x = 0;
		unsigned char y = 0;
		unsigned char z = 0;
	};
}