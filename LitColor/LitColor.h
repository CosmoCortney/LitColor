#pragma once

#include <iostream>
#include <sstream>
#include <vector>

class LitColor
{
private:
	int32_t _redI = 0;
	int32_t _greenI = 0;
	int32_t _blueI = 0;
	int32_t _alphaI = 0;
	uint32_t _rgba = 0;
	float _redF = 0;
	float _greenF = 0;
	float _blueF = 0;
	float _alphaF = 0;
	bool _useAlpha = true;
	int _typeSelect = 0;
	bool _hadValidSourceValue = true;

	void generateFloatFromInt()
	{
		_redF = (float)_redI / 255.0f;
		_greenF = (float)_greenI / 255.0f;
		_blueF = (float)_blueI / 255.0f;
		_alphaF = (float)_alphaI / 255.0f;
	}

	void generateIntFromFloat()
	{
		_redI = (int)(_redF * 255.0f);
		_greenI = (int)(_greenF * 255.0f);
		_blueI = (int)(_blueF * 255.0f);
		_alphaI = (int)(_alphaF * 255.0f);
	}

	void generateRgbaFromInt()
	{
		_rgba = (_alphaI) | (_blueI << 8) | (_greenI << 16) | (_redI << 24);
	}

	void generateIntFromRgba()
	{
		_redI = _rgba >> 24;
		_greenI = (_rgba >> 16) & 0xFF;
		_blueI = (_rgba >> 8) & 0xFF;
		_alphaI = _rgba & 0xFF;
	}

	template<typename T> void validateAllColorMembers()
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			_hadValidSourceValue = _redF >= 0.0f && _redF <= 1.0f
				&& _greenF >= 0.0f && _greenF <= 1.0f
				&& _blueF >= 0.0f && _blueF <= 1.0f
				&& (_useAlpha ? (_alphaF >= 0.0f && _alphaF <= 1.0f) : true);
		}
		else if constexpr (std::is_integral_v<T>)
		{
			_hadValidSourceValue = _redF >= 0 && _redF <= 0xFF
				&& _greenF >= 0 && _greenF <= 0xFF
				&& _blueF >= 0 && _blueF <= 0xFF
				&& (_useAlpha ? (_alphaF >= 0 && _alphaF <= 0xFF) : true);
		}
	}

	template<typename T> void validateColorValue(T value)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			_hadValidSourceValue = value >= 0.0f && value <= 1.0f;
		}
		else if constexpr (std::is_integral_v<T>)
		{
			_hadValidSourceValue = value >= 0 && value <= 0xFF;
		}
	}

	template<typename T> void generateFromPtr(const T* rgba)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			_redF = *rgba;
			_greenF = *(rgba + 1);
			_blueF = *(rgba + 2);
			_alphaF = _useAlpha ? *(rgba + 3) : 1.0f;
			validateAllColorMembers<T>();
			generateIntFromFloat();
		}
		else if constexpr (std::is_integral_v<T>)
		{
			_redI = *rgba;
			_greenI = *(rgba + 1);
			_blueI = *(rgba + 2);
			_alphaI = _useAlpha ? *(rgba + 3) : 0xFF;
			validateAllColorMembers<T>();
			generateFloatFromInt();
		}

		generateRgbaFromInt();
	}

	template<typename T> T toMinMaxColorValue(T val)
	{
		if constexpr (std::is_floating_point_v<T>)
			if (val < 0.0f)
				return 0.0f;
			else if (val > 1.0f)
				return 1.0f;
			else
				return val;
		else if constexpr (std::is_integral_v<T>)
			if (val < 0)
				return 0;
			else if (val > 0xFF)
				return 0xFF;
			else
				return val;

		return val;
	}

	uint32_t addThreshold(const uint32_t a, const uint32_t b)
	{
		uint32_t val = a + b;
		if (val > 255)
			return 255;

		return val;
	}

	uint32_t subThreshold(const uint32_t a, const uint32_t b)
	{
		if (b > a)
			return 0;
		
		return a - b;
	}

	uint32_t mulThreshold(const uint32_t a, const uint32_t b)
	{
		uint32_t val = a * b;
		if (val > 255)
			return 255;

		return val;
	}

	bool isEqual(const uint32_t rgbaOther) const
	{
		if (_useAlpha)
			return _rgba == rgbaOther;
		return (_rgba & 0xFFFFFF00) == (rgbaOther & 0xFFFFFF00);
	}

public:
	LitColor() {}

	LitColor(const uint32_t rgba)
	{
		_rgba = rgba;
		generateIntFromRgba();
		generateFloatFromInt();
	}

	LitColor(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 0xFF)
	{
		_redI = r;
		_greenI = g;
		_blueI = b;
		_alphaI = a;
		generateRgbaFromInt();
		generateFloatFromInt();
	}

	LitColor(const float r, const float g, const float b, const float a = 1.0f)
	{
		_redF = r;
		_greenF = g;
		_blueF = b;
		_alphaF = a;
		validateAllColorMembers<float>();
		generateIntFromFloat();
		generateRgbaFromInt();
	}

	template<typename T> LitColor(const T* rgba, const bool containsAlpha = true)
	{
		_useAlpha = containsAlpha;
		generateFromPtr<T>(rgba);
		generateIntFromFloat();
		generateRgbaFromInt();
	}

	LitColor(std::string expression)
	{
		std::stringstream stream;

		if (expression.at(0) == '#')
		{
			expression.erase(0, 1);
			stream << expression;

			if (expression.size() <= 6)
			{
				stream << "FF";
				_useAlpha = false;
			}

			stream >> std::hex >> _rgba;
			generateIntFromRgba();
			generateFloatFromInt();
			return;
		}

		stream << expression;
		std::string item;
		std::vector<float> itemList;

		while (std::getline(stream, item, ','))
		{
			itemList.push_back(stof(item));
		}

		_redF = itemList[0];
		_greenF = itemList[1];
		_blueF = itemList[2];

		if (itemList.size() > 3)
		{
			_alphaF = itemList[3];
		}
		else
		{
			_alphaF = 1.0f;
			_useAlpha = false;
		}
		
		generateIntFromFloat();
		generateRgbaFromInt();
	}

	enum Colors
	{
		RED = 0,
		GREEN = 1,
		BLUE = 2,
		ALPHA = 3
	};

	enum Types
	{
		RGB888 = 0,
		RGBA8888 = 1,
		RGBF = 2,
		RGBAF = 3,
		RGB565 = 4//,
		//RGB332 = 5,
		//RGB444 = 6,
		//RGB555 = 7,
		//RGB101010 = 8
	};

	uint32_t GetRGBA()
	{
		return _rgba;
	}

	uint32_t GetRgbLeShift()
	{
		return _rgba >> 8;
	}

	int GetSelectedType()
	{
		return _typeSelect;
	}

	void SelectType(int type)
	{
		_typeSelect = type;
		_useAlpha = (type == RGBA8888 || type == RGBAF) ? true : false;
	}

	template<typename T> void SetColorValue(T value, int colorIndicator)
	{
		validateColorValue<T>(value);
		value = toMinMaxColorValue(value);

		if constexpr (std::is_integral_v<T>)
		{
			switch (colorIndicator)
			{
			case RED:
				_redI = value;
				break;
			case GREEN:
				_greenI = value;
				break;
			case BLUE:
				_blueI = value;
				break;
			case ALPHA:
				_alphaI = value;
				break;
			}

			generateFloatFromInt();
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			switch (colorIndicator)
			{
			case RED:
				_redF = value;
				break;
			case GREEN:
				_greenF = value;
				break;
			case BLUE:
				_blueF = value;
				break;
			case ALPHA:
				_alphaF = value;
				break;
			}

			generateIntFromFloat();
			generateRgbaFromInt();
		}
	}

	template<typename T> T GetColorValue(int colorIndicator)
	{
		if constexpr (std::is_integral_v<T>)
		{
			switch (colorIndicator)
			{
			case RED: return _redI;
			case GREEN: return _greenI;
			case BLUE: return _blueI;
			case ALPHA: return _alphaI;
			}
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			switch (colorIndicator)
			{
			case RED: return _redF;
			case GREEN: return _greenF;
			case BLUE: return _blueF;
			case ALPHA: return _alphaF;
			}
		}

		return 0;
	}

	void SetUseAlpha(bool shallI)
	{
		_useAlpha = shallI;
	}

	bool UsesAlpha()
	{
		return _useAlpha;
	}

	void operator=(const LitColor& other)
	{
		_redI = other._redI;
		_greenI = other._greenI;
		_blueI = other._blueI;
		_alphaI = other._alphaI;
		_rgba = other._rgba;
		_redF = other._redF;
		_greenF = other._greenF;
		_blueF = other._blueF;
		_alphaF = other._alphaF;
		_useAlpha = other._useAlpha;
		_typeSelect = other._typeSelect;
		_hadValidSourceValue = other._hadValidSourceValue;
	}

	template<typename T> void operator=(T* colors)
	{
		generateFromPtr(colors);
		generateRgbaFromInt();
	}

	void operator=(uint32_t rgba)
	{
		_rgba = rgba;
		generateIntFromRgba();
		generateFloatFromInt();
	}

	bool operator==(const LitColor& other) const
	{
		if (_useAlpha)
			return _rgba == other._rgba;

		return _rgba & 0xFFFFFF00 == other._rgba & 0xFFFFFF00;
	}

	bool operator==(const uint32_t rgba) const
	{
		return *this == LitColor(rgba);
	}

	template<typename T> bool operator==(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T>)
		{
			return _redI == *valPtr
				&& _greenI == *(valPtr + 1)
				&& _blueI == *(valPtr + 2)
				&& _useAlpha ? _alphaI == *(valPtr + 3) : true;
		}
		else if constexpr (std::is_floating_pointl_v<T>)
		{
			return _redF == *valPtr
				&& _greenF == *(valPtr + 1)
				&& _blueF == *(valPtr + 2)
				&& _useAlpha ? _alphaF == *(valPtr + 3) : true;
		}

		return false;
	}

	bool operator!=(const LitColor& other) const
	{
		return !(*this == other);
	}

	bool operator!=(const uint32_t rgba) const
	{
		return *this != LitColor(rgba);
	}

	template<typename T> bool operator!=(const T* valPtr) const
	{
		return !(*this == valPtr);
	}

	bool operator<(const LitColor& other) const
	{
		float leftSum = _useAlpha ? (_redF + _greenF + _blueF + _alphaF) / 4.0f : (_redF + _greenF + _blueF) / 3.0f;
		float rightSum = _useAlpha ? (other._redF + other._greenF + other._blueF + other._alphaF) / 4.0f : (other._redF + other._greenF + other._blueF) / 3.0f;
		return leftSum < rightSum;
	}

	bool operator<(const uint32_t rgba) const
	{
		return *this < LitColor(rgba);
	}

	template<typename T> bool operator<(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
			return *this < LitColor(valPtr);

		return false;
	}

	bool operator<=(const LitColor& other) const
	{
		return *this < other || *this == other;
	}

	bool operator<=(const uint32_t rgba) const
	{
		return *this <= LitColor(rgba);
	}

	template<typename T> bool operator<=(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
			return *this <= LitColor(valPtr);

		return false;
	}

	bool operator>(const LitColor& other) const
	{
		return !(*this <= other);
	}

	bool operator>(const uint32_t rgba) const
	{
		return !(*this <= LitColor(rgba));
	}

	template<typename T> bool operator>(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
			return !(*this <= LitColor(valPtr));

		return false;
	}

	bool operator>=(const LitColor& other) const
	{
		return !(*this < other);
	}

	bool operator>=(const uint32_t rgba) const
	{
		return !(*this < LitColor(rgba));
	}

	template<typename T> bool operator>=(const T* valPtr) const
	{
		if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
			return !(*this < LitColor(valPtr));

		return false;
	}

	LitColor operator+(LitColor other)
	{
		other._redI = addThreshold(other._redI, _redI);
		other._greenI = addThreshold(other._greenI, _greenI);
		other._blueI = addThreshold(other._blueI, _blueI);
		other._alphaI = addThreshold(other._alphaI, _alphaI);
		other.generateFloatFromInt();
		other.generateRgbaFromInt();
		return other;
	}

	LitColor operator+(uint32_t rgba)
	{
		return *this + LitColor(rgba);
	}

	template<typename T> LitColor operator+(T* valPtr)
	{
		return *this + LitColor(valPtr);
	}

	void operator+=(const LitColor other)
	{
		*this = *this + other;
	}

	void operator+=(const uint32_t rgba)
	{
		*this += LitColor(rgba);
	}

	template<typename T> void operator+=(const T* valPtr)
	{
		*this += LitColor(valPtr);
	}

	LitColor operator-(LitColor other)
	{
		other._redI = subThreshold(_redI, other._redI);
		other._greenI = subThreshold(_greenI, other._greenI);
		other._blueI = subThreshold(_blueI, other._blueI);
		other._alphaI = subThreshold(_alphaI, other._alphaI);
		other.generateFloatFromInt();
		other.generateRgbaFromInt();
		return other;
	}

	LitColor operator-(uint32_t rgba)
	{
		return *this - LitColor(rgba);
	}

	template<typename T> LitColor operator-(T* valPtr)
	{
		return *this - LitColor(valPtr);
	}

	void operator-=(const LitColor other)
	{
		*this = *this - other;
	}

	void operator-=(const uint32_t rgba)
	{
		*this -= LitColor(rgba);
	}

	template<typename T> void operator-=(const T* valPtr)
	{
		*this -= LitColor(valPtr);
	}

	LitColor operator*(LitColor other)
	{
		other._redI = mulThreshold(_redI, other._redI);
		other._greenI = mulThreshold(_greenI, other._greenI);
		other._blueI = mulThreshold(_blueI, other._blueI);
		other._alphaI = mulThreshold(_alphaI, other._alphaI);
		other.generateFloatFromInt();
		other.generateRgbaFromInt();
		return other;
	}

	LitColor operator*(uint32_t rgba)
	{
		return *this * LitColor(rgba);
	}

	template<typename T> LitColor operator*(T* valPtr)
	{
		return *this * LitColor(valPtr);
	}

	void operator*=(const LitColor other)
	{
		*this = *this * other;
	}

	void operator*=(const uint32_t rgba)
	{
		*this *= LitColor(rgba);
	}

	template<typename T> void operator*=(const T* valPtr)
	{
		*this *= LitColor(valPtr);
	}

	LitColor operator/(LitColor other)
	{
		other._redI = _redI / other._redI;
		other._greenI = _greenI / other._greenI;
		other._blueI = _blueI / other._blueI;
		other._alphaI = _alphaI / other._alphaI;
		other.generateFloatFromInt();
		other.generateRgbaFromInt();
		return other;
	}

	LitColor operator/(uint32_t rgba)
	{
		return *this / LitColor(rgba);
	}

	template<typename T> LitColor operator/(T* valPtr)
	{
		return *this / LitColor(valPtr);
	}

	void operator/=(const LitColor other)
	{
		*this = *this / other;
	}

	void operator/=(const uint32_t rgba)
	{
		*this /= LitColor(rgba);
	}

	template<typename T> void operator/=(const T* valPtr)
	{
		*this /= LitColor(valPtr);
	}

	LitColor operator&(LitColor other)
	{
		return LitColor(other._rgba & _rgba);
	}

	LitColor operator&(uint32_t rgba)
	{
		return *this & LitColor(rgba);
	}

	template<typename T> LitColor operator&(T* valPtr)
	{
		return *this & LitColor(rgba);
	}

	void operator&=(const LitColor other)
	{
		*this = *this & other;
	}

	void operator&=(const uint32_t rgba)
	{
		*this &= LitColor(rgba);
	}

	template<typename T> void operator&=(const T* valPtr)
	{
		*this &= LitColor(valPtr);
	}

	LitColor operator|(LitColor other)
	{
		return LitColor(other._rgba | _rgba);
	}

	LitColor operator|(uint32_t rgba)
	{
		return *this | LitColor(rgba);
	}

	template<typename T> LitColor operator|(T* valPtr)
	{
		return *this | LitColor(rgba);
	}

	void operator|=(const LitColor other)
	{
		*this | *this & other;
	}

	void operator|=(const uint32_t rgba)
	{
		*this |= LitColor(rgba);
	}

	template<typename T> void operator|=(const T* valPtr)
	{
		*this |= LitColor(valPtr);
	}

	LitColor operator^(LitColor other)
	{
		return LitColor(other._rgba ^ _rgba);
	}

	LitColor operator^(uint32_t rgba)
	{
		return *this ^ LitColor(rgba);
	}

	template<typename T> LitColor operator^(T* valPtr)
	{
		return *this ^ LitColor(rgba);
	}

	void operator^=(const LitColor other)
	{
		*this ^ *this & other;
	}

	void operator^=(const uint32_t rgba)
	{
		*this ^= LitColor(rgba);
	}

	template<typename T> void operator^=(const T* valPtr)
	{
		*this ^= LitColor(valPtr);
	}

	LitColor operator~()
	{
		return LitColor(~_rgba);
	}

	void Print()
	{
		std::cout << "Uses Alpha: " << _useAlpha << std::endl;
		std::cout << "RGBA: #" << std::hex << _rgba << std::endl;
		std::cout << "Red Int: " << std::hex << _redI << " - " << std::dec << _redI << std::endl;
		std::cout << "Green Int: " << std::hex << _greenI << " - " << std::dec << _greenI << std::endl;
		std::cout << "Blue Int: " << std::hex << _blueI << " - " << std::dec << _blueI << std::endl;
		std::cout << "Alpha Int: " << std::hex << _alphaI << " - " << std::dec << _alphaI << std::endl;
		std::cout << "Red Float: " << _redF << std::endl;
		std::cout << "Green Float: " << _greenF << std::endl;
		std::cout << "Blue Float: " << _blueF << std::endl;
		std::cout << "Alpha Float: " << _alphaF << std::endl;
	}

	bool HadValidColorSource()
	{
		return _hadValidSourceValue;
	}
};