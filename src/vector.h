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
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		unsigned char x = 0;
		unsigned char y = 0;
		unsigned char z = 0;
	};
}

namespace Vector2 {
	class Int {
	public:
		inline Int() {};

		inline Int(int x, int y) {
			this->x = x;
			this->y = y;
		};
		~Int() = default;

		Int operator=(const Int& v) {
			x = v.x;
			y = v.y;
			return *this;
		}
		Int operator+=(const Int& v) {
			x += v.x;
			y += v.y;
			return *this;
		}
		friend Int operator+(Int first, Int const& second) {
			first += second;
			return first;
		}

		friend bool operator==(const Int& first, const Int& second) {
			return (first.x == second.x && first.y == second.y);
		}

		friend bool operator!=(const Int& first, const Int& second) {
			return !(first == second);
		}

		int x = 0;
		int y = 0;
	};
}