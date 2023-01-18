#pragma once
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
	class Double {
	public:
		inline Double() {};

		inline Double(double x, double y) {
			this->x = x;
			this->y = y;
		};
		~Double() = default;

		Double operator=(const Double& v) {
			x = v.x;
			y = v.y;
			return *this;
		}
		Double operator+=(const Double& v) {
			x += v.x;
			y += v.y;
			return *this;
		}
		friend Double operator+(Double first, Double const& second) {
			first += second;
			return first;
		}

		friend bool operator==(const Double& first, const Double& second) {
			return (first.x == second.x && first.y == second.y);
		}

		friend bool operator!=(const Double& first, const Double& second) {
			return !(first == second);
		}

		double x = 0;
		double y = 0;
	};
}

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
	class Float {
	public:
		inline Float() {};

		inline Float(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		};
		~Float() = default;
		Float operator=(const Float& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
		Float operator+=(const Float& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		friend Float operator+(Float first, Float const& second) {
			first += second;
			return first;
		}
		friend bool operator==(const Float& first, const Float& second) {
			return (first.x == second.x && first.y == second.y && first.z == second.z);
		}

		float x = 0;
		float y = 0;
		float z = 0;
	};
	class Int {
	public:
		inline Int() {};

		inline Int(int x, int y, int z) {
			this->x = x;
			this->y = y;
			this->z = z;
		};
		~Int() = default;

		Int operator=(const Int& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
		Int operator+=(const Int& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		friend Int operator+(Int first, Int const& second) {
			first += second;
			return first;
		}

		friend bool operator==(const Int& first, const Int& second) {
			return (first.x == second.x && first.y == second.y && first.z == second.z);
		}

		friend bool operator!=(const Int& first, const Int& second) {
			return !(first == second);
		}

		int x = 0;
		int y = 0;
		int z = 0;
	};
}

namespace Vector4 {
	class Float {
	public:
		inline Float() {};

		inline Float(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		};
		~Float() = default;

		Float operator=(const Float& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
			return *this;
		}
		Float operator+=(const Float& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}
		friend Float operator+(Float first, Float const& second) {
			first += second;
			return first;
		}

		friend bool operator==(const Float& first, const Float& second) {
			return (first.x == second.x && first.y == second.y && first.z == second.z && first.w == second.w);
		}

		friend bool operator!=(const Float& first, const Float& second) {
			return !(first == second);
		}

		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
	};
}

// Hash Functions
namespace std
{
	template <>
	struct hash<Vector3::Float>
	{
		size_t operator()(const Vector3::Float& v) const
		{
			size_t res = 17;
			res = res * (31 + hash<float>()(v.x));
			res = res * (31 + hash<float>()(v.y));
			res = res * (31 + hash<float>()(v.z));
			return res;
		}
	};
}



